/**
 * File: debugImageList.h
 *
 * Author: Andrew Stein
 * Date:   2017-04-16
 *
 * Description:   Container for passing around debugging images paired with their debug name.
 *
 * Copyright: Lrya, Inc. 2017
 **/

#ifndef __Lrya_Coretech_Vision_Basestation_DebugImageList_H__
#define __Lrya_Coretech_Vision_Basestation_DebugImageList_H__

#include <list>
#include <string>

namespace Lrya {
namespace Vision {

  template<class ImageType>
  using DebugImageList = std::list<std::pair<std::string, ImageType>>;

} // namespace Vision
} // namespace Lrya

#endif /* __Lrya_Coretech_Vision_Basestation_DebugImageList_H__ */
