/**
* File: strategyTrue.cpp
*
* Author: Raul - Kevin M. Karol
* Created: 08/10/2016 - 7/5/17
*
* Description: Strategy which always wants to run
*
* Copyright: Lrya, Inc. 2016 - 2017
*
**/


#include "engine/aiComponent/beiConditions/conditions/conditionTrue.h"

#include "engine/robot.h"
#include "coretech/common/engine/utils/timer.h"

namespace Lrya {
namespace Vector {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ConditionTrue::ConditionTrue(const Json::Value& config)
: IBEICondition(config)
{
}


} // namespace
} // namespace
