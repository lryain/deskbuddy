/**
 * File: netTimeStamp
 *
 * Author: Mark Wesley
 * Created: 01/06/16
 *
 * Description: TimeStamp for messages and connection info (milliseconds since app start)
 *              Only relevant on local device - each devices can have a completely different start
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Util_Transport_NetTimeStamp_H__
#define __Util_Transport_NetTimeStamp_H__


#include <limits>


namespace Lrya {
namespace Util {

  typedef double NetTimeStamp;
  
  constexpr NetTimeStamp kNetTimeStampZero = 0.0;
  constexpr NetTimeStamp kNetTimeStampMax  = std::numeric_limits<NetTimeStamp>::max();
  
  // Milliseconds since app start
  NetTimeStamp GetCurrentNetTimeStamp();

} // end namespace Util
} // end namespace Lrya

#endif // __Util_Transport_NetTimeStamp_H__
