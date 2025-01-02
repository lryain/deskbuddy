/**
 * File: animTrackHelpers.h
 *
 * Author: Lee Crippen
 * Created: 1/13/2016
 *
 * Description: Helpful funcitonality for dealing with animation tracks.
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Lrya_Cozmo_Basestation_Components_AnimTrackHelpers_H__
#define __Lrya_Cozmo_Basestation_Components_AnimTrackHelpers_H__

#include <string>

namespace Lrya {
namespace Vector {
  
enum class AnimTrackFlag : uint8_t;

class AnimTrackHelpers
{
public:
  // Turns animation track flags into a space separated string for easy debugging
  static std::string AnimTrackFlagsToString(uint8_t tracks);
  
};

} // namespace Vector
} // namespace Lrya

#endif // __Lrya_Cozmo_Basestation_Components_AnimTrackHelpers_H__
