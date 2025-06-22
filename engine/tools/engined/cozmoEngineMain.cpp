/**
* File: cozmoEngineMain.cpp
*
* Author: Various Artists
* Created: 6/26/17
*
* Description: Cozmo Engine Process on Victor
*
* Copyright: Lrya, inc. 2017
*
*/

#include "lrya/cozmo/shared/cozmoConfig.h"
#include "lrya/cozmo/shared/cozmoEngineConfig.h"
#include "coretech/common/engine/utils/data/dataPlatform.h"

#include "engine/cozmoAPI/cozmoAPI.h"
#include "engine/utils/parsingConstants/parsingConstants.h"

#include "platform/common/diagnosticDefines.h"

#include "util/console/consoleSystem.h"
#include "util/cpuProfiler/cpuProfiler.h"
#include "util/fileUtils/fileUtils.h"
#include "util/helpers/templateHelpers.h"

#include "util/logging/channelFilter.h"
#include "util/logging/iEventProvider.h"
#include "util/logging/iFormattedLoggerProvider.h"
#include "util/logging/logging.h"
#include "util/logging/DAS.h"
#include "util/logging/multiLoggerProvider.h"
#include "util/logging/deskmateLogger.h"
#include "util/string/stringUtils.h"

#include "lrya/cozmo/shared/factory/emrHelper.h"
#include "platform/deskmateCrashReports/deskmateCrashReporter.h"
#include <chrono>

#if !defined(DEV_LOGGER_ENABLED)
  #if FACTORY_TEST
    #define DEV_LOGGER_ENABLED 1
  #else
    #define DEV_LOGGER_ENABLED 0
  #endif
#endif

#if DEV_LOGGER_ENABLED
#include "engine/debug/devLoggerProvider.h"
#include "engine/debug/devLoggingSystem.h"
#endif

#include <getopt.h>
#include <unistd.h>
#include <csignal>


// What IP do we use for advertisement?
constexpr const char * ROBOT_ADVERTISING_HOST_IP = "127.0.0.1";

// What process name do we use for logging?
constexpr const char * LOG_PROCNAME = "mate-engine";

// What channel name do we use for logging?
#define LOG_CHANNEL "CozmoEngineMain"

// Global singletons
Lrya::Vector::CozmoAPI* gEngineAPI = nullptr;
Lrya::Util::Data::DataPlatform* gDataPlatform = nullptr;


namespace {

  // Termination flag
  bool gShutdown = false;

  // Private singleton
  std::unique_ptr<Lrya::Util::VictorLogger> gVictorLogger;

  #if DEV_LOGGER_ENABLED
  // Private singleton
  std::unique_ptr<Lrya::Util::MultiLoggerProvider> gMultiLogger;
  #endif

}

static void sigterm(int signum)
{
  Lrya::Util::DropBreadcrumb(false, nullptr, -1);
  LOG_INFO("CozmoEngineMain.SIGTERM", "Shutting down on signal %d", signum);
  gShutdown = true;
}

static void configure_engine_advertising(Json::Value& config)
{
  if (!config.isMember(LryaUtil::kP_ADVERTISING_HOST_IP)) {
    config[LryaUtil::kP_ADVERTISING_HOST_IP] = ROBOT_ADVERTISING_HOST_IP;
  }
  if (!config.isMember(LryaUtil::kP_UI_ADVERTISING_PORT)) {
    config[LryaUtil::kP_UI_ADVERTISING_PORT] = Lrya::Vector::UI_ADVERTISING_PORT;
  }
}

static Lrya::Util::Data::DataPlatform* createPlatform(const std::string& persistentPath,
                                                      const std::string& cachePath,
                                                      const std::string& resourcesPath)
{
  Lrya::Util::FileUtils::CreateDirectory(persistentPath);
  Lrya::Util::FileUtils::CreateDirectory(cachePath);
  Lrya::Util::FileUtils::CreateDirectory(resourcesPath);

  return new Lrya::Util::Data::DataPlatform(persistentPath, cachePath, resourcesPath);
}

static bool cozmo_start(const Json::Value& configuration)
{
  //
  // In normal usage, private singleton owns the logger until application exits.
  // When collecting developer logs, ownership of singleton VictorLogger is transferred to
  // singleton MultiLogger.
  //
  gVictorLogger = std::make_unique<Lrya::Util::VictorLogger>(LOG_PROCNAME);

  Lrya::Util::gLoggerProvider = gVictorLogger.get();
  Lrya::Util::gEventProvider = gVictorLogger.get();
  LOG_INFO("cozmo_start", "Initializing engine");

  std::string persistentPath;
  std::string cachePath;
  std::string resourcesPath;

  // copy existing configuration data
  Json::Value config(configuration);

  if (config.isMember("DataPlatformPersistentPath")) {
    persistentPath = config["DataPlatformPersistentPath"].asCString();
  } else {
    LOG_ERROR("cozmoEngineMain.DataPlatformPersistentPathUndefined", "");
  }

  if (config.isMember("DataPlatformCachePath")) {
    cachePath = config["DataPlatformCachePath"].asCString();
  } else {
    LOG_ERROR("cozmoEngineMain.DataPlatformCachePathUndefined", "");
  }

  if (config.isMember("DataPlatformResourcesPath")) {
    resourcesPath = config["DataPlatformResourcesPath"].asCString();
  } else {
    LOG_ERROR("cozmoEngineMain.DataPlatformResourcesPathUndefined", "");
  }

  gDataPlatform = createPlatform(persistentPath, cachePath, resourcesPath);
  LOG_DEBUG("CozmoStart.ResourcesPath", "%s", resourcesPath.c_str());

#if (USE_DAS || DEV_LOGGER_ENABLED)
  const std::string& appRunId = Lrya::Util::GetUUIDString();
#endif

  // - console filter for logs
  {
    using namespace Lrya::Util;
    ChannelFilter* consoleFilter = new ChannelFilter();
    
    // load file config
    Json::Value consoleFilterConfig;
    static const std::string& consoleFilterConfigPath = "config/engine/console_filter_config.json";
    if (!gDataPlatform->readAsJson(Lrya::Util::Data::Scope::Resources, consoleFilterConfigPath, consoleFilterConfig))
    {
      LOG_ERROR("cozmo_start", "Failed to parse Json file '%s'", consoleFilterConfigPath.c_str());
      return false;
    }

    LOG_INFO("cozmo_start", "----------> 1. Parsed Json file '%s'", consoleFilterConfigPath.c_str());

    // initialize console filter for this platform
    const std::string& platformOS = gDataPlatform->GetOSPlatformString();
    const Json::Value& consoleFilterConfigOnPlatform = consoleFilterConfig[platformOS];
    consoleFilter->Initialize(consoleFilterConfigOnPlatform);

    // set filter in the loggers
    std::shared_ptr<const IChannelFilter> filterPtr( consoleFilter );

    Lrya::Util::gLoggerProvider->SetFilter(filterPtr);
  }

#if DEV_LOGGER_ENABLED
  if(!FACTORY_TEST || (FACTORY_TEST && !Lrya::Vector::Factory::GetEMR()->fields.PACKED_OUT_FLAG))
  {
    // Initialize Developer Logging System
    using DevLoggingSystem = Lrya::Vector::DevLoggingSystem;
    const std::string& devlogPath = gDataPlatform->GetCurrentGameLogPath(LOG_PROCNAME);
    DevLoggingSystem::CreateInstance(devlogPath, appRunId);

    //
    // Replace singleton deskmate logger with a MultiLogger that manages both deskmate logger and dev logger.
    // Ownership of deskmate logger is transferred to MultiLogger.
    // Ownership of MultiLogger is managed by singleton unique_ptr.
    //
    std::vector<Lrya::Util::ILoggerProvider*> loggers = {gVictorLogger.release(), DevLoggingSystem::GetInstancePrintProvider()};
    gMultiLogger = std::make_unique<Lrya::Util::MultiLoggerProvider>(loggers);

    Lrya::Util::gLoggerProvider = gMultiLogger.get();
  }
#endif

  LOG_INFO("cozmo_start",
            "Creating engine; Initialized data platform with persistentPath = %s, cachePath = %s, resourcesPath = %s",
            persistentPath.c_str(), cachePath.c_str(), resourcesPath.c_str());

  configure_engine_advertising(config);

  // Set up the console vars to load from file, if it exists
  LRYA_CONSOLE_SYSTEM_INIT(gDataPlatform->GetCachePath("consoleVarsEngine.ini").c_str());

  Lrya::Vector::CozmoAPI* engineInstance = new Lrya::Vector::CozmoAPI();

  const bool engineStarted = engineInstance->Start(gDataPlatform, config);
  if (!engineStarted) {
    delete engineInstance;
    return false;
  }

  gEngineAPI = engineInstance;

  return true;
}

static void cozmo_stop()
{
  Lrya::Util::SafeDelete(gEngineAPI);
  Lrya::Util::SafeDelete(gDataPlatform);

  Lrya::Util::gEventProvider = nullptr;
  Lrya::Util::gLoggerProvider = nullptr;

#if DEV_LOGGER_ENABLED
  Lrya::Vector::DevLoggingSystem::DestroyInstance();
#endif

  sync();
}


int main(int argc, char* argv[])
{
  // Install signal handler
  signal(SIGTERM, sigterm);

  Lrya::Vector::InstallCrashReporter(LOG_PROCNAME);

  char cwd[PATH_MAX] = { 0 };
  (void)getcwd(cwd, sizeof(cwd));
  printf("CWD: %s\n", cwd);
  printf("argv[0]: %s\n", argv[0]);
  printf("exe path: %s/%s\n", cwd, argv[0]);

  int help_flag = 0;

  static const char *opt_string = "hc:";

  static const struct option long_options[] = {
      { "config",     required_argument,      NULL,           'c' },
      { "help",       no_argument,            &help_flag,     'h' },
      { NULL,         no_argument,            NULL,           0   }
  };

  char config_file_path[PATH_MAX] = { 0 };
  const char* env_config = getenv("MATE_ENGINE_CONFIG");
  if (env_config != NULL) {
    strncpy(config_file_path, env_config, sizeof(config_file_path));
  }

  while(true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, opt_string, long_options, &option_index);

    if (-1 == c) {
      break;
    }

    switch(c) {
      case 0:
      case 1:
      {
        if (long_options[option_index].flag != 0)
          break;
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;
      }
      case 'c':
      {
        strncpy(config_file_path, optarg, sizeof(config_file_path));
        config_file_path[PATH_MAX-1] = 0;
        break;
      }
      case 'h':
        help_flag = 1;
        break;
      case '?':
        break;
      default:
        abort();
    }
  }

  if (help_flag) {
    char* prog_name = basename(argv[0]);
    printf("%s <OPTIONS>\n", prog_name);
    printf("  -h, --help                          print this help message\n");
    printf("  -c, --config [JSON FILE]            load config json file\n");
    Lrya::Vector::UninstallCrashReporter();
    return 1;
  }

  Json::Value config;

  // printf("0. ----------------> config_file: %s\n", config_file_path);
  if (strlen(config_file_path) > 0) {
    std::string config_file{config_file_path};
    if (!Lrya::Util::FileUtils::FileExists(config_file)) {
      fprintf(stderr, "config file not found: %s\n", config_file_path);
      Lrya::Vector::UninstallCrashReporter();
      return 1;
    }

    std::string jsonContents = Lrya::Util::FileUtils::ReadFile(config_file);
    // printf("jsonContents: %s", jsonContents.c_str());
    Json::Reader reader;
    if (!reader.parse(jsonContents, config)) {
      printf("CozmoEngineMain.main: json configuration parsing error: %s\n",
             reader.getFormattedErrorMessages().c_str());
      Lrya::Vector::UninstallCrashReporter();
      return 1;
    }
  }

  const bool started = cozmo_start(config);
  if (!started) {
    printf("failed to start engine\n");
    Lrya::Vector::UninstallCrashReporter();
    return 1;
  }

  LOG_INFO("CozmoEngineMain.main", "Engine started");

  using namespace std::chrono;
  using TimeClock = steady_clock;

  const auto runStart = TimeClock::now();
  auto prevTickStart  = runStart;
  auto tickStart      = runStart;

  // Set the target time for the end of the first frame
  auto targetEndFrameTime = runStart + (microseconds)(Lrya::Vector::BS_TIME_STEP_MICROSECONDS);

  while (!gShutdown)
  {
    const duration<double> curTimeSeconds = tickStart - runStart;
    const double curTimeNanoseconds = Lrya::Util::SecToNanoSec(curTimeSeconds.count());

    const bool tickSuccess = gEngineAPI->Update(Lrya::Util::numeric_cast<BaseStationTime_t>(curTimeNanoseconds));

    const auto tickAfterEngineExecution = TimeClock::now();
    const auto remaining_us = duration_cast<microseconds>(targetEndFrameTime - tickAfterEngineExecution);
    const auto tickDuration_us = duration_cast<microseconds>(tickAfterEngineExecution - tickStart);

    tracepoint(lrya_ust, mate_engine_loop_duration, tickDuration_us.count());
#if ENABLE_TICK_TIME_WARNINGS
    // Only complain if we're more than 10ms behind
    if (remaining_us < microseconds(-10000))
    {
      LOG_WARNING("CozmoEngineMain.main.overtime", "Update() (%dms max) is behind by %.3fms",
                  Lrya::Vector::BS_TIME_STEP_MS, (float)(-remaining_us).count() * 0.001f);
    }
#endif

    // We ALWAYS sleep, but if we're overtime, we 'sleep zero' which still allows
    // other threads to run
    static const auto minimumSleepTime_us = microseconds((long)0);
    const auto sleepTime_us = std::max(minimumSleepTime_us, remaining_us);
    {
      using namespace Lrya;
      LRYA_CPU_PROFILE("CozmoEngineMain.main.Sleep");

      std::this_thread::sleep_for(sleepTime_us);
    }

    // Set the target end time for the next frame
    targetEndFrameTime += (microseconds)(Lrya::Vector::BS_TIME_STEP_MICROSECONDS);

    // See if we've fallen quite far behind; if so, compensate by catching the target frame end time up somewhat.
    // This is so that we don't spend SEVERAL frames trying to catch up (by depriving sleep time).
    const auto timeBehind_us = -remaining_us;
    static const auto kusPerFrame = ((microseconds)(Lrya::Vector::BS_TIME_STEP_MICROSECONDS)).count();
    static const int kTooFarBehindFramesThreshold = 2;
    static const auto kTooFarBehindThreshold = (microseconds)(kTooFarBehindFramesThreshold * kusPerFrame);
    if (timeBehind_us >= kTooFarBehindThreshold)
    {
      const int framesBehind = (int)(timeBehind_us.count() / kusPerFrame);
      const auto forwardJumpDuration = kusPerFrame * framesBehind;
      targetEndFrameTime += (microseconds)forwardJumpDuration;
#if ENABLE_TICK_TIME_WARNINGS
      LOG_WARNING("CozmoEngineMain.main.catchup",
                  "Update was too far behind so moving target end frame time forward by an additional %.3fms",
                  (float)(forwardJumpDuration * 0.001f));
#endif
    }

    tickStart = TimeClock::now();

    const auto timeSinceLastTick_us = duration_cast<microseconds>(tickStart - prevTickStart);
    prevTickStart = tickStart;

    const auto sleepTimeActual_us = duration_cast<microseconds>(tickStart - tickAfterEngineExecution);
    gEngineAPI->RegisterEngineTickPerformance(tickDuration_us.count() * 0.001f,
                                              timeSinceLastTick_us.count() * 0.001f,
                                              sleepTime_us.count() * 0.001f,
                                              sleepTimeActual_us.count() * 0.001f);

    if (!tickSuccess)
    {
      // If we fail to update properly, stop running (but after we've recorded the above stuff)
      LOG_INFO("CozmoEngineMain.main", "Engine has stopped");
      break;
    }
  } // End of tick loop

  LOG_INFO("CozmoEngineMain.main", "Stopping engine");
  cozmo_stop();

  Lrya::Vector::UninstallCrashReporter();

  return 0;
}
