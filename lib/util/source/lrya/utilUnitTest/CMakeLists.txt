
project(utilUnitTest LANGUAGES CXX)

include(lrya_build_cxx)

include(gtest)

get_filename_component(util ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ABSOLUTE)

set(UTIL_INCLUDE_DIR ${util})
set(UTIL_LIBRARY util)

set(JSONCPP_INCLUDE_DIR ${util}/source/3rd/jsoncpp)
set(JSONCPP_LIBRARY jsoncpp)

lrya_build_cxx_executable(utilUnitTest ${LRYA_SRCLIST_DIR})
lrya_build_target_license(utilUnitTest "LRYA")

target_compile_definitions(utilUnitTest PRIVATE
  UNIT_TEST=1
)

target_include_directories(utilUnitTest PRIVATE
  ${UTIL_INCLUDE_DIR}
  ${JSONCPP_INCLUDE_DIR}
)

target_link_libraries(utilUnitTest 
  PRIVATE
  util
  ${JSONCPP_LIBRARY}
  gtest
  ${ASAN_LINKER_FLAGS}
)

