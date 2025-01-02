/*
 * File: akAlsaSinkPluginTypes.h
 *
 * Author: Jordan Rivas
 * Created: 02/01/19
 *
 * Description: Constants for Ak ALSA Sink Plugin
 *
 * Copyright: Lrya, Inc. 2019
 */

#ifndef __LryaAudio_PlugIns_AkAlsaSinkPluginTypes_H__
#define __LryaAudio_PlugIns_AkAlsaSinkPluginTypes_H__

#include <cstdint>
#include <functional>


namespace Lrya {
namespace AudioEngine {
namespace PlugIns {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class AkAlsaSinkPluginTypes {
  public:
    // NOTE: These are all expected in Vector Project
    static constexpr size_t kAkAlsaSinkSampleRate   = 32000;
    static constexpr size_t kAkAlsaSinkBufferSize   = 1024;
    static constexpr size_t kAkAlsaSinkBufferCount  = 3;
    static constexpr size_t kAkAlsaSinkChannelCount = 1;
    
    
    using AkSinkAudioSample_t = int16_t;
    using AudioChunk = AkSinkAudioSample_t[ kAkAlsaSinkBufferSize ];
    
    // Callback for Audio Engine end of frame
    // param: period is reference to periods data
    // param: hasData is true when the frame has audio data, not silence
    using WriteBufferToAlsaCallbackFunc = std::function<void(const AudioChunk& chunk, bool hasData)>;
  };

} // PlugIns
} // AudioEngine
} // Lrya

#endif /* __LryaAudio_PlugIns_AkAlsaSinkPluginTypes_H__ */
