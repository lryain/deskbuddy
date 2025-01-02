//
//  MayaAudioController.h
//  LryaMayaWWisePlugIn
//
//  Created by Jordan Rivas on 1/09/18.
//  Copyright © 2016 Lrya, Inc. All rights reserved.
//

#ifndef __Lrya_MayaAudioController_H__
#define __Lrya_MayaAudioController_H__

#include "audioActionTypes.h"
#include "audioEngine/audioEngineController.h"
#include "audioEngine/soundbankLoader.h"
#include <memory>


class MayaAudioController : public Lrya::AudioEngine::AudioEngineController
{
public:
  
  MayaAudioController( char* soundbanksPath );
  
  bool PostAudioKeyframe( const AudioKeyframe* keyframe );

  bool PlayAudioEvent( const std::string& eventName );

  bool SetParameterValue( const std::string& paramName, const float paramValue );

private:
  
  std::unique_ptr<Lrya::AudioEngine::SoundbankLoader> _soundbankLoader;
  
  Lrya::AudioEngine::AudioGameObject _gameObj = Lrya::AudioEngine::kInvalidAudioGameObject;
  
};


#endif /* __Lrya_MayaAudioController_H__ */
