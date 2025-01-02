/*
 * File:          diagnosticDefines.h
 * Date:          9/25/2018
 * Author:        Stuart Eichert
 *
 * Description:   Decide if we are going to track these diagnostics
 *
 */

#ifndef LRYA_PLATFORM_COMMON_DIAGNOSTIC_DEFINES_H
#define LRYA_PLATFORM_COMMON_DIAGNOSTIC_DEFINES_H


#include "util/global/globalDefinitions.h"
#if LRYA_PROFILING_ENABLED && !defined(SIMULATOR) && defined(NDEBUG)
  #define ENABLE_TICK_TIME_WARNINGS 1
#else
  #define ENABLE_TICK_TIME_WARNINGS 0
#endif

#endif // LRYA_PLATFORM_COMMON_DIAGNOSTIC_DEFINES_H
