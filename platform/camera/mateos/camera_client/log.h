/**
 * File: log.h
 *
 * Author: seichert
 * Created: 1/10/2018
 *
 * Description: log functions for Lrya Bluetooth Daemon
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#ifndef __mm_lrya_camera_log_h__
#define __mm_lrya_camera_log_h__

#include <stdarg.h>
#include <stdbool.h>

enum lrya_camera_log_level {
    LryaCameraLogLevelVerbose = 2,
    LryaCameraLogLevelDebug   = 3,
    LryaCameraLogLevelInfo    = 4,
    LryaCameraLogLevelWarn    = 5,
    LryaCameraLogLevelError   = 6,
    LryaCameraLogLevelSilent  = 7,
    LryaCameraLogLevelMax     = LryaCameraLogLevelSilent,
};

bool isUsingAndroidLogging();
void enableAndroidLogging(const bool enable);
void setAndroidLoggingTag(const char* tag);
int getMinLogLevel();
void setMinLogLevel(const int level);

void logv(const char* fmt, ...) __attribute__((format(printf,1,2)));
void logd(const char* fmt, ...) __attribute__((format(printf,1,2)));
void logi(const char* fmt, ...) __attribute__((format(printf,1,2)));
void logw(const char* fmt, ...) __attribute__((format(printf,1,2)));
void loge(const char* fmt, ...) __attribute__((format(printf,1,2)));

#endif //__mm_lrya_camera_log_h__


