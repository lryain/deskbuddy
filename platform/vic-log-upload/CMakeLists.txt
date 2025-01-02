#
# platform/mate-log-upload/CMakeLists.txt
#
# Standalone application to upload robot log files
#

project(mate-log-upload)

include(lrya_build_cxx)

if (MATEOS)

  lrya_build_cxx_executable(mate-log-upload ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-log-upload "LRYA")

  target_compile_options(mate-log-upload
    PRIVATE
    -Wconversion
    -Wsign-conversion
    -Wshorten-64-to-32
    -Wsign-compare
  )

  target_include_directories(mate-log-upload
    PRIVATE
    ${CMAKE_SOURCE_DIR}
  )

  target_link_libraries(mate-log-upload
    PRIVATE
    robotLogUploader
    deskmateCrashReports
    jsoncpp
    util
  )

  lrya_build_strip(TARGET mate-log-upload)

endif(MATEOS)
