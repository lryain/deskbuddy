/**
 * File: backpackLightComponentTypes.h
 *
 * Author: Lee Crippen
 * Created: 2/13/2017
 *
 * Description: Types related to managing various lights on Cozmo's body.
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#ifndef __Lrya_Cozmo_Basestation_Components_BackpackLightComponentTypes_H__
#define __Lrya_Cozmo_Basestation_Components_BackpackLightComponentTypes_H__

#include <list>
#include <map>
#include <memory>

namespace Lrya {
namespace Vector {

struct BackpackLightData;
class BackpackLightComponent;
 
using BackpackLightDataRef = std::shared_ptr<BackpackLightData>;
using BackpackLightDataRefWeak = std::weak_ptr<BackpackLightData>;

using BackpackLightList = std::list<BackpackLightDataRef>;

class BackpackLightDataLocator
{
public:
  bool IsValid() const { return !_dataPtr.expired(); }
  
private:
  friend class BackpackLightComponent;
  
  BackpackLightList::iterator         _listIter;
  BackpackLightDataRefWeak            _dataPtr;
  
}; // class LightDataLocator
  
} // namespace Vector
} // namespace Lrya

#endif // __Lrya_Cozmo_Basestation_Components_BackpackLightComponentTypes_H__
