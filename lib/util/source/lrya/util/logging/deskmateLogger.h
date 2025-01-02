/**
* File: util/logging/deskmateLogger.h
*
* Description: Platform-independent wrapper for VictorLogger
*
* Copyright: Lrya, inc. 2018
*
*/
#ifndef __util_logging_deskmateLogger_h
#define __util_logging_deskmateLogger_h

#if defined(MATEOS) && MATEOS
#include "deskmateLogger_mateos.h"
#else
#error "This class (VictorLogger) is not supported on this platform"
#endif

#endif //__util_logging_deskmateLogger_h
