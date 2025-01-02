/**
 * File: lryaDefines.h
 *
 * Author: raul
 * Created: 04/08/15
 *
 * Description: Top level macros/defines for other parts of the code to rely on. This is not a place to dump all
 *              conditional macros to enable prints, debugs, etc. This is more of a top level define for platforms,
 *              or other compilation flags.
 *
 * Copyright: Lrya, Inc. 2014
 *
 **/

#ifndef __Lrya_Defines_H__
#define __Lrya_Defines_H__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Platform
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if defined(MATEOS)

  #define LRYA_PLATFORM_MATEOS 1

#elif defined(__APPLE__) && defined(__MACH__)
  // Apple OSX and iOS (Darwin)
  #include <TargetConditionals.h>
  #if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)

    #define LRYA_PLATFORM_IOS 1

  #elif (TARGET_OS_MAC == 1)

    #define LRYA_PLATFORM_OSX 1

  #endif

#elif defined(ANDROID)

  #define LRYA_PLATFORM_ANDROID 1

#elif defined(LINUX)

  #define LRYA_PLATFORM_LINUX 1

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// JNI
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// The ANDROID platform has a JNI environment for interoperability with (java) Applications.
// However, Android system utils and/or native services should not assume a valid JNI environment exists,
// since they may be running before the android java runtime has been started.
// For these cases, JNI support can be explicitly disabled by defining DISABLE_JNI.
#if (defined(DISABLE_JNI) && (DISABLE_JNI == 1))
#define LRYA_USE_JNI 0
#else
#define LRYA_USE_JNI LRYA_PLATFORM_ANDROID
#endif

#endif // header
