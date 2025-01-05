#!/usr/bin/env bash

set -e
set -u

: ${PROTOBUF_REVISION_TO_BUILD:="v1.11-anki-r1"}

echo "Building protobuf ${PROTOBUF_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       avs-device-sdk \
       git@github.com:lryain/avs-device-sdk.git \
       ${PROTOBUF_REVISION_TO_BUILD}

cd ${BUILDDIR}/avs-device-sdk

# Common build settings
CMAKE_C_FLAGS="-O3 -DNDEBUG -fvisibility=hidden -ffunction-sections -fstack-protector-all -Wno-error"
CMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -fvisibility-inlines-hidden"

# Build for mateos
echo "Building avs-device-sdk for deskmate (mateos) ...."
# rm -rf cmake/build/release
mkdir -p cmake/build/release
pushd cmake/build/release

# 编译器指定为clang
${CMAKE_EXE} \
  -DCMAKE_TOOLCHAIN_FILE=/home/pi/deskbuddy/cmake/mateos.raspi.toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
  -DCMAKE_C_FLAGS_RELEASE="${CMAKE_C_FLAGS}" \
  -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS}" \
  -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mateos \
  ../..

make -j8 install
popd

${MAKE_DEP_ARCHIVE_SH} protobuf ${PROTOBUF_REVISION_TO_BUILD}

# rm -rf ${DISTDIR}
