/** Embedded logging facilities for Cozmo
 * @author Daniel Casner <daniel@lrya.com>
 * @copyright Lrya Inc. 2015
 * For internal use only. No part of this code may be used without a signed non-disclosure agreement with Lrya, inc.
 * File created Dec 2015
 *
 * API is intended to be similar to previous errorHandling and DAS libraries but with low overhead appropriate to
 * Cozmo's real time and network processors.
 *
 * Implementations of SendLog will vary for the different processors:
 * On the WiFi processor, SendLog will accept a level and a format string and variable arguments for payload
 * On the Real time processor, SendLog will accept a level and a RtipTrace enum from robotLogging.clad along with the
 * variable arguments for payload.
 */

#ifndef __LRYA_COZMO_ROBOT_LOGGING_H_
#define __LRYA_COZMO_ROBOT_LOGGING_H_

#include "lrya/cozmo/robot/buildTypes.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef COZMO_ROBOT
#error "This logging file may only be included in mate-robot"
#endif

#if defined(MATEOS)

#include <android/log.h>
#define console_printf(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "mate-robot", fmt, ##__VA_ARGS__)
#define log_assert(line, file, expr, fmt, ...) __android_log_assert(expr, "mate-robot", fmt ": failed at line " line " in file " file, ##__VA_ARGS__)
#define log_error(name, fmt, ...) __android_log_print(ANDROID_LOG_ERROR, "mate-robot", name ": " fmt, ##__VA_ARGS__)
#define log_warn(name, fmt, ...) __android_log_print(ANDROID_LOG_WARN, "mate-robot", name ": " fmt, ##__VA_ARGS__)
#define log_info(name, fmt, ...) __android_log_print(ANDROID_LOG_INFO, "mate-robot", name ": " fmt, ##__VA_ARGS__)
#define log_debug(name, fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "mate-robot", name ": " fmt, ##__VA_ARGS__)

#else

#define console_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define log_assert(line, file, expr, fmt, ...) console_printf("[Assert] " fmt ": \"" expr "\" failed at line " line " in file " file "\r\n", ##__VA_ARGS__)
#define log_error(name, fmt, ...) console_printf("[Error] " name ": " fmt "\r\n", ##__VA_ARGS__)
#define log_warn(name, fmt, ...) console_printf("[Warn] " name ": " fmt "\r\n", ##__VA_ARGS__)
#define log_info(name, fmt, ...) console_printf("[Info] " name ": " fmt "\r\n", ##__VA_ARGS__)
#define log_debug(name, fmt, ...) console_printf("[Debug] " name ": " fmt "\r\n", ##__VA_ARGS__)

#endif

// Keil doesn't seem to reliably error on these not being defined below so trigger explicitly.
#ifndef LRYA_DEBUG_INFO
#error LRYA_DEBUG_INFO not defined
#endif
#ifndef LRYA_DEBUG_LEVEL
#error LRYA_DEBUG_LEVEL not defined
#endif

namespace Lrya {
  namespace Vector {
    namespace RobotInterface {

#if LRYA_DEBUG_INFO
      #define LryaInfo(nameString, fmtString, ...) \
      { \
        log_info(nameString, fmtString, ##__VA_ARGS__); \
      }
      
      #define LryaInfoPeriodic(num_calls_between_prints, nameString, fmtString, ...) \
      {   static u16 cnt = num_calls_between_prints; \
          if (++cnt > num_calls_between_prints) { \
            log_info(nameString, fmtString, ##__VA_ARGS__); \
            cnt = 0; \
          } \
      }
#else
      #define LryaInfo(...)
#endif

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ALL
      #define LryaDebug(nameString, fmtString, ...) \
      { \
        log_debug(nameString, fmtString, ##__VA_ARGS__); \
      }

      #define LryaDebugPeriodic(num_calls_between_prints, nameString, fmtString, ...) \
      {   static u16 cnt = num_calls_between_prints; \
          if (++cnt > num_calls_between_prints) { \
            log_debug(nameString, fmtString, ##__VA_ARGS__); \
            cnt = 0; \
          } \
      }
#else
      #define LryaDebug(...)
      #define LryaDebugPeriodic(...)
#endif

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS
      #define LryaError(nameString, fmtString, ...) { \
        log_error(nameString, fmtString, ##__VA_ARGS__); \
      }

      #define LryaConditionalError(expression, nameString, fmtString, ...) \
        if (!(expression)) { \
          log_error(nameString, fmtString, ##__VA_ARGS__); \
        }

      #define LryaConditionalErrorAndReturn(expression, nameString, fmtString, ...) \
        if (!(expression)) { \
          log_error(nameString, fmtString, ##__VA_ARGS__); \
          return; \
        }

      #define LryaConditionalErrorAndReturnValue(expression, returnValue, nameString, fmtString, ...) \
        if(!(expression)) { \
          log_error(nameString, fmtString, ##__VA_ARGS__); \
          return returnValue; \
        }
#else
      #define LryaError(...)
      #define LryaConditionalError (...)
      #define LryaConditionalErrorAndReturn (expression, ...) if (!(expression)) return
      #define LryaConditionalErrorAndReturnValue(expression, returnValue, ...) if (!(expression)) return returnValue
#endif

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS_AND_WARNS
      #define LryaWarn(nameString, fmtString, ...) { \
        log_warn(nameString, fmtString, ##__VA_ARGS__); \
      }

      #define LryaConditionalWarn(expression, nameString, fmtString, ...) \
        if (!(expression)) { \
          log_warn(nameString, fmtString, ##__VA_ARGS__); \
        }

      #define LryaConditionalWarnAndReturn(expression, nameString, fmtString, ...) \
        if (!(expression)) { \
          log_warn(nameString, fmtString, ##__VA_ARGS__); \
          return; \
        }

      #define LryaConditionalWarnAndReturnValue(expression, returnValue, nameString, fmtString, ...) \
        if(!(expression)) { \
          log_warn(nameString, fmtString, ##__VA_ARGS__); \
          return returnValue;\
        }
#else
      #define LryaWarn(...)
      #define LryaConditionalWarn(...)
      #define LryaConditionalWarnAndReturn(expression, ...) if (!(expression)) return
      #define LryaConditionalWarnAndReturnValue(expression, returnValue, ...) if (!(expression)) return returnValue
#endif

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS_AND_WARNS_AND_ASSERTS
      // Extra level of expansion required for proper stringification of __LINE__
      #define _STRINGIZE(x) #x
      #define STRINGIZE(x) _STRINGIZE(x)
      // Lrya assert sends assert CLAD message and then halts main exec
      #define LryaAssert(expression, fmtString, ...) \
        if (!(expression)) { \
          log_assert(STRINGIZE(__LINE__), __FILE__, #expression, fmtString, ##__VA_ARGS__); \
          exit(-1); \
        }
#else
      #define LryaAssert(...)
#endif

    }
  }
}

#endif
