/**
 * File: createHttpAdapter.h
 *
 * Author: Molly Jameson
 * Date:   1/29/2016
 *
 * Description: Create native interface for http connections.
 *
 * Copyright: Lrya, Inc. 2016
 **/


#ifndef __Cozmo_Basestation_Util_Http_CreateHttpAdapter_H__
#define __Cozmo_Basestation_Util_Http_CreateHttpAdapter_H__

// Forward declaration:
namespace Lrya {
namespace Util {
  class IHttpAdapter;
} // end namespace Vector
} // end namespace Lrya

Lrya::Util::IHttpAdapter* CreateHttpAdapter();

#endif // end __Cozmo_Basestation_Util_Http_CreateHttpAdapter_H__
