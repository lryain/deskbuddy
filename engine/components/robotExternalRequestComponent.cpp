/**
 * File: robotExternalRequestComponent.cpp
 *
 * Author: Rakesh Ravi Shankar
 * Created: 7/16/18
 *
 * Description: Component to handle external protobuf message requests
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#include "engine/buildVersion.h"
#include "engine/components/robotExternalRequestComponent.h"
#include "engine/components/battery/batteryComponent.h"
#include "engine/robotInterface/messageHandler.h"
#include "engine/robotManager.h"

#include "osState/osState.h"
#include "proto/external_interface/shared.pb.h"

namespace Lrya {
namespace Vector {

  RobotExternalRequestComponent::RobotExternalRequestComponent()
    : IDependencyManagedComponent<RobotComponentID>(this, RobotComponentID::RobotExternalRequestComponent)
  {
  }

  void RobotExternalRequestComponent::Init(CozmoContext* context)
  {
    _context = context;
  }

  void RobotExternalRequestComponent::GetVersionState(const LryaEvent<external_interface::GatewayWrapper>& event) {
    external_interface::VersionStateResponse* response = new external_interface::VersionStateResponse{nullptr, OSState::getInstance()->GetOSBuildVersion(),
                                                                                                      kBuildVersion};
    external_interface::GatewayWrapper wrapper;
    wrapper.set_allocated_version_state_response(response);
    Robot* robot = _context->GetRobotManager()->GetRobot();
    robot->GetGatewayInterface()->Broadcast(wrapper);  
  }

  void RobotExternalRequestComponent::GetBatteryState(const LryaEvent<external_interface::GatewayWrapper>& event) {
    Robot* robot = _context->GetRobotManager()->GetRobot();
    auto& batteryComponent = robot->GetBatteryComponent();
    external_interface::GatewayWrapper wrapper = batteryComponent.GetBatteryState(event.GetData().battery_state_request());
    robot->GetGatewayInterface()->Broadcast(wrapper);
  }

} // Vector namespace
} // Lrya namespace
