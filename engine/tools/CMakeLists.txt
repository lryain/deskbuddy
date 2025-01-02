cmake_minimum_required(VERSION 3.10)

project(engine_tools)

include(lrya_build_cxx)
include(lrya_build_strip)

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_FLAGS "")

if (MATEOS)
  set(PLATFORM_LIBS log)
elseif (MACOSX)
  find_library(FOUNDATION Foundation)
  set(PLATFORM_LIBS
    ${FOUNDATION}
  )
endif()

lrya_build_cxx_executable(mate-engine ${LRYA_SRCLIST_DIR})
lrya_build_target_license(mate-engine "LRYA")

target_link_libraries(mate-engine
  PRIVATE
  # lrya libs
  util
  cozmo_engine
  deskmateCrashReports
  platform
  ${PLATFORM_LIBS}
  ${ASAN_EXE_LINKER_FLAGS}
)

lrya_build_strip(TARGET mate-engine)
