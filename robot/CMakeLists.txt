
cmake_minimum_required(VERSION 3.10)

project(robot)

include(lrya_build_cxx)
include(lrya_build_strip)

add_library(robot_interface INTERFACE IMPORTED GLOBAL)
set_property(TARGET robot_interface
             PROPERTY INTERFACE_INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(clad)
# add_subdirectory(spi_lcd)

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")
set(PLATFORM_FLAGS "")
if (MACOSX)
  find_library(FOUNDATION Foundation)
  set(PLATFORM_COMPILE_DEFS
    SIMULATOR
  )
endif()

if (MATEOS)

add_library(robot_hal STATIC
  ${CMAKE_SOURCE_DIR}/robot/hal/src/hal.cpp
  ${CMAKE_SOURCE_DIR}/robot/hal/src/hal_motors.cpp
)

target_link_libraries(robot_hal
  PRIVATE
  cti_common_robot
  robot_interface
  robot_clad_cpplite
  hdr_histogram_static
  log
  ${ASAN_LINKER_FLAGS}
)

target_include_directories(robot_hal
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/syscon>
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/hal/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/hal/sim/include>
)

target_compile_options(robot_hal
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
  ${ASAN_CXX_FLAGS} 
)

target_compile_definitions(robot_hal
  PRIVATE
  COZMO_ROBOT
  ${PLATFORM_COMPILE_DEFS}
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
  #HAL_DUMMY_BODY # Uncomment if deploying on head only
)

lrya_build_target_license(robot_hal "LRYA")

endif()

# Create a lib for cube firmware led animation code, so
# we can link into it from webotsCtrlLightCube
if (MACOSX)
add_library(cube_led_animator STATIC
  ${CMAKE_SOURCE_DIR}/robot/cube_firmware/app/animation.c
)

target_compile_options(cube_led_animator
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

target_compile_definitions(cube_led_animator
  PRIVATE
  ${PLATFORM_COMPILE_DEFS}
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

lrya_build_target_license(cube_led_animator "LRYA")
endif()

set(PLATFORM_SRCS "")
set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")
set(PLATFORM_FLAGS "")
if (MATEOS)
  set(PLATFORM_LIBS
    robot_hal
  )
elseif (MACOSX)
  include(webots)

  set(PLATFORM_LIBS
      util
      util_audio
      cti_messaging
      ${WEBOTS_LIBS}
  )
  set(PLATFORM_INCLUDES
      # FIXME gross: ("simulator/robot/sim_overlayDisplay.h")
      ${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/robot/hal/include
      ${CMAKE_SOURCE_DIR}/robot/hal/sim/include
  )
  set(PLATFORM_COMPILE_DEFS
      SIMULATOR
  )
  set(PLATFORM_SRCS
      ${CMAKE_SOURCE_DIR}/robot/hal/sim/src/sim_DAS.cpp
      ${CMAKE_SOURCE_DIR}/robot/hal/sim/src/sim_hal.cpp
      ${CMAKE_SOURCE_DIR}/robot/hal/src/radio.cpp
      ${CMAKE_SOURCE_DIR}/simulator/robot/sim_overlayDisplay.cpp
  )
endif()

#
# displayFaultCode
#
add_executable(displayFaultCode
  ${CMAKE_SOURCE_DIR}/robot/include/lrya/cozmo/shared/factory/faultCodeMain.cpp
)

target_compile_options(displayFaultCode
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
  ${ASAN_CXX_FLAGS}
)

target_compile_definitions(displayFaultCode PRIVATE ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS})

target_include_directories(displayFaultCode
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

target_link_libraries(displayFaultCode
  PRIVATE
  ${LRYATRACE}
  ${ASAN_EXE_LINKER_FLAGS}
)

lrya_build_strip(TARGET displayFaultCode)

lrya_build_target_license(displayFaultCode "LRYA")

#
# supervisor
#
lrya_build_cxx_library(supervisor ${LRYA_SRCLIST_DIR} STATIC
  ${PLATFORM_SRCS}
)
lrya_build_target_license(supervisor "LRYA")

target_link_libraries(supervisor
  PRIVATE
  robot_interface
  robot_clad_cpplite
  cti_common_robot
  cti_planning_robot
  ${PLATFORM_LIBS}
)

target_compile_definitions(supervisor
  PRIVATE
  COZMO_ROBOT
  CORETECH_ROBOT
  ${PLATFORM_COMPILE_DEFS}
)

target_include_directories(supervisor
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/supervisor/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC
  ${PLATFORM_INCLUDES}
)

set(PLATFORM_LIBS "")
set(PLATFORM_INCLUDES "")
set(PLATFORM_COMPILE_DEFS "")
set(PLATFORM_FLAGS "")
if (MATEOS)
  set(PLATFORM_LIBS log)
elseif (MACOSX)
  find_library(FOUNDATION Foundation)
  set(PLATFORM_LIBS
    ${FOUNDATION}
  )
  set(PLATFORM_COMPILE_DEFS
    SIMULATOR
  )
endif()

#
# spine
#
if (MATEOS)

  lrya_build_cxx_library(spine ${LRYA_SRCLIST_DIR} STATIC)
  lrya_build_target_license(spine "LRYA")

  target_include_directories(spine
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/clad/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/syscon>
  )

  target_link_libraries(spine
    PRIVATE
    robot_hal
    ${PLATFORM_LIBS}
  )

endif()

#
# robot_supervisor
#
if (MATEOS)

  lrya_build_cxx_executable(mate-robot ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-robot "LRYA")

  target_compile_definitions(mate-robot
    PRIVATE
    COZMO_ROBOT
    CORETECH_ROBOT
    ${PLATFORM_COMPILE_DEFS}
    ${LRYATRACE}
    #HAL_DUMMY_BODY # Uncomment if deploying on head only
  )

  target_include_directories(mate-robot
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/hal>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/hal/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/robot/syscon>
  )

  target_link_libraries(mate-robot
    PRIVATE
    robot_hal
    supervisor
    spine
    robot_interface
    robot_clad_cpplite
    cti_common_robot
    cti_messaging_robot
    deskmateCrashReports
    ${LRYATRACE}
    ${PLATFORM_LIBS}
    ${ASAN_EXE_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-robot)

endif()
