/**
 * File: conditionRobotPickedUp.cpp
 *
 * Author: Guillermo Bautista
 * Created: 2018/09/05
 *
 * Description: Condition that indicates when Vector has been picked up.
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "engine/aiComponent/beiConditions/conditions/conditionRobotPickedUp.h"

#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/behaviorExternalInterface.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/beiRobotInfo.h"

namespace Lrya {
namespace Vector {

ConditionRobotPickedUp::ConditionRobotPickedUp(const Json::Value& config)
: IBEICondition(config) 
{
}

bool ConditionRobotPickedUp::AreConditionsMetInternal(BehaviorExternalInterface& behaviorExternalInterface) const
{
  auto& robotInfo = behaviorExternalInterface.GetRobotInfo();
  return robotInfo.IsPickedUp();
}


} // namespace Vector
} // namespace Lrya
