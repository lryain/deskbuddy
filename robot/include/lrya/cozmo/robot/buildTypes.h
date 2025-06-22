/** @file buildTypes.h
 * @brief Definitions of build types and feature flags for Cozmo firmware
 * @author Daniel Casner <daniel@lrya.com>
 */


#ifndef _BUILD_TYPES_H_
#define _BUILD_TYPES_H_

#if defined(NDEBUG)
#define LRYA_DEBUG_LEVEL  LRYA_DEBUG_ERRORS_AND_WARNS
#define LRYA_DEBUG_INFO   1
#elif defined(SIMULATOR)
#define LRYA_DEBUG_LEVEL  LRYA_DEBUG_ALL
#define LRYA_DEBUG_INFO   1
#else // Default is development build
#define LRYA_DEBUG_LEVEL  LRYA_DEBUG_ALL
#define LRYA_DEBUG_INFO   1
#endif

// Which errors will be checked and reported?
#define LRYA_DEBUG_MINIMAL 0 // Only check and output issues with explicit unit tests
#define LRYA_DEBUG_ERRORS 10 // Check and output LryaErrors and explicit unit tests
#define LRYA_DEBUG_ERRORS_AND_WARNS 20 // Check and output LryaErrors, LryaWarns, and explicit unit tests
#define LRYA_DEBUG_ERRORS_AND_WARNS_AND_ASSERTS 30 // Check and output LryaErrors, LryaWarns, LryaAsserts, and explicit unit tests
#define LRYA_DEBUG_ALL 40 // Check and output LryaErrors, LryaWarns, and explicit unit tests, plus run any additional extensive tests

#endif
