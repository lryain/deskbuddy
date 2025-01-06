cmake_minimum_required(VERSION 3.10)

project(deskmateCrashReports)

#
# Build options
#
option(USE_GOOGLE_BREAKPAD "Enable Google Breakpad crash reports" OFF)
option(USE_TOMBSTONE_HOOKS "Enable Android tombstone hooks" ON)

message(STATUS "---------> USE_GOOGLE_BREAKPAD is ${USE_GOOGLE_BREAKPAD}")
message(STATUS "USE_TOMBSTONE_HOOKS is ${USE_TOMBSTONE_HOOKS}")

include(breakpad)
include(lrya_build_cxx)
include(lrya_build_strip)

if (MATEOS)

  lrya_build_cxx_library(deskmateCrashReports ${LRYA_SRCLIST_DIR} STATIC)
  lrya_build_target_license(deskmateCrashReports "LRYA")

  target_include_directories(deskmateCrashReports
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/util/source/lrya>
  )

  if (USE_GOOGLE_BREAKPAD)
    target_compile_definitions(deskmateCrashReports
      PRIVATE
      USE_GOOGLE_BREAKPAD
    )
    target_include_directories(deskmateCrashReports
      PRIVATE
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/crash-reporting-mateos/Breakpad/include/breakpad>
    )
    target_link_libraries(deskmateCrashReports
      PRIVATE
      util
      breakpad_client
    )
  endif()

  if (USE_TOMBSTONE_HOOKS)
    target_compile_definitions(deskmateCrashReports
      PRIVATE
      USE_TOMBSTONE_HOOKS
    )
    target_link_libraries(deskmateCrashReports
      PRIVATE
      util
      cutils
    )
  endif()

  lrya_build_strip(TARGET deskmateCrashReports)

endif()
