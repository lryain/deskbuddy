/**
 * File: webVizSender.cpp
 *
 * Author: Brad Neuman
 * Created: 2018-08-11
 *
 * Description: RAII style helper to send json to webviz
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "webServerProcess/src/webVizSender.h"

#include "util/logging/logging.h"
#include "webServerProcess/src/webService.h"

#if !defined(LRYA_NO_WEBSERVER_ENABLED)
  #define LRYA_NO_WEBSERVER_ENABLED 0
#endif

namespace Lrya {
namespace Vector {
namespace WebService {

WebVizSender::WebVizSender(const std::string& moduleName, WebService* webService)
  : _module(moduleName)
  , _webService(webService)
{
  DEV_ASSERT(_webService != nullptr, "WebVizSender.Ctor.NullWebService");
}

WebVizSender::~WebVizSender()
{
#if !LRYA_NO_WEBSERVER_ENABLED
  DEV_ASSERT(_webService != nullptr, "WebVizSender.Dtor.NullWebService");
  if( !_data.empty() ) {
    _webService->SendToWebViz(_module, _data);
  }
#endif
}

std::shared_ptr<WebVizSender> WebVizSender::CreateWebVizSender(const std::string& moduleName, WebService* webService)
{

#if !LRYA_NO_WEBSERVER_ENABLED
  if( webService &&
      webService->IsWebVizClientSubscribed(moduleName) ) {
    return std::make_shared<WebVizSender>(moduleName, webService);
  }
#endif

  return {};
}


}
}
}
