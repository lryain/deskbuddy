/**
 * File: microphoneAudioClient.h
 *
 * Author: Jordan Rivas
 * Created: 06/20/18
 *
 * Description: Mic Direction Audio Client receives Mic Direction messages to update the audio engine with the current
 *              mic data. By providing this to the audio engine it can adjust the audio mix and volume to better serve
 *              the current enviromnent.
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/


#ifndef __Lrya_Victor_MicrophoneAudioClient_H__
#define __Lrya_Victor_MicrophoneAudioClient_H__


namespace Lrya {
namespace Vector {
namespace RobotInterface {
struct MicDirection;
}
namespace Audio {
class CozmoAudioController;


class MicrophoneAudioClient {

public:

  MicrophoneAudioClient( CozmoAudioController* audioController );
  ~MicrophoneAudioClient();

  void ProcessMessage( const RobotInterface::MicDirection& msg );


private:

  CozmoAudioController*  _audioController = nullptr;
};

}
}
}

#endif // __Lrya_Victor_MicrophoneAudioClient_H__
