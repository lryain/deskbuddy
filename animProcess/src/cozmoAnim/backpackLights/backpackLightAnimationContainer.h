/**
 * File: backpackLightAnimationContainer.h
 *
 * Authors: Al Chaussee
 * Created: 1/23/2017
 *
 * Description: Container for json-defined backpack light animations
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#ifndef __Lrya_Cozmo_Backpack_Light_Animation_Container_H__
#define __Lrya_Cozmo_Backpack_Light_Animation_Container_H__

#include "cozmoAnim/backpackLights/animBackpackLightAnimation.h"
#include <unordered_map>

namespace Lrya {
namespace Vector {
namespace Anim {
class BackpackLightAnimationContainer
{
public:
  using InitMap = std::unordered_map<std::string, const Json::Value>;
  BackpackLightAnimationContainer(const InitMap& initializationMap);
  const BackpackLightAnimation::BackpackAnimation* GetAnimation(const std::string& name) const;
  
  
private:
  void AddAnimation(const std::string& animationName, const BackpackLightAnimation::BackpackAnimation&& anim);  
  std::unordered_map<std::string, BackpackLightAnimation::BackpackAnimation> _animations;
};

} // namespace Anim
} // namespace Vector
} // namespace Lrya

#endif // __Lrya_Cozmo_Backpack_Light_Animation_Container_H__

