#
# platform/mate-log-kernel-panic/CMakeLists.txt
#
# Standalone application to generate a dummy crash report with
# dmesg attachment.
#

project(mate-log-kernel-panic)

include(lrya_build_cxx)

if (MATEOS)

  lrya_build_cxx_executable(mate-log-kernel-panic ${LRYA_SRCLIST_DIR})

  target_link_libraries(mate-log-kernel-panic
    PRIVATE
    deskmateCrashReports
    util
  )

  lrya_build_strip(TARGET mate-log-kernel-panic)

  lrya_build_target_license(mate-log-kernel-panic "LRYA")

endif(MATEOS)
