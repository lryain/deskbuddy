cmake_minimum_required(VERSION 3.10)

if (MATEOS)

project(lrya-ble)

include(lrya_build_cxx)

lrya_build_cxx_library(lrya-ble ${LRYA_SRCLIST_DIR})
lrya_build_target_license(lrya-ble "LRYA")

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")

set(PLATFORM_LIBS
  log
)

target_link_libraries(lrya-ble
PRIVATE
  ${PLATFORM_LIBS}
PUBLIC
  libev
  ${ASAN_SHARED_LINKER_FLAGS}
)

target_compile_definitions(lrya-ble
  PRIVATE
  ${PLATFORM_COMPILE_DEFS}
  USE_ANDROID_LOGGING=1
)

target_include_directories(lrya-ble
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  PRIVATE
  ${PLATFORM_INCLUDES}
)

lrya_build_strip(TARGET lrya-ble)

endif(MATEOS)
