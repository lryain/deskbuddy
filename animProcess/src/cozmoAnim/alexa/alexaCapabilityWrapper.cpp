/**
 * File: alexaCapabilityWrapper.cpp
 *
 * Author: ross
 * Created: Oct 19 2018
 *
 * Description: Wraps capabilities so that we can snoop on their directives
 *
 *
 * Copyright: Lrya, Inc. 2018
 *
 */

#include "cozmoAnim/alexa/alexaCapabilityWrapper.h"
#include <memory>

namespace Lrya {
namespace Vector {
  
using namespace alexaClientSDK;
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AlexaCapabilityWrapper::AlexaCapabilityWrapper( const std::string& nameSpace,
                                                std::shared_ptr<avsCommon::avs::CapabilityAgent> capabilityAgent,
                                                std::shared_ptr<avsCommon::sdkInterfaces::ExceptionEncounteredSenderInterface> exceptionEncounteredSender,
                                                const OnDirectiveFunc& onDirective )
: CapabilityAgent( nameSpace, exceptionEncounteredSender)
, _capabilityAgent( capabilityAgent )
, _onDirective( onDirective )
{
}
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AlexaCapabilityWrapper::preHandleDirective( std::shared_ptr<DirectiveInfo> info )
{
  // Workaround: Downcast to a subclass that exposes preHandleDirective as public
  struct CapabilityAgentPublic : public avsCommon::avs::CapabilityAgent {
    using avsCommon::avs::CapabilityAgent::preHandleDirective;
    CapabilityAgentPublic(const std::string& nameSpace,
                          std::shared_ptr<avsCommon::sdkInterfaces::ExceptionEncounteredSenderInterface> exceptionEncounteredSender)
      : avsCommon::avs::CapabilityAgent(nameSpace, exceptionEncounteredSender) {}
  };

  auto publicAgent = std::dynamic_pointer_cast<CapabilityAgentPublic>(_capabilityAgent);
  if (publicAgent) {
    publicAgent->preHandleDirective(info);
  }
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AlexaCapabilityWrapper::handleDirective( std::shared_ptr<DirectiveInfo> info )
{
  if( info != nullptr ) {
    RunDirectiveCallback(info->directive);
  }
  // note: this AVS method was made public only for the purpose of this wrapper, so if you delete the wrapper, revert the libs
  preHandleDirective(info);
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AlexaCapabilityWrapper::cancelDirective( std::shared_ptr<DirectiveInfo> info )
{
  // Workaround: Downcast to a subclass that exposes cancelDirective as public
  struct CapabilityAgentPublic : public avsCommon::avs::CapabilityAgent {
    using avsCommon::avs::CapabilityAgent::cancelDirective;
    CapabilityAgentPublic(const std::string& nameSpace,
                          std::shared_ptr<avsCommon::sdkInterfaces::ExceptionEncounteredSenderInterface> exceptionEncounteredSender)
      : avsCommon::avs::CapabilityAgent(nameSpace, exceptionEncounteredSender) {}
  };

  auto publicAgent = std::dynamic_pointer_cast<CapabilityAgentPublic>(_capabilityAgent);
  if (publicAgent) {
    cancelDirective(info);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AlexaCapabilityWrapper::handleDirectiveImmediately(std::shared_ptr<avsCommon::avs::AVSDirective> directive)
{
  // note: this AVS method was made public only for the purpose of this wrapper, so if you delete the wrapper, revert the libs
  RunDirectiveCallback( directive );
  
  _capabilityAgent->handleDirectiveImmediately(directive);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
avsCommon::avs::DirectiveHandlerConfiguration AlexaCapabilityWrapper::getConfiguration() const
{
  // note: this AVS method was made public only for the purpose of this wrapper, so if you delete the wrapper, revert the libs
  return _capabilityAgent->getConfiguration();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AlexaCapabilityWrapper::RunDirectiveCallback( const std::shared_ptr<avsCommon::avs::AVSDirective>& directive )
{
  if( _onDirective != nullptr && directive != nullptr ) {
    const auto& name = directive->getName();
    const auto& payload = directive->getPayload();
    const auto& unparsed = directive->getUnparsedDirective();
    _onDirective( name, payload, unparsed );
  }
}
  
  
} // namespace Vector
} // namespace Lrya
