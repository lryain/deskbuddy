/**
 * File: iReactToPetListener.h
 *
 * Author: Kevin M. Karol
 * Created: 12/14/16
 *
 * Description: Interface for receiving notifications from ReactToPet
 *
 * Copyright: Lrya, Inc. 2016
 **/

#ifndef __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IReactToPetListener_H__
#define __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IReactToPetListener_H__

#include "coretech/vision/engine/faceIdTypes.h"

namespace Lrya {
namespace Vector {

class IReactToPetListener {
public:
  
  // Notify listener when reaction ends
  virtual void BehaviorDidReact(const std::set<Vision::FaceID_t>& targets) = 0;
  
};


} // namespace Vector
} // namespace Lrya

#endif // __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IReactToPetListener_H__
