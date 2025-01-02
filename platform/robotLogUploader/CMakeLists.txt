#
# platform/robotLogUploader/CMakeLists.txt
#
# cmake directives for robotLogUploader
#

project(robotLogUploader)

include(lrya_build_cxx)

lrya_build_cxx_library(robotLogUploader ${LRYA_SRCLIST_DIR})
lrya_build_target_license(robotLogUploader "LRYA")

target_compile_options(robotLogUploader
  PRIVATE
  -Wconversion
  -Wsign-conversion
  -Wshorten-64-to-32
  -Wsign-compare
)

target_include_directories(robotLogUploader
  PUBLIC
  ${CMAKE_SOURCE_DIR}
)

target_link_libraries(robotLogUploader
  PRIVATE
  cloud_clad_cpp
  cti_messaging
  cti_common
  util
)

lrya_build_strip(TARGET robotLogUploader)
