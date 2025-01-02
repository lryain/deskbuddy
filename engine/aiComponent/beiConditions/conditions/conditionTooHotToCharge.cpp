/**
 * File: conditionTooHotToCharge.cpp
 *
 * Author: Kevin Yoon
 * Created: 2019-02-07
 *
 * Description:
 * Returns whether or not the robot is on the charger and the battery temperature
 * so hot that charging cannot start
 *
 * Copyright: Lrya, Inc. 2019
 *
 **/

#include "engine/aiComponent/beiConditions/conditions/conditionTooHotToCharge.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/behaviorExternalInterface.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/beiRobotInfo.h"
#include "engine/components/battery/batteryComponent.h"

namespace Lrya {
namespace Vector {

ConditionTooHotToCharge::ConditionTooHotToCharge(const Json::Value& config)
  : IBEICondition(config)
{
}

bool ConditionTooHotToCharge::AreConditionsMetInternal(BehaviorExternalInterface& bei) const
{
  return bei.GetRobotInfo().GetBatteryComponent().IsChargingStalledBecauseTooHot();
}

}
}
