/**
* File: deskmateWebServerMain.cpp
*
* Author: Paul Terry
* Created: 01/29/18
*
* Description: Standalone Web Server Process on Victor
*
* Copyright: Lrya, inc. 2018
*
*/
#include "webService.h"

#include "coretech/common/engine/utils/data/dataPlatform.h"

#include "lrya/cozmo/shared/cozmoConfig.h"

#include "util/fileUtils/fileUtils.h"
#include "util/logging/logging.h"
#include "util/logging/deskmateLogger.h"

#include "platform/deskmateCrashReports/deskmateCrashReporter.h"

#include <thread>
#include <condition_variable>
#include <csignal>

using namespace Lrya;
using namespace Lrya::Vector;

#define LOG_PROCNAME "mate-webserver"
#define LOG_CHANNEL "VictorWebServer"

namespace
{
  volatile bool _running = true;
}

static void Shutdown(int signum)
{
  LOG_INFO("VictorWebServer.Shutdown", "Shutdown on signal %d", signum);
  _running = false;
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
  const char* env_config = getenv("MATE_WEB_SERVER_CONFIG");
  if (env_config != NULL) {
    strncpy(config_file_path, env_config, sizeof(config_file_path));
  }

  Json::Value config;

  printf("0. -------------> config_file: %s\n", config_file_path);
  if (strlen(config_file_path)) {
    std::string config_file{config_file_path};
    if (!Lrya::Util::FileUtils::FileExists(config_file)) {
      fprintf(stderr, "config file not found: %s\n", config_file_path);
    }

    std::string jsonContents = Lrya::Util::FileUtils::ReadFile(config_file);
    //printf("jsonContents: %s", jsonContents.c_str());
    Json::Reader reader;
    if (!reader.parse(jsonContents, config)) {
      PRINT_STREAM_ERROR("deskmateWebServerMain.createPlatform.JsonConfigParseError",
        "json configuration parsing error: " << reader.getFormattedErrorMessages());
    }
  }

  std::string persistentPath;
  std::string cachePath;
  std::string resourcesPath;

  if (config.isMember("DataPlatformPersistentPath")) {
    persistentPath = config["DataPlatformPersistentPath"].asCString();
  } else {
    LOG_ERROR("deskmateWebServerMain.createPlatform.DataPlatformPersistentPathUndefined", "");
  }

  if (config.isMember("DataPlatformCachePath")) {
    cachePath = config["DataPlatformCachePath"].asCString();
  } else {
    LOG_ERROR("deskmateWebServerMain.createPlatform.DataPlatformCachePathUndefined", "");
  }

  if (config.isMember("DataPlatformResourcesPath")) {
    resourcesPath = config["DataPlatformResourcesPath"].asCString();
  } else {
    LOG_ERROR("deskmateWebServerMain.createPlatform.DataPlatformResourcesPathUndefined", "");
  }

  Util::Data::DataPlatform* dataPlatform =
    createPlatform(persistentPath, cachePath, resourcesPath);

  return dataPlatform;
}

int main(void)
{
  signal(SIGTERM, Shutdown);

  InstallCrashReporter(LOG_PROCNAME);

  // - create and set logger
  Util::VictorLogger logger(LOG_PROCNAME);
  Util::gLoggerProvider = &logger;

  Util::Data::DataPlatform* dataPlatform = createPlatform();

  // Create and init deskmateWebServer
  Json::Value wsConfig;
  static const std::string & wsConfigPath = "webserver/webServerConfig_standalone.json";
  const bool success = dataPlatform->readAsJson(Util::Data::Scope::Resources, wsConfigPath, wsConfig);
  if (!success)
  {
    LOG_ERROR("deskmateWebServerMain.WebServerConfigNotFound",
              "Web server config file %s not found or failed to parse",
              wsConfigPath.c_str());
    UninstallCrashReporter();
    Util::gLoggerProvider = nullptr;
    exit(1);
  }

  auto deskmateWebServer = std::make_unique<WebService::WebService>();
  deskmateWebServer->Start(dataPlatform, wsConfig);

  // Wait for shutdown signal
  while (_running) {
    sigset_t mask;
    sigprocmask(SIG_BLOCK, nullptr, &mask);
    sigsuspend(&mask);
  }


  LOG_INFO("deskmateWebServerMain.main", "Shutting down webserver");

  Util::gLoggerProvider = nullptr;
  UninstallCrashReporter();
  sync();
  exit(0);
}
