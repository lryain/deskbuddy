#include <stdio.h>
#include <chrono>
#include <csignal>
#include <thread>
#include <sys/mman.h>
#include <sched.h>
#include <unistd.h>

#include "lrya/cozmo/robot/hal.h"
#include "lrya/cozmo/robot/logging.h"
#include "lrya/cozmo/robot/cozmoBot.h"
#include  "../spine/cc_commander.h"
#include "lrya/cozmo/shared/factory/emrHelper.h"

#include "platform/deskmateCrashReports/deskmateCrashReporter.h"

#define LOG_PROCNAME "mate-robot"

//
// Enable Lrya::Utils log provider?
//
#ifndef LRYA_ROBOT_DESKMATE_LOGGER
#define LRYA_ROBOT_DESKMATE_LOGGER 0
#endif

#if LRYA_ROBOT_DESKMATE_LOGGER

#include "util/logging/deskmateLogger.h"

namespace {
  Lrya::Util::VictorLogger gVictorLogger(LOG_PROCNAME);
}

#endif

// For development purposes, while HW is scarce, it's useful to be able to run on phones
#ifdef HAL_DUMMY_BODY
  #define HAL_NOT_PROVIDING_CLOCK
#endif

int shutdownSignal = 0;
const int SHUTDOWN_COUNTDOWN_TICKS = 5;
int shutdownCounter = SHUTDOWN_COUNTDOWN_TICKS;

#if FACTORY_TEST
// Count of how many ticks we have been on the charger
uint8_t seenChargerCnt = 0;
// How many ticks we need to be on the charger before
// we will stay on
static const uint8_t MAX_SEEN_CHARGER_CNT = 5;
bool wasPackedOutAtBoot = false;
#endif

static void Shutdown(int signum)
{
  if (shutdownSignal == 0) {
    LryaInfo("robot.shutdown", "Shutdown on signal %d", signum);

    // Need to HAL::Step() in order for light commands to go down to robot
    // so set shutdownSignal here to signal process shutdown after
    // shutdownCounter more tics of main loop.
    shutdownSignal = signum;
    shutdownCounter = SHUTDOWN_COUNTDOWN_TICKS;
  }
}


int run()
{
  using Result = Lrya::Result;

  LryaInfo("1.0. robot.main", "Starting robot process invoke --> Robot::Init(&shutdownSignal)");

  //Robot::Init calls HAL::INIT before anything else.
  // TODO: move HAL::Init here into HAL main.
  const Result result = Lrya::Vector::Robot::Init(&shutdownSignal);
  if (result != Result::RESULT_OK) {
    LryaError("robot.main.InitFailed", "Unable to initialize (result %d)", result);
    if (shutdownSignal == SIGTERM) {
      return 0;
    } else if (shutdownSignal != 0) {
      return shutdownSignal;
    } else {
      return result;
    }
  }

  printf("1.1. robot.main -------------> After Init:\n1.all memory ready\n2. IMU instantiated\n3.lock our pages \n");
  // After Init, all memory we need has been initialized and the IMU thread (if used) has been
  // instantiated, lock our pages
  int lock_r = mlockall(MCL_FUTURE);
  if (lock_r == -1) {
    LryaError("robot.main", "Failed to lock pages");
  }

  auto start = std::chrono::steady_clock::now();
#if FACTORY_TEST
  printf("1.1.1. robot.main -------------> FACTORY_TEST Enabled! \n");
  auto timeOfPowerOn = start;
  wasPackedOutAtBoot = Lrya::Vector::Factory::GetEMR()->fields.PACKED_OUT_FLAG;
#endif

  printf("1.2. robot.main -------------> start go into forever loop! invoke HAL::Step() \n");
  for (;;) {
    //HAL::Step should never return !OK, but if it does, best not to trust its data.
    if (Lrya::Vector::HAL::Step() == Lrya::RESULT_OK) {
      if (Lrya::Vector::Robot::step_MainExecution() != Lrya::RESULT_OK) {
        LryaError("robot.main.MainStepFailed", "");
        return -1;
      }
    } else {
      LryaError("robot.main.HALStepFailed", "");
      return -2;
    }

    auto end = std::chrono::steady_clock::now();
#ifdef HAL_NOT_PROVIDING_CLOCK
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::chrono::duration<double, std::micro> sleepTime = std::chrono::milliseconds(5) - elapsed;
    std::this_thread::sleep_for(sleepTime);
    ///printf("Main tic: %lld, Sleep time: %f us\n", elapsed.count(), sleepTime.count());
#endif
    //printf("TS: %d\n", Lrya::Vector::HAL::GetTimeStamp() );
    start = end;

#if FACTORY_TEST
    // If we are packed out and have not yet seen the charger
    if (wasPackedOutAtBoot &&
        shutdownSignal == 0 &&
        seenChargerCnt < MAX_SEEN_CHARGER_CNT)
    {
      // Need to be on the charger for some number of ticks
      if (Lrya::Vector::HAL::BatteryIsOnCharger())
      {
        seenChargerCnt++;
      }
      else
      {
        seenChargerCnt = 0;
      }

      // If it has been more than 15 seconds since power on and we
      // have not been on the charger then shutdown
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - timeOfPowerOn);
      if (elapsed > std::chrono::seconds(15))
      {
        Lrya::Vector::Robot::Destroy();

        sync();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        Lrya::Vector::HAL::Shutdown();
        break;
      }
    }
#endif

    if (shutdownSignal != 0) {
      if (shutdownCounter == SHUTDOWN_COUNTDOWN_TICKS) {
        Lrya::Vector::Robot::Destroy();
      } else if (shutdownCounter == 0) {
        LryaInfo("robot.main.shutdown", "%d", shutdownSignal);
        return 0;
      }
      --shutdownCounter;
    }
  }
  return 0;
}


int main(int argc, const char* argv[])
{
  // Set output buffering for use with systemd journal
  setlinebuf(stdout);
  setlinebuf(stderr);

  struct sched_param params;
  params.sched_priority = sched_get_priority_max(SCHED_FIFO);
  sched_setscheduler(0, SCHED_FIFO, &params);

  signal(SIGTERM, Shutdown);

  #if LRYA_ROBOT_DESKMATE_LOGGER
  Lrya::Util::gLoggerProvider = &gVictorLogger;
  Lrya::Util::gEventProvider = &gVictorLogger;
  #endif

  Lrya::Vector::InstallCrashReporter(LOG_PROCNAME);

  if (argc > 1) {
    ccc_set_shutdown_function(Shutdown);
    ccc_parse_command_line(argc-1, argv+1);
  }

  int res = run();

  Lrya::Vector::Robot::Destroy();

  Lrya::Vector::UninstallCrashReporter();

  #if LRYA_ROBOT_DESKMATE_LOGGER
  Lrya::Util::gLoggerProvider = nullptr;
  Lrya::Util::gEventProvider = nullptr;
  #endif

  sync();

  return res;
}

#ifdef DEBUG_SPINE_TEST
#include "spine/spine.h"

int main_test(int argc, const char* argv[])
{
  mlockall(MCL_FUTURE);

  struct sched_param params;
  params.sched_priority = sched_get_priority_max(SCHED_FIFO);
  sched_setscheduler(0, SCHED_FIFO, &params);

  signal(SIGTERM, Shutdown);

  spine_test_setup();

  while (1) {
    spine_test_loop_once();
    Lrya::Vector::Robot::step_MainExecution();
  }

  if (shutdownSignal != 0 && --shutdownCounter == 0) {
    LryaInfo("robot.main.shutdown", "%d", shutdownSignal);
    exit(shutdownSignal);
  }
  return 0;
}
#endif
