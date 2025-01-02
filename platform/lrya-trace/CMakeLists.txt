#
# platform/lrya-trace/CMakeLists.txt
#
include (lrya_build_cxx)

if(USE_LRYATRACE AND MATEOS)
  message("---------------------------------USE_LRYATRACE AND MATEOS-------------------------------------")
  lrya_build_cxx_library(lryatrace ${LRYA_SRCLIST_DIR})

  target_include_directories(lryatrace
    PUBLIC
    $<BUILD_INTERFACE:${LTTNGUST_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    )

  target_link_libraries(lryatrace
    LINK_PUBLIC
    dl
    ${LTTNGUST_LIBRARIES}
    )

  lrya_build_strip(TARGET lryatrace)

endif()
