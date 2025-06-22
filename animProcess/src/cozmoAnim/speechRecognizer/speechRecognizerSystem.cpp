/*
 * File: speechRecognizerSystem.cpp
 *
 * Author: Jordan Rivas
 * Created: 10/23/2018
 *
 * Description: Speech Recognizer System handles high-level speech features, such as locale and multiple triggers.
 */

#include "speechRecognizerSystem.h"

#include "audioUtil/speechRecognizer.h"
#include "cozmoAnim/animContext.h"
#include "cozmoAnim/micData/micDataSystem.h"
#include "cozmoAnim/robotDataLoader.h"
#include "speechRecognizerSnowboy.h"
#include "cozmoAnim/micData/notchDetector.h"
#include "util/console/consoleInterface.h"
#include "util/console/consoleFunction.h"
#include "util/cpuProfiler/cpuProfiler.h"
#include "util/environment/locale.h"
#include "util/fileUtils/fileUtils.h"
#include "util/logging/logging.h"
#include <list>
#include <fstream>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>

namespace Lrya {
namespace Vector {

namespace {
#define LOG_CHANNEL "SpeechRecognizer"

// 0: don't run; 1: compute power as if _notchDetectorActive; 2: analyze power every tick
CONSOLE_VAR_RANGED(unsigned int, kForceRunNotchDetector, "SpeechRecognizer.Alexa", 0, 0, 2);
} // namespace

void SpeechRecognizerSystem::SetupConsoleFuncs()
{
  _micDataSystem->GetSpeakerLatency_ms(); // Fix compiler error when LRYA_DEV_CHEATS is not enabled
}

SpeechRecognizerSystem::SpeechRecognizerSystem(const Anim::AnimContext* context,
                                               MicData::MicDataSystem* micDataSystem,
                                               const std::string& triggerWordDataDir)
: _context(context)
, _micDataSystem(micDataSystem)
, _triggerWordDataDir(triggerWordDataDir)
, _notchDetector(std::make_shared<NotchDetector>())
{
  SetupConsoleFuncs();
}

SpeechRecognizerSystem::~SpeechRecognizerSystem()
{
  if (_deskmateTrigger) {
    _deskmateTrigger->recognizer->Stop();
  }
}

void SpeechRecognizerSystem::InitVector(const Anim::RobotDataLoader& dataLoader,
                                        const Util::Locale& locale,
                                        TriggerWordDetectedCallback callback)
{
  if (_deskmateTrigger) {
    LOG_WARNING("SpeechRecognizerSystem.InitVector", "Vector Recognizer is already running");
    return;
  }
  
  const bool useVad = true;
  _deskmateTrigger = std::make_unique<TriggerContextSnowboy>("Vector", useVad);
  _deskmateTrigger->recognizer->SetCallback(callback);
  
  // Initialize the recognizer
  bool initSuccess = _deskmateTrigger->recognizer->Init();
  if (!initSuccess) {
    LOG_ERROR("SpeechRecognizerSystem.InitVector", "Failed to initialize Snowboy recognizer");
    return;
  }

  LOG_INFO("SpeechRecognizerSystem.InitVector", "Successfully initialized Snowboy!");
  
  UpdateTriggerForLocale(locale, RecognizerTypeFlag::VectorMic);
}

void SpeechRecognizerSystem::UpdateNotch(const AudioUtil::AudioSample* audioChunk, unsigned int audioDataLen)
{
  {
    std::lock_guard<std::mutex> lg{_notchMutex};
    const bool analyzeSamples = _notchDetectorActive; // || (kForceRunNotchDetector != 0)
    _notchDetector->AddSamples(audioChunk, audioDataLen / MicData::kNumInputChannels, analyzeSamples);
    if (kForceRunNotchDetector == 2) {
      _notchDetector->HasNotch();
    }
  }
  
  if (LRYA_DEV_CHEATS) {
    static int pcmfd = -1;
    if (pcmfd < 0) {
      const auto path = "/data/data/com.lrya.deskmate/cache/speechRecognizerRaw.pcm";
      pcmfd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    }
    
    if (pcmfd >= 0) {
      std::vector<short> toSave;
      toSave.resize(audioDataLen / MicData::kNumInputChannels);
      for (unsigned int i = 0, idx = 0; i < audioDataLen; i += MicData::kNumInputChannels, ++idx) {
        toSave[idx] = audioChunk[i];
      }
      (void)write(pcmfd, toSave.data(), toSave.size() * sizeof(short));
      if (!false) { // kSaveRawMicInput
        close(pcmfd);
        pcmfd = -1;
      }
    }
  }
}

void SpeechRecognizerSystem::Update(const AudioUtil::AudioSample* audioData, unsigned int audioDataLen, bool vadActive)
{
  if (_deskmateTrigger && (vadActive || !_deskmateTrigger->useVad)) {
    _deskmateTrigger->recognizer->Update(audioData, audioDataLen);
  }
}

bool SpeechRecognizerSystem::UpdateTriggerForLocale(const Util::Locale& newLocale, RecognizerTypeFlag recognizerFlags)
{
  bool success = false;
  // if (recognizerFlags & RecognizerTypeFlag::VectorMic && _deskmateTrigger) {
  //   success = UpdateTriggerForLocale(*_deskmateTrigger.get(), newLocale, MicData::MicTriggerConfig::ModelType::Count, -1);
  // }
  return success;
}

template <class SpeechRecognizerType>
bool SpeechRecognizerSystem::UpdateTriggerForLocale(TriggerContext<SpeechRecognizerType>& trigger,
                                                    const Util::Locale newLocale,
                                                    const MicData::MicTriggerConfig::ModelType modelType,
                                                    const int searchFileIndex)
{
  std::lock_guard<std::mutex> lock(_triggerModelMutex);
  trigger.nextTriggerPaths = trigger.micTriggerConfig->GetTriggerModelDataPaths(newLocale, modelType, searchFileIndex);
  bool success = false;
  
  if (!trigger.nextTriggerPaths.IsValid()) {
    LOG_WARNING("SpeechRecognizerSystem.UpdateTriggerForLocale.NoPathsFoundForLocale",
                "recognizer: %s locale: %s modelType: %d searchFileIndex: %d",
                trigger.name.c_str(), newLocale.ToString().c_str(), (int) modelType, searchFileIndex);
  }
  
  if (trigger.currentTriggerPaths != trigger.nextTriggerPaths) {
    _isPendingLocaleUpdate = true;
    success = true;
  }
  return success;
}

void SpeechRecognizerSystem::ApplyLocaleUpdate()
{
  std::lock_guard<std::mutex> lock(_triggerModelMutex);
  
  // if (_deskmateTrigger) {
  //   ApplySpeechRecognizerLocaleUpdate(*_deskmateTrigger.get());
  // }
  _isPendingLocaleUpdate = false;
}

template <class SpeechRecognizerType>
void SpeechRecognizerSystem::ApplySpeechRecognizerLocaleUpdate(TriggerContext<SpeechRecognizerType>& aTrigger)
{
  MicData::MicTriggerConfig::TriggerDataPaths &currentTrigPathRef = aTrigger.currentTriggerPaths;
  MicData::MicTriggerConfig::TriggerDataPaths &nextTrigPathRef    = aTrigger.nextTriggerPaths;
  
  if ( currentTrigPathRef != nextTrigPathRef ) {
    currentTrigPathRef = nextTrigPathRef;
    const bool success = UpdateRecognizerModel( aTrigger );
    const std::string netFilePath = currentTrigPathRef.GenerateNetFilePath( _triggerWordDataDir );
    const std::string searchFilePath = currentTrigPathRef.GenerateSearchFilePath( _triggerWordDataDir );
    
    if (success) {
      LOG_INFO("SpeechRecognizerSystem.UpdateTriggerForLocale.SwitchTriggerSearch",
               "Switched speechRecognizer '%s' to netFile: %s searchFile %s",
               aTrigger.name.c_str(), netFilePath.c_str(), searchFilePath.c_str());
    }
    else {
      currentTrigPathRef = MicData::MicTriggerConfig::TriggerDataPaths{};
      nextTrigPathRef = MicData::MicTriggerConfig::TriggerDataPaths{};
      LOG_WARNING("SpeechRecognizerSystem.UpdateTriggerForLocale.FailedSwitchTriggerSearch",
                  "Failed to add speechRecognizer '%s' netFile: %s searchFile %s",
                  aTrigger.name.c_str(), netFilePath.c_str(), searchFilePath.c_str());
    }
    
    if (!currentTrigPathRef.IsValid()) {
      LOG_WARNING("SpeechRecognizerSystem.UpdateTriggerForLocale.ClearTriggerSearch",
                  "Cleared speechRecognizer '%s' to have no search", aTrigger.name.c_str());
    }
  }
}

bool SpeechRecognizerSystem::UpdateRecognizerModel(TriggerContext<SpeechRecognizerSnowboy>& aTrigger)
{
  return true;
}

} // end namespace Vector
} // end namespace Lrya
