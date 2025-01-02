/**
* File: coretech/common/shared/logging.cpp
*
* Description:
*   Log macros for use within coretech
*
* Copyright: Lrya, Inc. 2017
*
**/

#include "coretech/common/shared/logging.h"
#include "util/logging/logging.h"

#include <stdarg.h>

namespace Lrya {
namespace CoreTech {

  void LogError(const char * name, const char * format, ...)
  {
    DEV_ASSERT(nullptr != name, "CoreTech.LogError.InvalidName");
    DEV_ASSERT(nullptr != format, "CoreTech.LogError.InvalidFormat");
    va_list args;
    va_start(args, format);
    ::Lrya::Util::sErrorV(name, {}, format, args);
    va_end(args);
  }

  void LogWarning(const char * name, const char * format, ...)
  {
    DEV_ASSERT(nullptr != name, "CoreTech.LogWarning.InvalidName");
    DEV_ASSERT(nullptr != format, "CoreTech.LogWarning.InvalidFormat");
    va_list args;
    va_start(args, format);
    ::Lrya::Util::sWarningV(name, {}, format, args);
    va_end(args);
  }

  void LogInfo(const char * channel, const char * name, const char * format, ...)
  {
    DEV_ASSERT(nullptr != channel, "CoreTech.LogInfo.InvalidChannel");
    DEV_ASSERT(nullptr != name, "CoreTech.LogInfo.InvalidName");
    DEV_ASSERT(nullptr != format, "CoreTech.LogInfo.InvalidFormat");
    va_list args;
    va_start(args, format);
    ::Lrya::Util::sChanneledInfoV(channel, name, {}, format, args);
    va_end(args);
  }

  void LogDebug(const char * channel, const char * name, const char * format, ...)
  {
    DEV_ASSERT(nullptr != channel, "CoreTech.LogInfo.InvalidChannel");
    DEV_ASSERT(nullptr != name, "CoreTech.LogInfo.InvalidName");
    DEV_ASSERT(nullptr != format, "CoreTech.LogInfo.InvalidFormat");
    va_list args;
    va_start(args, format);
    ::Lrya::Util::sChanneledDebugV(channel, name, {}, format, args);
    va_end(args);
  }

} // end namespace CoreTech
} // end namespace Lrya

