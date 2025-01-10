cmake_minimum_required(VERSION 3.6)

project(animProcess)

include(lrya_build_cxx)
include(aubio)
include(flatbuffers)
include(opencv)
# include(text2speech)
# include(pryon_lite)
include(robot_core)

lrya_build_cxx_library(mate_anim ${LRYA_SRCLIST_DIR} STATIC)
lrya_build_target_license(mate_anim "LRYA" 
                          "Public Domain,${CMAKE_SOURCE_DIR}/lib/util/source/3rd/gif-h/LICENSE" 
                          "Public Domain,${CMAKE_SOURCE_DIR}/lib/util/source/3rd/jo_gif/jo_gif.h" 
                          "FFTPACK5,${CMAKE_SOURCE_DIR}/licenses/pffft.license")
set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES ${OPENCV_INCLUDE_PATHS})
set(PLATFORM_COMPILE_DEFS "")

option(ALEXA_ACOUSTIC_TEST "Enable alexa acoustic testing (no movement mode)" OFF)

if (MATEOS)
    set(PLATFORM_LIBS
        log
        robot_core)
elseif (MACOSX)
    include(webots)
    set(PLATFORM_LIBS
        ${OPENCV_LIBS}
        ${WEBOTS_LIBS}
    )
    set(PLATFORM_COMPILE_DEFS "-DSIMULATOR")
endif()

target_link_libraries(mate_anim
  PUBLIC
  mate_web_library
  robot_interface  # Needs to be public for cozmoConfig.h
  PRIVATE
  util
  util_audio
  canned_anim_lib_anim
  # cti
  cti_common_robot
  cti_vision
  cti_messaging_robot
  robot_clad_cpplite
  cloud_clad_cpp
  audio_engine
  audio_multiplexer_robot
  osState
  micdata
  libev
  # vendor
  ${OPENCV_LIBS}
  ${FLATBUFFERS_LIBS}
#   signal_essence2
#   ${TEXT2SPEECH_LIBS}
#   ${PRYON_LITE_LIBS}
  ${SPEEXDSP_LIBS}
  # platform
  ${PLATFORM_LIBS}
  # Aubio beat detection
  ${AUBIO_LIBS}
  ${AVS_LIBS}
  ${PFFFT_LIBS}
  ${MPG123_LIBS}
  PUBLIC
  mate_web_library
  robot_interface  # Needs to be public for cozmoConfig.h
#   ${ASAN_LINKER_FLAGS}
)

lrya_build_strip(TARGET mate_anim)

target_compile_definitions(mate_anim
  PRIVATE
  ${PLATFORM_COMPILE_DEFS}
  ALEXA_ACOUSTIC_TEST=$<BOOL:${ALEXA_ACOUSTIC_TEST}>
)

target_include_directories(mate_anim
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/util/source/3rd>
  ${PLATFORM_INCLUDES}
)

# mate_anim binary only builds on mateos now
# mac implementation uses (webotsCtrlAnim)
if (MATEOS)

  lrya_build_cxx_executable(mate-anim ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-anim "LRYA" "Commercial,${CMAKE_SOURCE_DIR}/licenses/acapela.license")

  target_link_libraries(mate-anim
    PRIVATE
    mate_anim
    deskmateCrashReports
    # NOTE: deliberately using non-robot cti_common here to ease build
    #       pain given differing include paths for clad types. This still
    #       does not expose cpplite clad messaging within the anim process
    #       (which is undesirable) since this is limited just to the
    #       mate-anim executable (vs. using cti_common_robot and thus
    #       cpplite clad in the mate_anim library above). Ideally this
    #       separation would be clearer and cleaner or we would
    #       exclusively use one or the other, but that will have to
    #       wait for a (much) larger cleanup to cmake / clad / coretech.
    cti_common
    jsoncpp
    ${LRYATRACE}
    ${PLATFORM_LIBS}
    ${ASAN_EXE_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-anim)


  project(mate-bootAnim)

  lrya_build_cxx_executable(mate-bootAnim ${LRYA_SRCLIST_DIR})

  target_compile_definitions(mate-bootAnim
    PRIVATE
    ${PLATFORM_COMPILE_DEFS}
  )

  target_link_libraries(mate-bootAnim
    PRIVATE
    robot_core
#     ${ASAN_EXE_LINKER_FLAGS}
#     pigpio
    util
    PUBLIC
    gpio
# spi_lcd
    robot_interface  # Needs to be public for cozmoConfig.h
  )

  lrya_build_strip(TARGET mate-bootAnim)

  lrya_build_target_license(mate-bootAnim "LRYA")

endif()
