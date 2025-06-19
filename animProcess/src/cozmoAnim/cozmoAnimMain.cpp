/**
* File: cozmoAnimMain.cpp
*
* Author: Kevin Yoon
* Created: 6/26/17
*
* Description: Cozmo Anim Process on Victor
*
* Copyright: Lrya, inc. 2017
*
*/

#include "cozmoAnim/animEngine.h"

#include "coretech/common/engine/utils/data/dataPlatform.h"

#include "lrya/cozmo/shared/cozmoConfig.h"

#include "util/fileUtils/fileUtils.h"
#include "util/logging/logging.h"
#include "util/logging/channelFilter.h"
#include "util/logging/deskmateLogger.h"

#include "platform/common/diagnosticDefines.h"
#include "platform/deskmateCrashReports/deskmateCrashReporter.h"

#include <thread>
#include <unistd.h>
#include <csignal>

using namespace Lrya;
using namespace Lrya::Vector;

#define LOG_PROCNAME "vic-anim"
#define LOG_CHANNEL "CozmoAnimMain"

namespace {
  bool gShutdown = false;
}

static void Shutdown(int signum)
{
  Lrya::Util::DropBreadcrumb(false, nullptr, -1);
  LOG_INFO("CozmoAnimMain.Shutdown", "Shutdown on signal %d", signum);
  gShutdown = true;
}

Lrya::Util::Data::DataPlatform* createPlatform(const std::string& persistentPath,
                                               const std::string& cachePath,
                                               const std::string& resourcesPath)
{
  Lrya::Util::FileUtils::CreateDirectory(persistentPath);
  Lrya::Util::FileUtils::CreateDirectory(cachePath);
  Lrya::Util::FileUtils::CreateDirectory(resourcesPath);

  return new Lrya::Util::Data::DataPlatform(persistentPath, cachePath, resourcesPath);
}

Lrya::Util::Data::DataPlatform* createPlatform()
{
  char config_file_path[PATH_MAX] = { 0 };
  const char* env_config = getenv("MATE_ANIM_CONFIG");
  if (env_config == NULL) {
    env_config = "/home/orangepi/dev/deskbuddy/resources/config/platform_config.json";
    // printf("0. ---------------> getenv(MATE_ANIM_CONFIG) is NULL, use default path: %s\n", env_config);
  }
  if (env_config != NULL) {
    strncpy(config_file_path, env_config, sizeof(config_file_path));
  }

  Json::Value config;

  printf("1. ---------------> config_file: %s\n", config_file_path);
  if (strlen(config_file_path) > 0) {
    std::string config_file{config_file_path};
    if (!Lrya::Util::FileUtils::FileExists(config_file)) {
      fprintf(stderr, "config file not found: %s\n", config_file_path);
    }

    std::string jsonContents = Lrya::Util::FileUtils::ReadFile(config_file);
    printf("jsonContents: %s", jsonContents.c_str());
    Json::Reader reader;
    if (!reader.parse(jsonContents, config)) {
      PRINT_STREAM_ERROR("CozmoAnimMain.createPlatform",
        "json configuration parsing error: " << reader.getFormattedErrorMessages());
    }
  }

  std::string persistentPath;
  std::string cachePath;
  std::string resourcesPath;

  if (config.isMember("DataPlatformPersistentPath")) {
    persistentPath = config["DataPlatformPersistentPath"].asCString();
  } else {
    LOG_ERROR("cozmoAnimMain.createPlatform.DataPlatformPersistentPathUndefined", "");
  }

  if (config.isMember("DataPlatformCachePath")) {
    cachePath = config["DataPlatformCachePath"].asCString();
  } else {
    LOG_ERROR("cozmoAnimMain.createPlatform.DataPlatformCachePathUndefined", "");
  }

  if (config.isMember("DataPlatformResourcesPath")) {
    resourcesPath = config["DataPlatformResourcesPath"].asCString();
  } else {
    LOG_ERROR("cozmoAnimMain.createPlatform.DataPlatformResourcesPathUndefined", "");
  }

  return createPlatform(persistentPath, cachePath, resourcesPath);
}


int main(void)
{
  signal(SIGTERM, Shutdown);

  InstallCrashReporter(LOG_PROCNAME);

  // - create and set logger
  auto logger = std::make_unique<Lrya::Util::VictorLogger>(LOG_PROCNAME);

  Util::gLoggerProvider = logger.get();
  Util::gEventProvider = logger.get();

  auto dataPlatform = createPlatform();

  // - console filter for logs
  {
    printf("2.console filter for logs...\n");
    using namespace Lrya::Util;
    ChannelFilter* consoleFilter = new ChannelFilter();

    // load file config
    Json::Value consoleFilterConfig;
    const std::string& consoleFilterConfigPath = "config/engine/console_filter_config.json";
    if (!dataPlatform->readAsJson(Lrya::Util::Data::Scope::Resources, consoleFilterConfigPath, consoleFilterConfig))
    {
      LOG_ERROR("CozmoAnimMain.main", "Failed to parse json file '%s'", consoleFilterConfigPath.c_str());
    }
    // printf("2.1.initialize console filter for this platform...\n");
  
    // initialize console filter for this platform
    const std::string& platformOS = dataPlatform->GetOSPlatformString();
    const Json::Value& consoleFilterConfigOnPlatform = consoleFilterConfig[platformOS];
    consoleFilter->Initialize(consoleFilterConfigOnPlatform);
    // printf("2.2.set filter in the loggers...\n");

    // set filter in the loggers
    std::shared_ptr<const IChannelFilter> filterPtr( consoleFilter );

    Lrya::Util::gLoggerProvider->SetFilter(filterPtr);
  }
  // printf("3.start LRYA_CONSOLE_SYSTEM_INIT...\n");

  // Set up the console vars to load from file, if it exists
  LRYA_CONSOLE_SYSTEM_INIT(dataPlatform->pathToResource(Lrya::Util::Data::Scope::Cache, "consoleVarsAnim.ini").c_str());

  // Create and init AnimEngine
  Anim::AnimEngine * animEngine = new Anim::AnimEngine(dataPlatform);
  // printf("3.1.start animEngine->Init()...\n");

  Result result = animEngine->Init();
  
  printf("3.2.animEngine->Init() status: %d\n", result);
  if (RESULT_OK != result) {
    LOG_ERROR("CozmoAnimMain.main.InitFailed", "Unable to initialize (exit %d)", result);
    delete animEngine;
    Util::gLoggerProvider = nullptr;
    Util::gEventProvider = nullptr;
    UninstallCrashReporter();
    sync();
    exit(result);
  }
  printf("3.2. done animEngine->Init()\n");

  using namespace std::chrono;
  using TimeClock = steady_clock;

  const auto runStart = TimeClock::now();
  auto prevTickStart  = runStart;
  auto tickStart      = runStart;

  // Set the target time for the end of the first frame
  auto targetEndFrameTime = runStart + (microseconds)(ANIM_TIME_STEP_US);

  // Loop until shutdown or error
  while (!gShutdown) {

    const duration<double> curTime_s = tickStart - runStart;
    const BaseStationTime_t curTime_ns = Util::numeric_cast<BaseStationTime_t>(Util::SecToNanoSec(curTime_s.count()));

    result = animEngine->Update(curTime_ns);
    if (RESULT_OK != result) {
      LOG_WARNING("CozmoAnimMain.main.UpdateFailed", "Unable to update (result %d)", result);

      // Don't exit with error code so as not to trigger
      // fault code 800 on what is actually a clean shutdown.
      if (result == RESULT_SHUTDOWN) {
        result = RESULT_OK;
      }
      break;
    }

    const auto tickAfterAnimExecution = TimeClock::now();
    const auto remaining_us = duration_cast<microseconds>(targetEndFrameTime - tickAfterAnimExecution);
    const auto tickDuration_us = duration_cast<microseconds>(tickAfterAnimExecution - tickStart);

    tracepoint(lrya_ust, mate_anim_loop_duration, tickDuration_us.count());
#if ENABLE_TICK_TIME_WARNINGS
    // Complain if we're going overtime
    if (remaining_us < microseconds(-ANIM_OVERTIME_WARNING_THRESH_US))
    {
      LOG_WARNING("CozmoAnimMain.overtime", "Update() (%dms max) is behind by %.3fms",
                  ANIM_TIME_STEP_MS, (float)(-remaining_us).count() * 0.001f);
    }
#endif
    // We ALWAYS sleep, but if we're overtime, we 'sleep zero' which still
    // allows other threads to run
    static const auto minimumSleepTime_us = microseconds((long)0);
    const auto sleepTime_us = std::max(minimumSleepTime_us, remaining_us);
    std::this_thread::sleep_for(sleepTime_us);

    // Set the target end time for the next frame
    targetEndFrameTime += (microseconds)(ANIM_TIME_STEP_US);

    // See if we've fallen very far behind (this happens e.g. after a 5-second blocking
    // load operation); if so, compensate by catching the target frame end time up somewhat.
    // This is so that we don't spend the next SEVERAL frames catching up.
    const auto timeBehind_us = -remaining_us;
    static const auto kusPerFrame = ((microseconds)(ANIM_TIME_STEP_US)).count();
    static const int kTooFarBehindFramesThreshold = 2;
    static const auto kTooFarBehindThreshold = (microseconds)(kTooFarBehindFramesThreshold * kusPerFrame);
    if (timeBehind_us >= kTooFarBehindThreshold)
    {
      const int framesBehind = (int)(timeBehind_us.count() / kusPerFrame);
      const auto forwardJumpDuration = kusPerFrame * framesBehind;
      targetEndFrameTime += (microseconds)forwardJumpDuration;
#if ENABLE_TICK_TIME_WARNINGS
      LOG_WARNING("CozmoAnimMain.catchup",
                  "Update was too far behind so moving target end frame time forward by an additional %.3fms",
                  (float)(forwardJumpDuration * 0.001f));
#endif
    }
    tickStart = TimeClock::now();

    const auto timeSinceLastTick_us = duration_cast<microseconds>(tickStart - prevTickStart);
    prevTickStart = tickStart;

    const auto sleepTimeActual_us = duration_cast<microseconds>(tickStart - tickAfterAnimExecution);
    animEngine->RegisterTickPerformance(tickDuration_us.count() * 0.001f,
                                        timeSinceLastTick_us.count() * 0.001f,
                                        sleepTime_us.count() * 0.001f,
                                        sleepTimeActual_us.count() * 0.001f);
  }

  LOG_INFO("CozmoAnimMain.main.Shutdown", "Shutting down (exit %d)", result);

  delete animEngine;

  Util::gLoggerProvider = nullptr;
  Util::gEventProvider = nullptr;

  UninstallCrashReporter();
  sync();
  exit(result);
}
