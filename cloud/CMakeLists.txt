#
# cloud/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.10)

project(cloud)

include(lrya_build_cxx)
include(lrya_build_go)
include(lrya_build_util)
include(breakpad)
# include(webots)

#
# Gateway Protbuf and GRPC configuration
#

# Grab the gateway files
file(STRINGS "${LRYA_SRCLIST_DIR}/gateway_grpc.srcs.lst" GATEWAY_GRPC_SRCS)
file(STRINGS "${LRYA_SRCLIST_DIR}/gateway_proto.srcs.lst" GATEWAY_PROTO_SRCS)

# exclude grpc file from proto sources
list(FILTER GATEWAY_PROTO_SRCS EXCLUDE REGEX ".*external_interface.proto")

set(GATEWAY_GRPC_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/generated/go/src/proto/external_interface")
set(PROTOBUF_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/generated/proto/external_interface")
set(GATEWAY_PROTO_HOME_DIR "${CMAKE_SOURCE_DIR}/tools/protobuf/gateway")
set(GATEWAY_PROTO_HOME_DIR_RELATIVE "../tools/protobuf/gateway")

# Formulate the generated file names
string(REPLACE ".proto" ".pb.go" GATEWAY_PROTO_GO_GEN_SRCS "${GATEWAY_PROTO_SRCS};${GATEWAY_GRPC_SRCS}")
string(REPLACE ".proto" ".pb.gw.go" GATEWAY_GRPC_GO_GEN_SRCS "${GATEWAY_GRPC_SRCS}")
string(REPLACE ".proto" ".pb.h" GATEWAY_PROTO_CPP_GEN_HEADERS "${GATEWAY_PROTO_SRCS}")
string(REPLACE ".proto" ".pb.cc" GATEWAY_PROTO_CPP_GEN_SRCS "${GATEWAY_PROTO_SRCS}")

# and give those generated files the appropriate pathing
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}/public/" "${GATEWAY_GRPC_OUTPUT_DIR}/" GATEWAY_PROTO_GO_GEN_SRCS "${GATEWAY_PROTO_GO_GEN_SRCS}")
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}/public/" "${GATEWAY_GRPC_OUTPUT_DIR}/" GATEWAY_GRPC_GO_GEN_SRCS "${GATEWAY_GRPC_GO_GEN_SRCS}")
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}/public/" "${PROTOBUF_OUTPUT_DIR}/" GATEWAY_PROTO_CPP_GEN_HEADERS "${GATEWAY_PROTO_CPP_GEN_HEADERS}")
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}/public/" "${PROTOBUF_OUTPUT_DIR}/" GATEWAY_PROTO_CPP_GEN_SRCS "${GATEWAY_PROTO_CPP_GEN_SRCS}")

# Full paths for the proto files
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}" "${GATEWAY_PROTO_HOME_DIR}/" GATEWAY_GRPC_SRCS "${GATEWAY_GRPC_SRCS}")
string(REPLACE "${GATEWAY_PROTO_HOME_DIR_RELATIVE}" "${GATEWAY_PROTO_HOME_DIR}/" GATEWAY_PROTO_SRCS "${GATEWAY_PROTO_SRCS}")

# Expose the necessary files for the engine to build thease into it
set(GATEWAY_PROTO_CPP_GEN_HEADERS "${GATEWAY_PROTO_CPP_GEN_HEADERS}" PARENT_SCOPE)
set(GATEWAY_PROTO_CPP_GEN_SRCS "${GATEWAY_PROTO_CPP_GEN_SRCS}" PARENT_SCOPE)
set(GATEWAY_PROTO_SRCS "${GATEWAY_PROTO_SRCS}" PARENT_SCOPE)
set(PROTOBUF_OUTPUT_DIR "${PROTOBUF_OUTPUT_DIR}" PARENT_SCOPE)
set(GATEWAY_PROTO_HOME_DIR "${GATEWAY_PROTO_HOME_DIR}" PARENT_SCOPE)

#
# Vision Protobuf and GRPC configuration
#

# Grab the vision files
file(STRINGS "${LRYA_SRCLIST_DIR}/vision_proto.srcs.lst" VISION_PROTO_SRCS)

set(VISION_PROTOBUF_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/generated/go/src/proto/vision")
set(VISION_PROTO_HOME_DIR "${CMAKE_SOURCE_DIR}/tools/protobuf/vision")
set(VISION_PROTO_HOME_DIR_RELATIVE "../tools/protobuf/vision")

string(REPLACE ".proto" ".pb.go" VISION_PROTO_GO_GEN_SRCS "${VISION_PROTO_SRCS}")

string(REPLACE "${VISION_PROTO_HOME_DIR_RELATIVE}/private/" "${VISION_PROTOBUF_OUTPUT_DIR}/" VISION_PROTO_GO_GEN_SRCS "${VISION_PROTO_GO_GEN_SRCS}")

string(REPLACE "${VISION_PROTO_HOME_DIR_RELATIVE}" "${VISION_PROTO_HOME_DIR}/" VISION_PROTO_SRCS "${VISION_PROTO_SRCS}")

get_property(VISION_CLAD_SRCS GLOBAL PROPERTY VISION_GO_GEN_SRCS)

# This is where the extra go dependencies get added
set(__extra_go_deps "${CLAD_GO_GEN_SRCS}" "${GATEWAY_PROTO_GO_GEN_SRCS}" "${GATEWAY_GRPC_GO_GEN_SRCS}" "${CLAD_GATEWAY_GEN_SRCS}" "${VISION_CLAD_SRCS}" "${VISION_PROTO_GO_GEN_SRCS}")

# preserving this now-unused option as an example of how to set variables at build times
# hopefully someday this saves someone hours of figuring out how many backslashes are necessary
# set(chipper_flags "-X \\\\\'lrya/voice.ChipperSecret=${chipper_flags}\\\\\'")
# lrya_go_set_ldflags(${target_name} ${chipper_flags})

if(MATEOS)
  set(__opus_build_dir ${LRYA_THIRD_PARTY_DIR}/opus/mateos)
  add_library(opus SHARED IMPORTED)
  lrya_build_target_license(opus "BSD-3,${CMAKE_SOURCE_DIR}/licenses/opus.license")
  set_target_properties(opus PROPERTIES IMPORTED_LOCATION ${__opus_build_dir}/lib/libopus.so.0)
elseif(MACOSX)
  set(__opus_build_dir ${LRYA_THIRD_PARTY_DIR}/opus/mac)
  add_library(opus STATIC IMPORTED)
  lrya_build_target_license(opus "BSD-3,${CMAKE_SOURCE_DIR}/licenses/opus.license")
  set_target_properties(opus PROPERTIES IMPORTED_LOCATION ${__opus_build_dir}/lib/libopus.a)
endif()


macro(_add_chipper_flags target_name)
  lrya_go_add_include_dir(${target_name} ${__opus_build_dir}/include/opus)
  lrya_go_add_c_library(${target_name} opus)
endmacro()

macro(_add_crash_reporter_flags target_name)
  if (MATEOS)
    if(USE_LRYATRACE)
      lrya_go_add_c_library(${target_name} dl)
      lrya_go_add_c_library(${target_name} ${LTTNGUST_LIBRARIES})
      lrya_go_add_c_library(${target_name} ${LRYATRACE})
    endif()
    lrya_go_set_cgo_cppflags(${target_name} -DLRYA_PLATFORM_MATEOS)
    lrya_go_add_include_dir(${target_name} ${CMAKE_SOURCE_DIR})
    lrya_go_add_c_library(${target_name} deskmateCrashReports)
    lrya_go_add_c_library(${target_name} breakpad_client)
    lrya_go_add_c_library(${target_name} lryautil)
    lrya_go_add_c_library(${target_name} log)
    #
    # Our build macros don't handle the case where a library (util) has a custom output name (lryautil).
    # Add an explicit dependency to ensure that the library is built before our target.
    add_dependencies(${target_name} util)
  endif()
endmacro()

macro(_build_cloud_executable target_name)
  lrya_build_go_executable(${target_name} ${LRYA_SRCLIST_DIR} "${__extra_go_deps}")
  get_target_property(__util_include_dirs util INCLUDE_DIRECTORIES)
  lrya_go_add_include_dir(${target_name} ${__util_include_dirs})
endmacro()

macro(_build_cloud_c_library target_name header_name)
  lrya_build_go_c_library(${target_name} ${header_name} ${LRYA_SRCLIST_DIR} "${__extra_go_deps}")
endmacro()

# voicetest project (mac only)
if (MACOSX)

  # create Go target
  _build_cloud_c_library(voicego VOICEGO_HEADER)
  lrya_build_target_license(voicego "LRYA")
  _add_chipper_flags(voicego)
  get_filename_component(VOICEGO_HEADER_DIR ${VOICEGO_HEADER} DIRECTORY)

  # create C target
  lrya_build_cxx_executable(voicetest ${LRYA_SRCLIST_DIR} ${VOICEGO_HEADER})
  lrya_build_target_license(voicetest "LRYA")
  lrya_disable_default_build(voicetest)

  # tell voicetest where to find voicego.h
  target_include_directories(voicetest
    PUBLIC
    ${VOICEGO_HEADER_DIR}
  )

  # need security framework too (SAI dependencies??)
  find_library(SECURITY_FRAMEWORK Security)
  if(NOT SECURITY_FRAMEWORK)
    message(FATAL_ERROR "Security not found")
  endif()

  # link audio and go libraries
  target_link_libraries(voicetest
    PRIVATE
    util_audio
    voicego
    opus
    ${SECURITY_FRAMEWORK}
  )


  # proctest - test deskmate audio process w/ laptop mic
  _build_cloud_c_library(proctest_go PROCTEST_GO_HEADER)
  lrya_build_target_license(proctest_go "LRYA")
  _add_chipper_flags(proctest_go)
  get_filename_component(PROCTEST_HEADER_DIR ${PROCTEST_GO_HEADER} DIRECTORY)
  lrya_build_cxx_executable(proctest ${LRYA_SRCLIST_DIR} ${PROCTEST_GO_HEADER})
  lrya_build_target_license(proctest "LRYA")
  target_include_directories(proctest PUBLIC ${PROCTEST_HEADER_DIR})
  target_compile_definitions(proctest PUBLIC "HEADER_NAME=\"proctest_go.h\"")
  target_link_libraries(proctest PRIVATE util_audio proctest_go ${SECURITY_FRAMEWORK})
  lrya_disable_default_build(proctest)

  _build_cloud_executable(opusupload)
  _add_chipper_flags(opusupload)

  _build_cloud_executable(sessiondl)
  lrya_disable_default_build(sessiondl)

  _build_cloud_executable(tokprovision)
  lrya_build_target_license(tokprovision "LRYA")

  # unit tests
  lrya_go_add_test_dir("lrya" ${LRYA_SRCLIST_DIR} LRYA_GO_TEST_TARGETS "${CLAD_GO_GEN_SRCS}")
  foreach(i ${LRYA_GO_TEST_TARGETS})
    lrya_go_add_c_library(${i} util)
    _add_chipper_flags(${i})
  endforeach(i)

endif()

# CLI tool for testing logcollector (log uploads)
_build_cloud_executable(logcollectorcli)
lrya_disable_default_build(logcollectorcli)

# ipctest - for testing interop capability of C++ and Go socket code
_build_cloud_executable(ipctest_go)
lrya_disable_default_build(ipctest_go)
lrya_build_cxx_executable(ipctest_cpp ${LRYA_SRCLIST_DIR})
lrya_build_target_license(ipctest_cpp "LRYA")
target_link_libraries(ipctest_cpp PRIVATE cti_messaging)
lrya_disable_default_build(ipctest_cpp)

# ipc server test things
_build_cloud_executable(servbench)
lrya_disable_default_build(servbench)

# cloud process
# _build_cloud_executable(mate-cloud)

# see also VIC-6541 Solve keeping list of approved open-source licenses up-to-date with the license checks

# lrya_build_target_license(mate-cloud
#   "LRYA"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aws/aws-sdk-go/vendor/github.com/go-ini/ini/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aws/aws-sdk-go/vendor/github.com/jmespath/go-jmespath/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/golang/glog/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/google/martian/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/google.golang.org/genproto/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/google.golang.org/grpc/LICENSE"
#   "BSD-2,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aalpern/go-metrics/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/google/uuid/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gorilla/context/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/grd/ogg/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/golang.org/x/text/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/golang.org/x/time/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/lrya/opus-go/libopus/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/cenkalti/backoff/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/dgrijalva/jwt-go/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gwatts/iniconf/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gwatts/rootcerts/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/jawher/mow.cli/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/satori/go.uuid/LICENSE"
# )

# _add_chipper_flags(mate-cloud)
# _add_crash_reporter_flags(mate-cloud)

# copy opusfile to lib dir
if (MATEOS)
  configure_file(${__opus_build_dir}/lib/libopus.so.0 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libopus.so.0 COPYONLY)
endif()

#
# wavtest
#
_build_cloud_executable(wavtest)
lrya_build_target_license(wavtest "LRYA")
_add_chipper_flags(wavtest)

if (MATEOS)
  lrya_disable_default_build(wavtest)
endif()

#
# opusenc
#
_build_cloud_executable(opusenc)
_add_chipper_flags(opusenc)
lrya_disable_default_build(opusenc)

#
# mate-vision
#
# Generate grpc go files for offboard vision
#

set(GOPATH_VISION "${CMAKE_CURRENT_SOURCE_DIR}/go")
# Generate grpc go files
add_custom_command(
  OUTPUT  ${VISION_PROTO_GO_GEN_SRCS}
  COMMAND mkdir -p ${VISION_PROTOBUF_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E env "PATH=${GOPATH_VISION}/bin:$ENV{PATH}"
    ${PROTOBUF_HOME}/bin/protoc "-I${VISION_PROTO_HOME_DIR}/private"
    "-I${PROTOBUF_HOME}/include"
    "-I${GOPATH_VISION}/src"
    "--go_out=plugins=grpc:${VISION_PROTOBUF_OUTPUT_DIR}"
    ${VISION_PROTO_SRCS}
  DEPENDS ${VISION_PROTO_SRCS}
)

#
# mate-gateway
#
# SDK translation service that interfaces between the engine and the external world.
#

set(GOPATH_GATEWAY "${CMAKE_CURRENT_SOURCE_DIR}/go")

add_custom_command(
  OUTPUT ${GATEWAY_PROTO_GO_GEN_SRCS} ${GATEWAY_GRPC_GO_GEN_SRCS}
  COMMAND mkdir -p ${GATEWAY_GRPC_OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E env "PATH=${GOPATH_GATEWAY}/bin:$ENV{PATH}"
    ${PROTOBUF_HOME}/bin/protoc "-I${GATEWAY_PROTO_HOME_DIR}/public"
    "-I${PROTOBUF_HOME}/include"
    "-I${GOPATH_GATEWAY}/src"
    "-I${GOPATH_GATEWAY}/src/github.com/grpc-ecosystem/grpc-gateway/third_party/googleapis"
    "--go_out=plugins=grpc:${GATEWAY_GRPC_OUTPUT_DIR}"
    ${GATEWAY_GRPC_SRCS} ${GATEWAY_PROTO_SRCS}
  COMMAND ${CMAKE_COMMAND} -E env "PATH=${GOPATH_GATEWAY}/bin:$ENV{PATH}"
    ${PROTOBUF_HOME}/bin/protoc "-I${GATEWAY_PROTO_HOME_DIR}/public"
    "-I${PROTOBUF_HOME}/include"
    "-I${GOPATH_GATEWAY}/src"
    "-I${GOPATH_GATEWAY}/src/github.com/grpc-ecosystem/grpc-gateway/third_party/googleapis"
    "--grpc-gateway_out=logtostderr=true:${GATEWAY_GRPC_OUTPUT_DIR}"
    ${GATEWAY_GRPC_SRCS} ${GATEWAY_PROTO_SRCS}
  DEPENDS ${GATEWAY_GRPC_SRCS} ${GATEWAY_PROTO_SRCS}
)

# _build_cloud_executable(mate-gateway)
#_add_chipper_flags(mate-gateway)
#lrya_go_add_c_library(mate-gateway robotLogUploader)
#_add_crash_reporter_flags(mate-gateway)

# see also VIC-6541 Solve keeping list of approved open-source licenses up-to-date with the license checks

# lrya_build_target_license(mate-gateway
#   "LRYA"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aws/aws-sdk-go/vendor/github.com/go-ini/ini/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aws/aws-sdk-go/vendor/github.com/jmespath/go-jmespath/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/golang/glog/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/google/martian/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/google.golang.org/genproto/LICENSE"
#   "Apache-2.0,${CMAKE_SOURCE_DIR}/cloud/go/src/google.golang.org/grpc/LICENSE"
#   "BSD-2,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/aalpern/go-metrics/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/google/uuid/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gorilla/context/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/grd/ogg/LICENSE"
#   "BSD-3,${CMAKE_SOURCE_DIR}/cloud/go/src/golang.org/x/text/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/lrya/opus-go/libopus/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/cenkalti/backoff/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/dgrijalva/jwt-go/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gwatts/iniconf/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/gwatts/rootcerts/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/jawher/mow.cli/LICENSE"
#   "MIT,${CMAKE_SOURCE_DIR}/cloud/go/src/github.com/satori/go.uuid/LICENSE"
# )

# Install certs for mac build of mate-gateway
#if (MACOSX)

#  set(GATEWAY_MAC_CERTS
#    "/tmp/lrya/gateway/trust.cert"
#    "/tmp/lrya/gateway/trust.key"
#  )

#  add_custom_target(
#    GenerateGatewayMacCerts ALL
#    DEPENDS ${GATEWAY_MAC_CERTS}
#  )

#  add_custom_command(
#    OUTPUT ${GATEWAY_MAC_CERTS}
#    COMMAND ./mac_setup.sh
#    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/sdk/scripts"
#    COMMENT "Creating mac certs for mate-gateway"
#  )

  #if (WEBOTS_SETUP_FIREWALL)
  #  webots_setup_exe(mate-gateway-firewall ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mate-gateway)
  #  add_dependencies(mate-gateway-firewall mate-gateway)
  #endif()

#endif()
