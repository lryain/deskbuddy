/**
* File: audioDataTypes.h
*
* Author: Lee Crippen
* Created: 12/13/2016
*
* Description: Some often reused audio defines and types.
*
* Copyright: Lrya, Inc. 2016
*
*/

#ifndef __Lrya_AudioUtil_AudioDataTypes_H_
#define __Lrya_AudioUtil_AudioDataTypes_H_

#include <cstdint>
#include <deque>
#include <vector>
// #include "iAudioInputSource.h"

// Fwd delcared mac struct type to be filled out in the .mm file for iOS and OSX
struct AudioStreamBasicDescription;

namespace Lrya {
namespace AudioUtil {
  
using AudioSample = int16_t;
using AudioChunk = std::vector<AudioSample>;
using AudioChunkList = std::deque<AudioChunk>;
  
constexpr uint32_t kSampleRate_hz = 16000;
constexpr uint32_t kTimePerAudioChunk_ms = 125;
constexpr uint32_t kSamplesPerChunk = kTimePerAudioChunk_ms * kSampleRate_hz / 1000;
constexpr uint32_t kBytesPerChunk = kSamplesPerChunk * sizeof(AudioSample);

void GetStandardAudioDescriptionFormat(AudioStreamBasicDescription& description_out);

} // end namespace AudioUtil
} // end namespace Lrya


#endif //__Lrya_AudioUtil_AudioDataTypes_H_
