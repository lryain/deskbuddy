#
# platform/dasmgr/CMakeLists.txt
#

project(mate-dasmgr)

include(lrya_build_cxx)

if (MATEOS)

  lrya_build_cxx_executable(mate-dasmgr ${LRYA_SRCLIST_DIR})
  lrya_build_target_license(mate-dasmgr "LRYA")

  # Note: overrides target_compile_options from lrya_build_cxx_executable
  
  target_compile_options(mate-dasmgr
    PRIVATE
    -Wconversion -Wsign-conversion -Wshorten-64-to-32 -Wsign-compare
    ${ASAN_CXX_FLAGS} 
  )

  target_include_directories(mate-dasmgr
    PRIVATE
    ${CMAKE_SOURCE_DIR}
  )

  target_link_libraries(mate-dasmgr
    PRIVATE
    deskmateCrashReports
    osState
    DAS
    cti_common
    util
    ${ASAN_EXE_LINKER_FLAGS}
  )

  lrya_build_strip(TARGET mate-dasmgr)

endif(MATEOS)
