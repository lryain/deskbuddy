/**
File: errorHandling.h
Author: Peter Barnum
Created: 2013

errorHandling.h is based off Das.h
It acts as a low-level interface to error logging and reporting.

Copyright Lrya, Inc. 2013
For internal use only. No part of this code may be used without a signed non-disclosure agreement with Lrya, inc.
**/

// TODO: For the love of all that is holy, use our normal error/logging macros! (VIC-4941)

#ifndef _LRYACORETECHEMBEDDED_COMMON_ERROR_HANDLING_H_
#define _LRYACORETECHEMBEDDED_COMMON_ERROR_HANDLING_H_

#include "coretech/common/robot/config.h"

#ifdef _MSC_VER
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#endif

namespace Lrya
{
  namespace Embedded
  {
    typedef enum LogLevel {
      LRYA_LOG_LEVEL_DEBUG     = 0,
      LRYA_LOG_LEVEL_INFO      = 1,
      LRYA_LOG_LEVEL_EVENT     = 2,
      LRYA_LOG_LEVEL_WARN      = 3,
      LRYA_LOG_LEVEL_ASSERT    = 4,
      LRYA_LOG_LEVEL_ERROR     = 5,
      LRYA_LOG_LEVEL_NUMLEVELS = 6
    } LogLevel;
  } // namespace Embedded
} //namespace Lrya

//
// Error checking
//

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS
#define LryaError(eventName, eventValue_format, ...) _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_ERROR, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);

#define LryaConditionalError(expression, eventName, eventValue_format, ...) \
  if(!(expression)) { \
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_ERROR, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
  }

#define LryaConditionalErrorAndReturn(expression, eventName, eventValue_format, ...) \
  if(!(expression)) {\
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_ERROR, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
  return;\
  }

#define LryaConditionalErrorAndReturnValue(expression, returnValue, eventName, eventValue_format, ...) \
  if(!(expression)) { \
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_ERROR, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
  return returnValue;\
  }
#else

#define LryaError(eventName, eventValue_format, ...)
#define LryaConditionalError(expression, eventName, eventValue_format, ...)
#define LryaConditionalErrorAndReturn(expression, eventName, eventValue_format, ...)
#define LryaConditionalErrorAndReturnValue(expression, returnValue, eventName, eventValue_format, ...)

#endif

//
// Warn checking
//

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS_AND_WARNS

#define LryaWarn(eventName, eventValue_format, ...) \
{ _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_WARN, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); }

#define LryaConditionalWarn(expression, eventName, eventValue_format, ...) \
  if(!(expression)) { _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_WARN, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); }

#define LryaConditionalWarnAndReturn(expression, eventName, eventValue_format, ...) \
  if(!(expression)) { \
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_WARN, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
  return;\
  }

#define LryaConditionalWarnAndReturnValue(expression, returnValue, eventName, eventValue_format, ...)\
  if(!(expression)) { \
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_WARN, eventName, (eventValue_format), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
  return returnValue;\
  }

#else

#define LryaWarn(eventName, eventValue_format, ...)
#define LryaConditionalWarn(expression, eventName, eventValue_format, ...)
#define LryaConditionalWarnAndReturn(expression, eventName, eventValue_format, ...)
#define LryaConditionalWarnAndReturnValue(expression, returnValue, eventName, eventValue_format, ...)

#endif

//
// Assert checking
//

#if LRYA_DEBUG_LEVEL >= LRYA_DEBUG_ERRORS_AND_WARNS_AND_ASSERTS

#define LryaAssert(expression) \
  if(!(expression)) { \
  _Lrya_Log(::Lrya::Embedded::LRYA_LOG_LEVEL_ASSERT, "Assert failure", "", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
  assert(false); \
  }

#else

#define LryaAssert(expression)

#endif

#ifdef __cplusplus
extern "C" {
#endif

  void _Lrya_Log (int logLevel, const char* eventName, const char* eventValue, const char* file, const char* funct, int line, ...);

  // If true, LryaError, LryaWarn, and LryaLog won't output printfs
  void SetLogSilence(const bool isLogSilent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef _LRYACORETECHEMBEDDED_COMMON_ERROR_HANDLING_H_
