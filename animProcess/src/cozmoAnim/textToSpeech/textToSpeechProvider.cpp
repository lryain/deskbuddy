/**
 * File: textToSpeechProvider.cpp
 *
 * Description: Platform-agnostic interface to platform-specific implementations
 *
 * Copyright: Lrya, Inc. 2017
 *
 */

#include "textToSpeechProvider.h"

#include "util/helpers/lryaDefines.h"

// Which provider implementation do we use for this platform?
#if defined(LRYA_PLATFORM_OSX)
#include "textToSpeechProvider_mac.h"
#elif defined(LRYA_PLATFORM_MATEOS)
#include "textToSpeechProvider_mateos.h"
#else
#error "No text-to-speech provider implemented for this platform"
#endif

#include "cozmoAnim/animContext.h"
#include "json/json.h"
#include "util/logging/logging.h"

#define LOG_CHANNEL "TextToSpeechProvider"

namespace Lrya {
namespace Vector {
namespace TextToSpeech {

TextToSpeechProvider::TextToSpeechProvider(const Anim::AnimContext * ctx, const Json::Value& tts_config)
{
  // Get configuration struct for this platform
#if defined(LRYA_PLATFORM_OSX)
  Json::Value tts_platform_config = tts_config["osx"];
#elif defined(LRYA_PLATFORM_IOS)
  Json::Value tts_platform_config = tts_config["ios"];
#elif defined(LRYA_PLATFORM_MATEOS)
  Json::Value tts_platform_config = tts_config["mateos"];
#endif

  // Instantiate provider for this platform
  _impl = std::make_unique<TextToSpeechProviderImpl>(ctx, tts_platform_config);
}

TextToSpeechProvider::~TextToSpeechProvider()
{
  // Nothing to do here
}

Result TextToSpeechProvider::SetLocale(const std::string & locale)
{
  // Forward to implementation
  DEV_ASSERT(_impl != nullptr, "TextToSpeechProvider.SetLocale.InvalidImplementation");
  return _impl->SetLocale(locale);
}

Result TextToSpeechProvider::GetFirstAudioData(const std::string & text,
                                               float durationScalar,
                                               float pitchScalar,
                                               TextToSpeechProviderData & data,
                                               bool & done)
{
  // Forward to implementation
  DEV_ASSERT(_impl != nullptr, "TextToSpeechProvider.GetFirstAudioData.InvalidImplementation");
  return _impl->GetFirstAudioData(text, durationScalar, pitchScalar, data, done);
}

Result TextToSpeechProvider::GetNextAudioData(TextToSpeechProviderData & data, bool & done)
{
  // Forward to implementation
  DEV_ASSERT(_impl != nullptr, "TextToSpeechProvider.GetNextAudioData.InvalidImplementation");
  return _impl->GetNextAudioData(data, done);
}

} // end namespace TextToSpeech
} // end namespace Vector
} // end namespace Lrya
