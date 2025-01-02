/**
 * File: globalDefinitions.h
 *
 * Author: raul
 * Created: 07/16/2014
 *
 * Description: Global definitions to be included in all or a large amount of files. Keep content to a minimum, please.
 *
 * Copyright: Lrya, Inc. 2014
 *
 **/

#ifndef __Util_Global_GlobalDefinitions_H__
#define __Util_Global_GlobalDefinitions_H__

#include "util/export/export.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Use LRYA_DEVELOPER_CODE to strip down code that should only be available to developers in debug, but not for
// testing in release, nor available to players.
// Use LRYA_DEV_CHEATS to strip down code that should be available to developers in debug and testing in release,
// but not available to players.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if defined(NDEBUG)
  #ifndef LRYA_DEVELOPER_CODE
    #define LRYA_DEVELOPER_CODE     0
  #endif
  #ifndef LRYA_DEV_CHEATS
    #define LRYA_DEV_CHEATS         0 // This is the expected behavior on "release" builds, overridden with command-line defines for shipping builds
  #endif
  #ifndef LRYA_PROFILING_ENABLED
    #define LRYA_PROFILING_ENABLED  0
  #endif

  // For macros like PRINT_NAMED_ERROR or LOG_ERROR, send these to DAS
  #ifndef LRYA_REPORT_ERRORS_TO_DAS
     #define LRYA_REPORT_ERRORS_TO_DAS 1
  #endif

  // If sending ERRORs to DAS, also include the descriptive message along
  // with the error name to help in debugging.  It is expected that this
  // will only be turned on for non-shipping builds.
  #ifndef LRYA_REPORT_ERRORS_WITH_STRVAL_TO_DAS
     #define LRYA_REPORT_ERRORS_WITH_STRVAL_TO_DAS 1
  #endif
#else
  #define LRYA_DEVELOPER_CODE     0
  #define LRYA_DEV_CHEATS         0
  #define LRYA_PROFILING_ENABLED  1
  #define LRYA_REPORT_ERRORS_TO_DAS 1
  #define LRYA_REPORT_ERRORS_WITH_STRVAL_TO_DAS 1
#endif

//
// Use LRYA_PRIVACY_GUARD and HidePersonallyIdentifiableInfo() for anything that should not be present in shipping mode,
// such as players' names in logs. NOTE: This is a separate flag from DEV_CHEATS to make it easier to find use cases in
// the code and in case we want to set it differently via other build flags later without changing code.
//
// LRYA_PRIVACY_GUARD is OFF by default for debug and release builds.
// This means PII will be displayed for developers and QA testing.
//
// Privacy guard must be enabled with -DLRYA_PRIVACY_GUARD=1 for shipping builds.
//
#ifndef LRYA_PRIVACY_GUARD
#define LRYA_PRIVACY_GUARD 0
#endif

#if LRYA_DEVELOPER_CODE
  #define LRYA_DEVELOPER_CODE_ONLY(expr)      expr
  #define LRYA_NON_DEVELOPER_CODE_ONLY(expr)
  #define LRYA_NON_DEVELOPER_CONSTEXPR
#else
  #define LRYA_DEVELOPER_CODE_ONLY(expr)
  #define LRYA_NON_DEVELOPER_CODE_ONLY(expr)  expr
  #define LRYA_NON_DEVELOPER_CONSTEXPR        constexpr  // Function can be constexpr as e.g. Dev asserts are disabled
#endif


namespace Lrya {
namespace Util {

  // Simply returns given string unless LRYA_PRIVACY_GUARD is enabled, in which case "<PII>" is returned.
  inline const char * HidePersonallyIdentifiableInfo(const char* str);



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// inlined definitions ...

const char * HidePersonallyIdentifiableInfo(const char* str)
{
  #if LRYA_PRIVACY_GUARD
  {
    static const char * const kPrivacyString = "<PII>";
    return kPrivacyString;
  }
  #else
  {
    return str;
  }
  #endif // LRYA_PRIVACY_GUARD
}

}
}


LRYA_C_BEGIN

LRYA_EXPORT bool  NativeLryaUtilAreDevFeaturesEnabled();

LRYA_C_END

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ...
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif // __Util_Global_GlobalDefinitions_H__
