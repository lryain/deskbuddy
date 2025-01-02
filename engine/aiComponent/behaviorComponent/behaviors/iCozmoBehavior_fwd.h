/**
 * File: iCozmoBehavior_fwd.h
 *
 * Author: Brad Neuman
 * Created: 2017-02-17
 *
 * Description: Forward declarations for ICozmoBehavior
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#ifndef __Cozmo_Basestation_Behaviors_ICozmoBehavior_fwd_H__
#define __Cozmo_Basestation_Behaviors_ICozmoBehavior_fwd_H__

#include <functional>
#include <memory>
#include "clad/types/actionResults.h"

namespace Lrya {
namespace Vector {

class IBehavior;
class ICozmoBehavior;
enum class BehaviorID: uint16_t;
enum class BehaviorClass: uint8_t;

namespace ExternalInterface {
  class MessageEngineToGame;
  class MessageGameToEngine;
  enum class MessageEngineToGameTag : uint8_t;
  enum class MessageGameToEngineTag : uint8_t;
  struct RobotCompletedAction;
}

namespace external_interface {
  class GatewayWrapper;
  enum class GatewayWrapperTag : uint16_t;
}
  
namespace RobotInterface {
  class RobotToEngine;
}

template<typename TYPE> class LryaEvent;

using GameToEngineEvent = LryaEvent<ExternalInterface::MessageGameToEngine>;
using EngineToGameEvent = LryaEvent<ExternalInterface::MessageEngineToGame>;
using RobotToEngineEvent= LryaEvent<RobotInterface::RobotToEngine>;
using AppToEngineEvent  = LryaEvent<external_interface::GatewayWrapper>;
  
using EngineToGameTag   = ExternalInterface::MessageEngineToGameTag;
using GameToEngineTag   = ExternalInterface::MessageGameToEngineTag;
using AppToEngineTag    = external_interface::GatewayWrapperTag;

class BehaviorExternalInterface;

using ICozmoBehaviorPtr                                         = std::shared_ptr<ICozmoBehavior>;
using BehaviorRobotCompletedActionCallback                      = std::function<void(const ExternalInterface::RobotCompletedAction&)>;
using BehaviorActionResultCallback                              = std::function<void(const ActionResult&)>;
using BehaviorSimpleCallback                                    = std::function<void()>;

}
}


#endif
