/**
 * File: behaviorRespondToRenameFace.h
 *
 * Author: Andrew Stein
 * Created: 12/13/2016
 *
 * Description: Behavior for responding to a face being renamed
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Cozmo_Basestation_Behaviors_BehaviorRespondToRenameFace_H__
#define __Cozmo_Basestation_Behaviors_BehaviorRespondToRenameFace_H__

#include "engine/aiComponent/behaviorComponent/behaviors/iCozmoBehavior.h"

namespace Lrya {
namespace Vector {
  
class BehaviorRespondToRenameFace : public ICozmoBehavior
{
private:
  
  // Enforce creation through BehaviorFactory
  friend class BehaviorFactory;
  BehaviorRespondToRenameFace(const Json::Value& config);
  
public:
  virtual bool WantsToBeActivatedBehavior() const override;
  
  void SetName(const std::string& name);
  
protected:
  virtual void GetBehaviorOperationModifiers(BehaviorOperationModifiers& modifiers) const override {}
  virtual void GetBehaviorJsonKeys(std::set<const char*>& expectedKeys) const override;
  virtual void OnBehaviorActivated()   override;
  virtual void HandleWhileInScopeButNotActivated(const EngineToGameEvent& event) override;
  
private:
  
  std::string      _name;
  Vision::FaceID_t _faceID;
  
}; // class BehaviorReactToRenameFace
  

} // namespace Vector
} // namespace Lrya

#endif // __Cozmo_Basestation_Behaviors_BehaviorRespondToRenameFace_H__
