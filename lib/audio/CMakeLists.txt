cmake_minimum_required(VERSION 3.10)

project(lrya_audio)

if (NOT DEFINED LRYA_LIB_AUDIO_PLUGINS)
  message(FATAL_ERROR "You must define LRYA_LIB_AUDIO_PLUGINS before including this library")
  message(FATAL_ERROR "Try set(LRYA_LIB_AUDIO_PLUGINS AE_AK_COMPRESSOR_FX AE_AK_DELAY_FX...)")
  return()
endif()

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

# sadly... these seem to be ignored by the cmake android toolchain
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)

include(lrya_build_cxx_compile_options)
include(lrya_build_cxx_compile_definitions)
include(lrya_build_strip)

# 定义一个选项
# !!DO NOT USE AUDIO ENGINE!!
add_compile_definitions(EXCLUDE_LRYA_AUDIO_LIBS ON)

option(USE_STREAMING_WAVE_PORTAL_PLUGIN "Use STREAMING_WAVE_PORTAL Plugin" OFF)
option(USE_KROTOS_VOCODER_PLUGIN "Use Krotos Vocoder plugin" OFF)
option(USE_AKALSASINK_PLUGIN "Use AK Alsa Sink plugin" OFF)
option(USE_HIJACK_PLUGIN "Use Hijack plugin" OFF)
option(USE_WAVE_PORTAL_PLUGIN "Use Wave Portal plugin" OFF)

if (NOT TARGET util)
  # get path to lrya-util and use it below
  add_subdirectory(../../lib/util "${CMAKE_BINARY_DIR}/util")
endif()

if (NOT TARGET clad)
  # get path to message-buffers
  add_subdirectory(../../tools/message-buffers "${CMAKE_BINARY_DIR}/message-buffers")
endif()

#
# wwise
#

include(wwise)

#
# zipreader
#

add_library(zipreader
  STATIC
  zipreader/zipReader.cpp
  zipreader/zipReader.h
)

target_compile_options(zipreader
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_compile_definitions(zipreader
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

target_include_directories(zipreader 
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/zipreader>
)

lrya_build_target_license(zipreader "LRYA")

lrya_build_strip(TARGET zipreader)


#
# Hijack Audio plugin
#
if(USE_HIJACK_PLUGIN)
        set(HIJACK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugins/hijackAudio)

        add_library(hijack_audio
        STATIC
        ${HIJACK_DIR}/pluginSource/hijackFx.cpp
        ${HIJACK_DIR}/pluginSource/hijackFxDsp.cpp
        ${HIJACK_DIR}/pluginSource/hijackFxParams.cpp
        )

        target_compile_options(hijack_audio
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
        -Wno-deprecated-declarations
        -Wno-undef
        )

        target_compile_definitions(hijack_audio
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
        )

        target_include_directories(hijack_audio
        PUBLIC
        $<BUILD_INTERFACE:${HIJACK_DIR}/pluginInclude>
        # PRIVATE - BRC-TODO: should be private be needs hijackFx.h
        $<BUILD_INTERFACE:${HIJACK_DIR}/pluginSource>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${WWISE_SDK_ROOT}/include>
        )

        lrya_build_target_license(hijack_audio "LRYA")

        lrya_build_strip(TARGET hijack_audio)
else()
        message("Skipping AKALSASINK Plugin")
endif()

#
# Wave Portal plugin
#
if(USE_WAVE_PORTAL_PLUGIN)
        set(WAVEPORTAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugins/wavePortal)

        add_library(wave_portal
        STATIC
        ${WAVEPORTAL_DIR}/pluginSource/wavePortalFx.cpp
        ${WAVEPORTAL_DIR}/pluginSource/wavePortalFxParams.cpp
        )

        target_compile_options(wave_portal
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
        -Wno-deprecated-declarations
        -Wno-undef
        )

        target_compile_definitions(wave_portal
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
        )

        target_include_directories(wave_portal
        PUBLIC
        $<BUILD_INTERFACE:${WAVEPORTAL_DIR}/pluginInclude>
        # PRIVATE TODO-BRC: should be private be engine needs  wavePortalFx.h
        $<BUILD_INTERFACE:${WAVEPORTAL_DIR}/pluginSource>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${WWISE_SDK_ROOT}/include>
        )

        lrya_build_target_license(wave_portal "LRYA")

        lrya_build_strip(TARGET wave_portal)

        add_compile_definitions(EXCLUDE_LRYA_AUDIO_LIBS ON)
else()
        message("Skipping Wave Portal plugin")
endif()
#
# Streaming Wave Portal plugin
#

# 根据选项决定编译行为
if(USE_STREAMING_WAVE_PORTAL_PLUGIN)
        message("--------------------------------------USE_STREAMING_WAVE_PORTAL_PLUGIN----------------------------------------------")
        set(STREAMING_WAVE_PORTAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugins/streamingWavePortal)

        add_library(streaming_wave_portal
        STATIC
        ${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx/streamingWavePortalFx.cpp
        ${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx/streamingWavePortalFx.h
        ${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx/streamingWavePortalId.h
        ${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx/streamingWavePortalParams.cpp
        ${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx/streamingWavePortalParams.h
        )

        target_compile_options(streaming_wave_portal
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
        -Wno-deprecated-declarations
        -Wno-undef
        )

        target_compile_definitions(streaming_wave_portal
        PRIVATE
        ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
        )

        target_include_directories(streaming_wave_portal
        PUBLIC
        $<BUILD_INTERFACE:${STREAMING_WAVE_PORTAL_DIR}/pluginInclude>
        $<BUILD_INTERFACE:${STREAMING_WAVE_PORTAL_DIR}/audioEngineFx>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${WWISE_SDK_ROOT}/include>
        )

        lrya_build_target_license(streaming_wave_portal "LRYA")

        lrya_build_strip(TARGET streaming_wave_portal)
else()
        message("Skipping STREAMING_WAVE_PORTAL Plugin")
endif()

#
# Krotos Vocoder plugin
#
if(USE_KROTOS_VOCODER_PLUGIN)
        add_subdirectory("plugins/krotosVocoder")
else()
        message("Skipping Krotos Vocoder Plugin")
endif()

#
# Ak Alsa Sink Plugin
#
if(USE_AKALSASINK_PLUGIN)
        if(MATEOS)
        set(AKALSASINK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugins/akAlsaSink)
        set(AKALSASINK_PLUGIN_DIR ${AKALSASINK_DIR}/soundEnginePlugin)

        add_library(ak_alsa_sink STATIC
        ${AKALSASINK_PLUGIN_DIR}/AkAlsaSink.cpp
        ${AKALSASINK_PLUGIN_DIR}/AkAlsaSinkParams.cpp
        ${AKALSASINK_DIR}/common/src/Logging.cpp
        ${AKALSASINK_DIR}/common/src/WAVFileWriter.cpp
        )

        target_include_directories(ak_alsa_sink
        PUBLIC  
        $<BUILD_INTERFACE:${AKALSASINK_DIR}/pluginInclude>
        $<BUILD_INTERFACE:${AKALSASINK_PLUGIN_DIR}>
        $<BUILD_INTERFACE:${AKALSASINK_DIR}/common/include>
        PRIVATE
        $<BUILD_INTERFACE:${AKALSASINK_DIR}/common/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${WWISE_SDK_ROOT}/include>
        )

        target_link_libraries(ak_alsa_sink
        PRIVATE
        asound
        ${WWISE_LIBS}
        util
        )

        target_compile_options(ak_alsa_sink
        PRIVATE
        #-DAK_ALSA_SINK_LOGS   # Turn on Plugin Logs
        -DAK_LINUX            # We are pretending to be Linux Platform
        -D_POSIX_SOURCE       # Fix ALSA duplicate definition
        -DMATEOS               # Define MATEOS here for wwise compat
        )

        lrya_build_target_license(ak_alsa_sink "LRYA" "Commercial")

        lrya_build_strip(TARGET ak_alsa_sink)

        endif(MATEOS)
else()
        message("Skipping AKALSASINK Plugin")
endif()


#
# audioEngine
#
# if(USE_AUDIO_ENGINE_PLUGIN)

set(AUDIO_ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/engine)

add_library(audio_engine
  ${AUDIO_ENGINE_DIR}/audioCallback.cpp
#   ${AUDIO_ENGINE_DIR}/audioDefaultIOHookBlocking.cpp
  ${AUDIO_ENGINE_DIR}/audioEngineController.cpp
#   ${AUDIO_ENGINE_DIR}/audioFileLocationBase.cpp
#   ${AUDIO_ENGINE_DIR}/audioFilePackage.cpp
#   ${AUDIO_ENGINE_DIR}/audioFilePackageLowLevelIO.inl
#   ${AUDIO_ENGINE_DIR}/audioFilePackageLUT.cpp
  ${AUDIO_ENGINE_DIR}/audioMultipleFileLocation.inl
  ${AUDIO_ENGINE_DIR}/audioScene.cpp
  ${AUDIO_ENGINE_DIR}/musicConductor.cpp
  ${AUDIO_ENGINE_DIR}/soundbankBundleInfo.cpp
  ${AUDIO_ENGINE_DIR}/soundbankLoader.cpp
#   ${AUDIO_ENGINE_DIR}/wwiseComponent.cpp
  ${AUDIO_ENGINE_DIR}/audioTools/audioWaveFileReader.cpp
#   ${AUDIO_ENGINE_DIR}/audioTools/streamingWaveDataInstance.cpp
#   ${AUDIO_ENGINE_DIR}/plugins/akAlsaSinkPlugIn.cpp
  ${AUDIO_ENGINE_DIR}/plugins/lryaPluginInterface.cpp
#   ${AUDIO_ENGINE_DIR}/plugins/hijackAudioPlugIn.cpp
#   ${AUDIO_ENGINE_DIR}/plugins/wavePortalPlugIn.cpp
#   ${AUDIO_ENGINE_DIR}/plugins/streamingWavePortalPlugIn.cpp
)

target_include_directories(audio_engine
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
)

#
# Plugin definitions are defined while building audio engine but
# are not inherited by downstream targets.  If a downstream target
# needs to check for a specific plugin, they can add ${LRYA_LIB_AUDIO_PLUGINS}
# to their definitions.
#
target_compile_definitions(audio_engine
  PRIVATE
  ${LRYA_LIB_AUDIO_PLUGINS}
  ${LRYA_AUDIO_DEFINITIONS}
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

target_compile_options(audio_engine
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

lrya_build_target_license(audio_engine "LRYA" "Commercial")

set(PLATFORM_LIBS "")

if(ANDROID)
  set(PLATFORM_LIBS
      OpenSLES
      android
  )
elseif(MATEOS)
  set(PLATFORM_LIBS
      log
      dl
  )
elseif(MACOSX)
  find_library(CORE_FOUNDATION CoreFoundation)
  find_library(AUDIO_TOOLBOX AudioToolbox)
  find_library(CORE_AUDIO CoreAudio)
  find_library(AUDIO_UNIT AudioUnit)
  set(PLATFORM_LIBS
      ${CORE_FOUNDATION}
      ${AUDIO_TOOLBOX}
      ${CORE_AUDIO}
      ${AUDIO_UNIT}
  )
endif()

set(AUDIO_ENGINE_LIBS
#   streaming_wave_portal
#   wave_portal
#   hijack_audio
#   krotos_vocoder
  clad
  util
  zipreader
  ${PLATFORM_LIBS}
#   ${WWISE_LIBS}
)

# if(MATEOS)
# list(APPEND AUDIO_ENGINE_LIBS
#   ak_alsa_sink
# )
# endif()

target_link_libraries(audio_engine
  PRIVATE
  ${AUDIO_ENGINE_LIBS}
)

lrya_build_strip(TARGET audio_engine)


#
# Audio Multiplexer lib for Engine
#

add_library(audio_multiplexer_engine
  STATIC
  ${AUDIO_ENGINE_DIR}/audioTypeTranslator.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioCladMessageHelper.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMultiplexer.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMuxClient.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMuxInput.cpp
)

target_compile_options(audio_multiplexer_engine
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_compile_definitions(audio_multiplexer_engine
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

target_include_directories(audio_multiplexer_engine
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
)

target_link_libraries(audio_multiplexer_engine
  clad
  util
  audio_engine
  engine_clad
)

lrya_build_target_license(audio_multiplexer_engine "LRYA")

lrya_build_strip(TARGET audio_multiplexer_engine)


#
# Audio Multiplexer lib for Robot
#

add_library(audio_multiplexer_robot
  STATIC
  ${AUDIO_ENGINE_DIR}/audioTypeTranslator.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMultiplexer.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMuxClient.cpp
  ${AUDIO_ENGINE_DIR}/multiplexer/audioMuxInput.cpp
)

target_compile_options(audio_multiplexer_robot
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_compile_definitions(audio_multiplexer_robot
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

target_include_directories(audio_multiplexer_robot
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
)

target_link_libraries(audio_multiplexer_robot
  clad
  util
  audio_engine
  robot_clad
)

lrya_build_target_license(audio_multiplexer_robot "LRYA")

lrya_build_strip(TARGET audio_multiplexer_robot)


#
# Maya-Wwise plugin
#

option(BUILD_MAYA_WWISE_PLUGIN "Build Maya plugin" OFF)
if (BUILD_MAYA_WWISE_PLUGIN)
  if(MACOSX)
    add_subdirectory("mayaWwisePlugin")
  else()
    message(SEND_ERROR "Lrya-Audio: Can only build Maya Plugin for Mac OSX platform!")
  endif()
endif()

#
# Audio Test App
#

option(BUILD_TEST_AUDIO_APP "Build test audio app" OFF)
if (BUILD_TEST_AUDIO_APP)
  add_subdirectory("tools/testAudioApp")
endif()
