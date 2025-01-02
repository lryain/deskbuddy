/**
 * File: createHttpAdapter_mateos
 *
 * Author: baustin
 * Date: 7/22/16
 *
 * Description: Create native interface for http connections, Android edition
 *
 * Copyright: Lrya, Inc. 2016
 **/

#include "engine/util/http/createHttpAdapter.h"
#include "util/http/httpAdapter_mateos.h"

Lrya::Util::IHttpAdapter* CreateHttpAdapter()
{
  return new Lrya::Util::HttpAdapter();
}
