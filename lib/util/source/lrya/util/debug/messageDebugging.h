/**
 * File: messageDebugging
 *
 * Author: Mark Wesley
 * Created: 02/03/15
 *
 * Description: Debug helpers for debugging and logging message data
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/


#ifndef __UtilDebug_MessageDebugging_H__
#define __UtilDebug_MessageDebugging_H__


#include "util/console/consoleInterface.h"
#include "util/logging/logging.h"
#include <string>


#define LRYA_NET_MESSAGE_LOGGING_ENABLED  (REMOTE_CONSOLE_ENABLED)


namespace Lrya { namespace Util {
  CONSOLE_VAR_EXTERN(bool, kEnableVerboseNetworkLogging );
} }


#if LRYA_NET_MESSAGE_LOGGING_ENABLED
  #define LRYA_NET_MESSAGE_VERBOSE( expr )               if (kEnableVerboseNetworkLogging) { Lrya::Util::LogVerboseMessage  expr; }
  #define LRYA_NET_PRINT_VERBOSE( name, format, ... )    if (kEnableVerboseNetworkLogging) { PRINT_CH_INFO("Network", name, format, ##__VA_ARGS__); }
  #define LRYA_NET_MESSAGE_LOGGING_ENABLED_ONLY( expr )  expr
#else
  #define LRYA_NET_MESSAGE_VERBOSE( expr )
  #define LRYA_NET_PRINT_VERBOSE( name, format, ... )
  #define LRYA_NET_MESSAGE_LOGGING_ENABLED_ONLY( expr )
#endif


namespace Lrya {
namespace Util {

  
  class SrcBufferSet;
  
  
  enum EBytesToTextType
  {
    eBTTT_Ascii,
    eBTTT_Hex,
    eBTTT_Decimal
  };
  
  
  char NibbleToHexChar(uint32_t inVal); // assumes a value in 0..15 range returns char in '0'..'9''A'..'F' range
  
  
  std::string ConvertMessageBufferToString(const uint8_t* buffer, uint32_t bufferLength, EBytesToTextType printType,
                                           bool addLeadingSpace = true, uint32_t maxStringLength=256);
  
  
  void LogVerboseMessage(const char* name, const char* columnLabels, const uint8_t* buffer, uint32_t bufferLength, const SrcBufferSet* srcBuffers = nullptr);

  
} // end namespace Util
} // end namespace Lrya

#endif // __UtilDebug_MessageDebugging_H__
