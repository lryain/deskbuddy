/*
 * File: audioTypeTranslator.h
 *
 * Author: Jordan Rivas
 * Created: 9/29/17
 *
 * Description: Helper functions to properly convert CLAD and standard types to AudioEngine types
 *
 * Copyright: Lrya, Inc. 2017
 */


#ifndef __LryaAudio_AudioTypeTranslator_H__
#define __LryaAudio_AudioTypeTranslator_H__

#include "audioEngine/audioTypes.h"
#include "clad/audio/audioMessageTypes.h"
#include "clad/audio/audioEventTypes.h"
#include "clad/audio/audioGameObjectTypes.h"
#include "clad/audio/audioStateTypes.h"
#include "clad/audio/audioSwitchTypes.h"
#include "clad/audio/audioParameterTypes.h"
#include "util/math/numericCast.h"


namespace Lrya {
namespace AudioEngine {

// Translate AudioMetaData Clad types to AudioEngine Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AudioGameObject ToAudioGameObject(AudioMetaData::GameObjectType gameObject);

AudioEventId ToAudioEventId(AudioMetaData::GameEvent::GenericEvent event);

AudioParameterId ToAudioParameterId(AudioMetaData::GameParameter::ParameterType parameter);

AudioSwitchGroupId ToAudioSwitchGroupId(AudioMetaData::SwitchState::SwitchGroupType switchGroup);

AudioSwitchStateId ToAudioSwitchStateId(AudioMetaData::SwitchState::GenericSwitch switchState);

AudioStateGroupId ToAudioStateGroupId(AudioMetaData::GameState::StateGroupType stateGroup);

AudioStateId ToAudioStateId(AudioMetaData::GameState::GenericState state);


// Translate standard types to AudioEngine Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AudioRTPCValue ToAudioRTPCValue(float value);

AudioTimeMs ToAudioTimeMs(int32_t timeMs);

  
// Translate Multiplexer types to AudioEngine Types
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AudioCurveType ToAudioCurveType( Multiplexer::CurveType curve );

}
}

#endif /* __LryaAudio_AudioTypeTranslator_H__ */
