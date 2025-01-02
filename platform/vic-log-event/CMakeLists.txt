#
# platform/mate-log-event/CMakeLists.txt
#
# Standalone application to log a DAS event
#

project(mate-log-event)

include(lrya_build_cxx)

if (MATEOS)

  lrya_build_cxx_executable(mate-log-event ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-log-event "LRYA")

  target_include_directories(mate-log-event
    PRIVATE
    ${CMAKE_SOURCE_DIR}
  )

  target_link_libraries(mate-log-event
    PRIVATE
    util
    ${ASAN_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-log-event)

endif(MATEOS)
