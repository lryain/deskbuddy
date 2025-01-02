#
# platform/mate-log-forward/CMakeLists.txt
#
# Standalone application to forward DAS events from text file to log system
#

project(mate-log-forward)

include(lrya_build_cxx)

if (MATEOS)

  lrya_build_cxx_executable(mate-log-forward ${LRYA_SRCLIST_DIR})

  lrya_build_target_license(mate-log-forward "LRYA")

  target_include_directories(mate-log-forward
    PRIVATE
    ${CMAKE_SOURCE_DIR}
  )

  target_link_libraries(mate-log-forward
    PRIVATE
    util
  )

  lrya_build_strip(TARGET mate-log-forward)

endif(MATEOS)
