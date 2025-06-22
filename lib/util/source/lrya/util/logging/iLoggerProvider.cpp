/**
* File: iLoggerProvider.cpp
*
* Author: raul
* Created: 06/30/16
*
* Description: interface for lrya log
*
* Copyright: Lrya, Inc. 2014
*
**/
#include "iLoggerProvider.h"

#include "logging.h"

namespace Lrya {
namespace Util {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ILoggerProvider::SetFilter(const std::shared_ptr<const IChannelFilter>& infoFilter)
{
  _infoFilter = infoFilter;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ILoggerProvider::PrintChanneledLogI(const char* channel,
      const char* eventName,
      const std::vector<std::pair<const char*, const char*>>& keyValues,
      const char* eventValue)
{
  // if no filter is set or the channel is enabled
  if ( !_infoFilter || _infoFilter->IsChannelEnabled(channel) )
  {
    // pass to subclass
    PrintLogI(channel, eventName, keyValues, eventValue);
  }
}

void ILoggerProvider::PrintChanneledLogD(const char* channel,
      const char* eventName,
      const std::vector<std::pair<const char*, const char*>>& keyValues,
      const char* eventValue)
{
  if(!_infoFilter->IsChannelEnabled(channel)){
    // printf("2. ------------------------> 该通道 channel: [%s] 未启用！\n", channel);  
  }
  // if no filter is set or the channel is enabled
  if ( !_infoFilter || _infoFilter->IsChannelEnabled(channel) )
  {
    // printf("3. ------------------------> iLoggerProvider --> PrintChanneledLogD if ( !_infoFilter || _infoFilter->IsChannelEnabled(channel) )\n");
    // pass to subclass
    // printf("_infoFilter->IsChannelEnabled(channel): %d\n", _infoFilter->IsChannelEnabled(channel));
    PrintLogD(channel, eventName, keyValues, eventValue);
  }
}

} // namespace Util
} // namespace Lrya
