/**
 * File: iFistBumpListener.h
 *
 * Author: Kevin Yoon
 * Created: 02/14/17
 *
 * Description: Interface for receiving notifications from BehaviorFistBump
 *
 * Copyright: Lrya, Inc. 2016
 **/

#ifndef __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IFistBumpListener_H__
#define __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IFistBumpListener_H__


namespace Lrya {
namespace Vector {

class IFistBumpListener{
public:
  virtual void ResetTrigger(bool updateLastCompletionTime) = 0;
};


} // namespace Vector
} // namespace Lrya

#endif // __Cozmo_Basestation_BehaviorSystem_BehaviorListenerInterfaces_IFistBumpListener_H__

