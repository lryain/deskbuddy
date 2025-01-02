/*
 * File: audioEngineMessageHandler.cpp
 *
 * Author: Jordan Rivas
 * Created: 11/09/2015
 *
 * Description: This is an event handler for Broadcasting and Subscribing to Audio CLAD Messages through
 * MessageAudioClient Message.
 *
 * Copyright: Lrya, Inc. 2015
 */

#include "audioEngine/multiplexer/audioEngineMessageHandler.h"


namespace Lrya {
namespace Cozmo {
namespace Audio {

void AudioEngineMessageHandler::Broadcast( const MessageAudioClient& message)
{
  _eventMgr.Broadcast( LryaEvent<MessageAudioClient>( static_cast<uint32_t>(message.GetTag()), message ));
}


Signal::SmartHandle AudioEngineMessageHandler::Subscribe(const MessageAudioClientTag& tagType, std::function<void(const LryaEvent<MessageAudioClient>&)> messageHandler)
{
  return _eventMgr.Subscribe( static_cast<uint32_t>(tagType), messageHandler );
}

  
} // Audio
} // Cozmo
} // Lrya
