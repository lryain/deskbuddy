include(lrya_build_cxx_compile_definitions)
include(lrya_build_cxx_compile_options)
include(lrya_build_source_list)
include(lrya_build_header_file_only_list)
include(lrya_build_data_list)

macro(lrya_build_cxx_library target_name srclist_dir)

lrya_build_source_list(${target_name} ${srclist_dir})

lrya_build_header_file_only_list(${HEADERS})

lrya_build_absolute_data_list(${target_name} ${srclist_dir})

set(extra_argv ${ARGN})
list(LENGTH extra_argv extra_argc)

if (extra_argc GREATER 0)
  # process extra args here
endif()
add_library(${target_name} ${extra_argv}
  ${SRCS}
  ${HEADERS}
  ${_ab_PLATFORM_SRCS}
  ${_ab_PLATFORM_HEADERS}
  ${DATA}
)
if(DATA)
  source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${DATA})
endif()

target_compile_definitions(${target_name}
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

target_compile_options(${target_name}
  PRIVATE
  ${ASAN_CXX_FLAGS} 
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
)

endmacro()

macro(lrya_build_cxx_executable target_name srclist_dir)

lrya_build_source_list(${target_name} ${srclist_dir})

lrya_build_header_file_only_list(${HEADERS})

lrya_build_absolute_data_list(${target_name} ${srclist_dir})

set(extra_argv ${ARGN})
list(LENGTH extra_argv extra_argc)

if (extra_argc GREATER 0)
  # process extra args here
endif()

add_executable(${target_name} ${extra_argv}
  ${SRCS}
  ${HEADERS}
  ${_ab_PLATFORM_SRCS}
  ${_ab_PLATFORM_HEADERS}
  ${DATA}
)

target_compile_definitions(${target_name}
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_DEFINITIONS}
)

# Note: if there are any target_compile_options in project CMakeLists.txt these
#       options will be overwritten

target_compile_options(${target_name}
  PRIVATE
  ${LRYA_BUILD_CXX_COMPILE_OPTIONS}
  ${ASAN_CXX_FLAGS} 
)

endmacro()

macro(lrya_test_build_cxx_executable target_name srclist_dir)

lrya_build_cxx_executable(${target_name} ${srclist_dir})

set_target_properties(${target_name}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/auto-test/bin"
)

endmacro()
