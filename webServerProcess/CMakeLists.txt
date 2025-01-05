cmake_minimum_required(VERSION 3.6)

project(webServerProcess)

# Build options
option(USE_DAS "Enable DAS reporting" OFF)

include(lrya_build_cxx)

#
# mate_web_library
#

lrya_build_cxx_library(mate_web_library ${LRYA_SRCLIST_DIR})
lrya_build_target_license(mate_web_library "LRYA")

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES ${OPENCV_INCLUDE_PATHS})
set(PLATFORM_COMPILE_DEFS "")

if (MATEOS)
    set(PLATFORM_LIBS
        log
    )
elseif (MACOSX)
    include(webots)
    set(PLATFORM_LIBS
        ${WEBOTS_LIBS}
    )
    set(PLATFORM_COMPILE_DEFS "-DSIMULATOR")
endif()

target_link_libraries(mate_web_library
  PRIVATE
  cti_vision
  DAS
  osState
  PUBLIC
  # vendor
  civetweb
  robot_interface  # Needs to be public for cozmoConfig.h
)

lrya_build_strip(TARGET mate_web_library)

target_compile_definitions(mate_web_library
  PRIVATE
  USE_DAS=$<BOOL:${USE_DAS}>
  ${PLATFORM_COMPILE_DEFS}
)

target_include_directories(mate_web_library
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # allow "android/" prefix
  ${PLATFORM_INCLUDES}
)

# mate_web_server binary only builds on MATEOS now
# mac implementation uses (webotsCtrlWebServer)
if (MATEOS AND NOT LRYA_NO_WEBSERVER_ENABLED)

  lrya_build_cxx_executable(mate-webserver ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-webserver "LRYA")

  target_link_libraries(mate-webserver
    PRIVATE
    mate_web_library
    cti_common
    deskmateCrashReports
    ${PLATFORM_LIBS}
    ${ASAN_EXE_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-webserver)

endif()
