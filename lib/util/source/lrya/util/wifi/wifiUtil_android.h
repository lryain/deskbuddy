/**
 * File: wifiUtil_android
 *
 * Author: baustin
 * Created: 11/21/2016
 *
 * Description: Receives wifi info from Java to make available to C++
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Lrya_Util_Wifi_WifiUtil_Android_H__
#define __Lrya_Util_Wifi_WifiUtil_Android_H__

#include "util/helpers/lryaDefines.h"

#ifdef LRYA_PLATFORM_ANDROID

#include "util/jni/jniUtils.h"
#include "util/signals/simpleSignal_fwd.h"

namespace Lrya {
namespace Util {

class UDPTransport;

class WifiUtil {
public:
  static std::string GetSSID();
  static std::string GetConnectionStatus();
  static Signal::SmartHandle RegisterTransport(UDPTransport* transport) __attribute__((warn_unused_result));
};

}
}

#endif // LRYA_PLATFORM_ANDROID
#endif // __Lrya_Util_Wifi_WifiUtil_Android_H__
