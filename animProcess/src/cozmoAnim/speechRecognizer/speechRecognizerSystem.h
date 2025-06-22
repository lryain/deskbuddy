/**
* File: speechRecognizerSystem.h
*
* Author: Jordan Rivas
* Created: 10/23/2018
*
* Description: Speech Recognizer System handles high-level speech features, such as locale and multiple triggers.
*
*/

#ifndef __AnimProcess_VictorAnim_SpeechRecognizerSystem_H_
#define __AnimProcess_VictorAnim_SpeechRecognizerSystem_H_

#include "audioUtil/audioDataTypes.h"
#include "cozmoAnim/micData/micTriggerConfig.h"
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

namespace Lrya {
  namespace AudioUtil {
    class SpeechRecognizer;
    struct SpeechRecognizerCallbackInfo;
    struct SpeechRecognizerIgnoreReason;
  }
  namespace Vector {
    namespace Anim {
      class AnimContext;
    }
    namespace MicData {
      class MicDataSystem;
    }
    class NotchDetector;
    namespace Anim {
      class RobotDataLoader;
    }
    class SpeechRecognizerSnowboy;
    class SpeechRecognizerPryonLite;
    namespace {
      struct TriggerModelTypeData;
    }
  }
  namespace Util {
    class Locale;
  }
}

namespace Lrya {
namespace Vector {

class SpeechRecognizerSystem
{
public:

  SpeechRecognizerSystem(const Anim::AnimContext* context,
                         MicData::MicDataSystem* micDataSystem,
                         const std::string& triggerWordDataDir);
  
  ~SpeechRecognizerSystem();
  
  SpeechRecognizerSystem(const SpeechRecognizerSystem& other) = delete;
  SpeechRecognizerSystem& operator=(const SpeechRecognizerSystem& other) = delete;
  
  using TriggerWordDetectedCallback = std::function<void(const AudioUtil::SpeechRecognizerCallbackInfo& info)>;
  
  // Init Vector trigger detector
  // Note: This always happens at boot
  void InitVector(const Anim::RobotDataLoader& dataLoader,
                  const Util::Locale& locale,
                  TriggerWordDetectedCallback callback);

  // add 'raw' audio samples
  void UpdateNotch(const AudioUtil::AudioSample* audioChunk, unsigned int audioDataLen);

  // Update recognizer audio
  // NOTE: Always call from the same thread
  void Update(const AudioUtil::AudioSample * audioData, unsigned int audioDataLen, bool vadActive);
  
  // Set Default models for locale
  // Use flag to describe what recognizer(s) to updated
  // Return true when locale file was found and is different then current locale
  // NOTE: Locale is not updated until the next Update() call
  enum RecognizerTypeFlag {
    None          = 0,
    VectorMic     = 1 << 0,
    All           = VectorMic
  };
  
  bool UpdateTriggerForLocale(const Util::Locale& newLocale,
                              RecognizerTypeFlag recognizerFlags = RecognizerTypeFlag::All);

private:
  
  // Trigger context
  template <class SpeechRecognizerType>
  struct TriggerContext {
    std::string                                 name;
    std::unique_ptr<SpeechRecognizerType>       recognizer;
    std::unique_ptr<MicData::MicTriggerConfig>  micTriggerConfig;

    // For tracking and altering the trigger model being used
    MicData::MicTriggerConfig::TriggerDataPaths currentTriggerPaths;
    MicData::MicTriggerConfig::TriggerDataPaths nextTriggerPaths;
    
    bool                                        useVad;

    TriggerContext(const std::string& name, bool useVad)
    : name(name)
    , recognizer(std::make_unique<SpeechRecognizerType>())
    , micTriggerConfig(std::make_unique<MicData::MicTriggerConfig>())
    , useVad(useVad)
    { }
  };
  
  using TriggerContextSnowboy = TriggerContext<SpeechRecognizerSnowboy>;
  using TriggerContextPryon = TriggerContext<SpeechRecognizerPryonLite>;
  
  const Anim::AnimContext*                    _context = nullptr;
  MicData::MicDataSystem*                     _micDataSystem = nullptr;
  std::unique_ptr<TriggerContextSnowboy>    _deskmateTrigger;
  
  std::atomic_uint64_t                        _playbackTrigerSampleIdx{ 0 };
  std::string                                 _triggerWordDataDir;
  
  std::mutex                                  _triggerModelMutex;
  std::atomic_bool                            _isPendingLocaleUpdate{ false };
  
  std::shared_ptr<NotchDetector>              _notchDetector;
  std::mutex                                  _notchMutex;
  bool                                        _notchDetectorActive = false;
  
  // Set custom model and search files for locale
  // Return true when locale file was found and is different then current locale
  // NOTE: This only sets the _nextTriggerPaths; the locale will be updated in the Update() call
  template <class SpeechRecognizerType>
  bool UpdateTriggerForLocale(TriggerContext<SpeechRecognizerType>& trigger,
                              const Util::Locale newLocale,
                              const MicData::MicTriggerConfig::ModelType modelType,
                              const int searchFileIndex);
  
  // This should only be called from the Update() methods; needs to be performed on the same thread
  void ApplyLocaleUpdate();
  
  template <class SpeechRecognizerType>
  void ApplySpeechRecognizerLocaleUpdate(TriggerContext<SpeechRecognizerType>& aTrigger);
  
  bool UpdateRecognizerModel(TriggerContext<SpeechRecognizerSnowboy>& aTrigger);
  bool UpdateRecognizerModel(TriggerContext<SpeechRecognizerPryonLite>& aTrigger);
  
  // Console Var methods
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // NOTE: These methods provide no functionality when LRYA_DEV_CHEATS is turned off
  void SetupConsoleFuncs();
  
  template <class SpeechRecognizerType>
  std::string UpdateRecognizerHelper(size_t& inOut_modelIdx, size_t new_modelIdx,
                                     size_t& inOut_searchIdx, size_t new_searchIdx,
                                     const TriggerModelTypeData modelTypeDataList[],
                                     TriggerContext<SpeechRecognizerType>& trigger);
};

} // end namespace Vector
} // end namespace Lrya

#endif // __AnimProcess_VictorAnim_SpeechRecognizerSystem_H_

