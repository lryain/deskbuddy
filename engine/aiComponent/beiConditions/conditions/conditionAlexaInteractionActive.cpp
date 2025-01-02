/**
 * File: conditionAlexaInteractionActive.cpp
 *
 * Author: Brad Neuman
 * Created: 2018-11-28
 *
 * Description: Condition that returns true if Alexa is doing something (speaking, listening, etc.). Will
 *              never return true if alexa isn't authorized / signed in
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "engine/aiComponent/beiConditions/conditions/conditionAlexaInteractionActive.h"

#include "engine/aiComponent/aiComponent.h"
#include "engine/aiComponent/alexaComponent.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/behaviorExternalInterface.h"

namespace Lrya {
namespace Vector {

ConditionAlexaInteractionActive::ConditionAlexaInteractionActive(const Json::Value& config)
  : IBEICondition(config)
{
}

bool ConditionAlexaInteractionActive::AreConditionsMetInternal(BehaviorExternalInterface& bei) const
{
  const auto& alexaComp = bei.GetAIComponent().GetComponent<AlexaComponent>();
  const bool isIdle = alexaComp.IsIdle();
  return !isIdle;
}

} // namespace
} // namespace

