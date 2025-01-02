cmake_minimum_required(VERSION 3.10)

include(lrya_build_cxx)

project(cameraService)

if (MATEOS)

add_library(camera_client STATIC
    mateos/camera_client/camera_client.h
    mateos/camera_client/camera_client.c
    mateos/camera_client/log.h
    mateos/camera_client/log.c
)

target_include_directories(camera_client
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  SYSTEM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mateos/camera_client/inc>
)

target_link_libraries(camera_client
  PRIVATE
  gpio
)

target_compile_definitions(camera_client
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
  USE_ION
  USE_ANDROID_LOGGING=1

)

target_compile_options(camera_client
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
  -std=c11 # needed for stdatomic.h
)

set_target_properties(camera_client PROPERTIES LINKER_LANGUAGE C)
lrya_build_target_license(camera_client "LRYA")

endif()



lrya_build_cxx_library(cameraService ${LRYA_SRCLIST_DIR})
lrya_build_target_license(cameraService "LRYA")

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")
if (MATEOS)
    set(PLATFORM_LIBS
        log
        camera_client)
elseif (MACOSX)
    include(webots)
    set(PLATFORM_LIBS
        webotsCtrlCommon
        ${OPENCV_LIBS}
        ${WEBOTS_LIBS}
    )
    set(PLATFORM_COMPILE_DEFS "-DSIMULATOR")
endif()

target_link_libraries(cameraService
PRIVATE
  util
  cti_common
  cti_vision
  engine_clad # imageTypes.h
  robot_interface # cozmoConfig.h
  ${PLATFORM_LIBS}
  ${ASAN_LINKER_FLAGS}
)

lrya_build_strip(TARGET cameraService)

target_compile_definitions(cameraService
  PRIVATE
  ${PLATFORM_COMPILE_DEFS}
)

target_include_directories(cameraService
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..> # allow "mateos/" prefix
  ${PLATFORM_INCLUDES}
)
