cmake_minimum_required(VERSION 3.10)

if (MATEOS)

project(mate-switchboard)

include(lrya_build_cxx)
include(sodium)

lrya_build_cxx_library(switchboard ${LRYA_SRCLIST_DIR} STATIC)
lrya_build_target_license(switchboard "LRYA" "CC0,${CMAKE_SOURCE_DIR}/licenses/simple-signal.license")

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")

set(PLATFORM_LIBS
  log
  c
)

target_link_libraries(switchboard
PRIVATE
  ${PLATFORM_LIBS}
  sdk_clad_cpp
  robot_clad
  cti_messaging_shared
  engine_clad
  ${SODIUM_LIBS}
  jsoncpp
  libglib-2.0.so
  libgio-2.0.so
  libgobject-2.0.so
  libffi.so.6
  libdl.so.2
  libz.so.1
  libresolv.so.2
  libgmodule-2.0.so.0
  libpcre.so.1
  libcutils.so.0
  util
  deskmateCrashReports
  lrya-ble
  osState
)

target_compile_options(switchboard
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_compile_definitions(switchboard
  PRIVATE
  ${PLATFORM_COMPILE_DEFS}
  LRYA_SWITCHBOARD_CLOUD_AUTH=1
)

target_include_directories(switchboard
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/switchboardd>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bleClient>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cutils>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lrya-wifi>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/signals>
  $<BUILD_INTERFACE:${LIBSODIUM_INCLUDE_PATH}>
  $<BUILD_INTERFACE:${MATEOS_SDK}/sysroot/usr/include/gio-unix-2.0>
  $<BUILD_INTERFACE:${MATEOS_SDK}/sysroot/usr/include/glib-2.0>
  $<BUILD_INTERFACE:${MATEOS_SDK}/sysroot/usr/lib/glib-2.0/include>
  ${PLATFORM_INCLUDES}
)

lrya_build_cxx_executable(mate-switchboard ${LRYA_SRCLIST_DIR})
lrya_build_target_license(mate-switchboard "LRYA")

target_compile_options(mate-switchboard
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_link_libraries(mate-switchboard
  PRIVATE
  switchboard
  ${PLATFORM_LIBS}
  sdk_clad_cpp
  robot_clad
  cti_messaging_shared
  engine_clad
  ${SODIUM_LIBS}
  jsoncpp
  libglib-2.0.so
  libgio-2.0.so
  libgobject-2.0.so
  libffi.so.6
  libdl.so.2
  libz.so.1
  libresolv.so.2
  libgmodule-2.0.so.0
  libpcre.so.1
  libcutils.so.0
  util
  deskmateCrashReports
  lrya-ble
)

lrya_build_strip(TARGET mate-switchboard)

lrya_build_cxx_executable(mate-rescue ${LRYA_SRCLIST_DIR})
lrya_build_target_license(mate-rescue "LRYA")

include(robot_core)

target_compile_options(mate-rescue
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_link_libraries(mate-rescue
  PRIVATE
  switchboard
  robot_interface
  robot_core
  cti_common
  cti_vision 
  ${PLATFORM_LIBS}
  sdk_clad_cpp
  robot_clad
  cti_messaging_shared
  engine_clad
  ${SODIUM_LIBS}
  jsoncpp
  libglib-2.0.so
  libgio-2.0.so
  libgobject-2.0.so
  libffi.so.6
  libdl.so.2
  libz.so.1
  libresolv.so.2
  libgmodule-2.0.so.0
  libpcre.so.1
  libcutils.so.0
  util
  deskmateCrashReports
  lrya-ble
)

lrya_build_strip(TARGET mate-rescue)

endif(MATEOS)
