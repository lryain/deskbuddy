cmake_minimum_required(VERSION 3.10)

project(mate-faultCodeDisplay)

include(lrya_build_cxx)
include(robot_core)

if (MATEOS)

  lrya_build_cxx_executable(mate-faultCodeDisplay ${LRYA_SRCLIST_DIR})

  target_link_libraries(mate-faultCodeDisplay
    PUBLIC
    robot_interface
    robot_core
    cti_common
    cti_vision
    ${LRYATRACE}
    ${ASAN_EXE_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-faultCodeDisplay)

endif(MATEOS)
