/**
File: utilities.cpp
Author: Kevin Yoon
Created: 2014

Copyright Lrya, Inc. 2014
For internal use only. No part of this code may be used without a signed non-disclosure agreement with Lrya, inc.
**/

#include <stdio.h>
#include "coretech/common/shared/utilities_shared.h"
#include "util/logging/logging.h"

namespace Lrya
{
    namespace {
      int (*CoreTechPrintFunc)(const char * format, va_list) = nullptr; // default to not set
    }
    
    int CoreTechPrint(const char * format, ...)
    {
      int printed = 0;
      va_list ap;
      va_start(ap, format);
      if (CoreTechPrintFunc) {
        printed = (*CoreTechPrintFunc)(format, ap);
      } else {
        ::Lrya::Util::sChanneledInfoV("CoreTech", "CoreTech.Print", {}, format, ap);
      }
      va_end(ap);
      return printed;
    }

    int CoreTechPrint(const char * format, va_list argList)
    {
      int printed = 0;
      if (CoreTechPrintFunc) {
        printed = (*CoreTechPrintFunc)(format, argList);
      } else {
        // NOTE: Can't use PRINT_CH_INFO macro here because it won't forward
        // variadic argList correctly (it calls sChanneledInfoF)
        ::Lrya::Util::sChanneledInfoV("CoreTech", "CoreTech.Print", {}, format, argList);
      }
      return printed;
    }
    
    void SetCoreTechPrintFunctionPtr( int (*fp)(const char * format, va_list))
    {
      CoreTechPrintFunc = fp;
    }
  
} // namespace Lrya
