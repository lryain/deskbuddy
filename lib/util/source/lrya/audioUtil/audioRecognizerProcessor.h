/**
* File: audioRecognizerProcessor.h
*
* Author: Lee Crippen
* Created: 1/20/17
*
* Description: Component that uses native audio capture and feeds it to a specified speech recognizer,
* then holds onto the results to be accessed by another system.
*
* Copyright: Lrya, Inc. 2017
*
*/

#ifndef __Lrya_AudioUtil_AudioRecognizerProcessor_H_
#define __Lrya_AudioUtil_AudioRecognizerProcessor_H_

#include "audioUtil/audioDataTypes.h"

#include <deque>
#include <memory>
#include <mutex>
#include <string>

namespace Lrya {
namespace AudioUtil {
class IAudioInputSource;
class SpeechRecognizer;
struct SpeechRecognizerCallbackInfo;

  
class AudioRecognizerProcessor
{
public:
  AudioRecognizerProcessor(const std::string& savedAudioDir);
  virtual ~AudioRecognizerProcessor();
  AudioRecognizerProcessor(AudioRecognizerProcessor&& other) = delete;
  AudioRecognizerProcessor& operator=(AudioRecognizerProcessor&& other) = delete;
  AudioRecognizerProcessor(const AudioRecognizerProcessor& other) = delete;
  AudioRecognizerProcessor& operator=(const AudioRecognizerProcessor& other) = delete;
  
  void SetAudioInputSource(IAudioInputSource* newCaptureSystem);
  void SetSpeechRecognizer(SpeechRecognizer* newRecog);
  void Start();
  void Stop();

  using ResultType = std::pair<std::string, float>;
  bool HasResults() const;
  ResultType PopNextResult();
  
private:
  SpeechRecognizer*                       _recognizer = nullptr;
  IAudioInputSource*                      _audioInputSource = nullptr;
  bool                                    _capturingAudio = false;
  std::mutex                              _componentsMutex;
  mutable std::mutex                      _resultMutex;
  std::deque<ResultType>                  _procResults;
  
  void AudioSamplesCallback(const AudioSample* buffer, uint32_t numSamples);
  void AddRecognizerResult(const AudioUtil::SpeechRecognizerCallbackInfo& info);
  
}; // class AudioRecognizerProcessor
    
} // end namespace AudioUtil
} // end namespace Lrya

#endif // __Lrya_AudioUtil_AudioRecognizerProcessor_H_
