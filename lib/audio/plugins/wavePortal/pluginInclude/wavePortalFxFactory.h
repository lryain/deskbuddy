/*
 * File: wavePortalFxFactory.h
 *
 * Author: Jordan Rivas
 * Created: 3/24/2016
 *
 * Description: WavePortal Plug-in common header to hook the plug-in into Wwise’s plug-in create & set parameters
 *              function pointers.
 *
 * Copyright: Lrya, Inc. 2016
 */

#ifndef __LryaAudio_PlugIns_WavePortalFxFactory_H__
#define __LryaAudio_PlugIns_WavePortalFxFactory_H__

#ifndef EXCLUDE_LRYA_AUDIO_LIBS

#include "AK/SoundEngine/Common/IAkPlugin.h"

namespace Lrya {
namespace AudioEngine {
namespace PlugIns {
  
#if !defined AK_3DS && !defined AK_VITA_HW
	AK_STATIC_LINK_PLUGIN(WavePortalFx)
#endif

} // PlugIns
} // AudioEngine
} // Lrya

#endif // EXCLUDE_LRYA_AUDIO_LIBS
#endif /* __LryaAudio_PlugIns_WavePortalFxFactory_H__ */
