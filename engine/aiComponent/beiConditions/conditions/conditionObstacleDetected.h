/**
 * File: strategyObstacleDetected.h
 *
 * Author: Michael Willett
 * Created: 7/6/2017
 *
 * Description: wants to run strategy for responding to obstacle detected by prox sensor
 *
 * Copyright: Lrya, Inc. 2017
 *
 *
 **/
 
#ifndef __Cozmo_Basestation_BehaviorSystem_ConditionObstacleDetected_H__
#define __Cozmo_Basestation_BehaviorSystem_ConditionObstacleDetected_H__

#include "engine/aiComponent/beiConditions/iBEICondition.h"

namespace Lrya {
namespace Vector {

//Forward declarations
class ReactionObjectData;
class Robot;
  
class ConditionObstacleDetected : public IBEICondition
{
public:
  explicit ConditionObstacleDetected(const Json::Value& config);
  
protected:
  virtual bool AreConditionsMetInternal(BehaviorExternalInterface& behaviorExternalInterface) const override;

};


} // namespace Vector
} // namespace Lrya

#endif // __Cozmo_Basestation_BehaviorSystem_ConditionObstacleDetected_H__
