/**
* File: audioPlayer.h
*
* Author: Lee Crippen
* Created: 11/29/2016
*
* Description: Platform independent interface for playing audio data natively.
*
* Copyright: Lrya, Inc. 2016
*
*/

#ifndef __Lrya_AudioUtil_AudioPlayer_H_
#define __Lrya_AudioUtil_AudioPlayer_H_

#include "audioDataTypes.h"

#include <memory>

namespace Lrya {
namespace AudioUtil {
  
// Type declaration to allow hiding impl in cpp
struct AudioPlayerData;

class AudioPlayer
{
public:
  AudioPlayer(uint32_t samplesPerChunk = kSamplesPerChunk);
  virtual ~AudioPlayer();
  AudioPlayer& operator=(AudioPlayer&&);
  AudioPlayer(AudioPlayer&&);
  AudioPlayer& operator=(const AudioPlayer&) = delete;
  AudioPlayer(const AudioPlayer&) = delete;
  
  bool StartPlaying(AudioChunkList::const_iterator beginIter, AudioChunkList::const_iterator endIter);
  void StopPlaying();
  
private:
  std::unique_ptr<AudioPlayerData>   _impl;
  
}; // class AudioPlayer

} // end namespace AudioUtil
} // end namespace Lrya


#endif //__Lrya_AudioUtil_AudioPlayer_H_
