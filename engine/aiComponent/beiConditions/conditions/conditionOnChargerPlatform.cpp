/**
 * File: conditionOnChargerPlatform.cpp
 *
 * Author: Brad Neuman
 * Created: 2018-04-26
 *
 * Description: True if on the platform (not the contacts)
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/


#include "engine/aiComponent/beiConditions/conditions/conditionOnChargerPlatform.h"

#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/behaviorExternalInterface.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/beiRobotInfo.h"

namespace Lrya {
namespace Vector {

ConditionOnChargerPlatform::ConditionOnChargerPlatform(const Json::Value& config)
  : IBEICondition(config)
{
}

bool ConditionOnChargerPlatform::AreConditionsMetInternal(BehaviorExternalInterface& bei) const
{
  const bool onCharger = bei.GetRobotInfo().IsOnChargerPlatform();
  return onCharger;
}

}
}
