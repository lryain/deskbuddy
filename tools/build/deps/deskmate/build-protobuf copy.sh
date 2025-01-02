#!/usr/bin/env bash

set -e
set -u

# : ${PROTOBUF_REVISION_TO_BUILD:="v3.5.1"}
: ${PROTOBUF_REVISION_TO_BUILD:="v3.6.1"}

echo "Building protobuf ${PROTOBUF_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       protobuf \
       https://github.com/protocolbuffers/protobuf.git \
       ${PROTOBUF_REVISION_TO_BUILD}
BUILDDIR=/home/lryain/deskmate-dev/tools/build/build-deskmate-deps
PROTOBUF_DIR=${BUILDDIR}/build-protobuf/protobuf
echo "PROTOBUF_DIR: ${PROTOBUF_DIR}"
# cd ${BUILDDIR}/protobuf
cd $PROTOBUF_DIR
pwd

# Common build settings
CMAKE_C_FLAGS="-O3 -DNDEBUG -fvisibility=hidden -ffunction-sections -fstack-protector-all -Wno-error"
CMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -fvisibility-inlines-hidden"

# # Build for macOS
# echo "Building protobuf for deskmate (macOS) ...."
# ./autogen.sh
# mkdir -p cmake/build/release
# pushd cmake/build/release
# export CMAKE_EXE=/usr/bin/cmake

# ${CMAKE_EXE} \
#   -DCMAKE_BUILD_TYPE=Release \
#   -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
#   -DCMAKE_C_FLAGS_RELEASE="${CMAKE_C_FLAGS}" \
#   -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS}" \
#   -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mac \
#   -Dprotobuf_BUILD_SHARED_LIBS=OFF \
#   ../..

# make -j8 install
# echo "make install done!"
# cp $(find . -name js_embed -type f) ${DISTDIR}/mac/bin/js_embed
# popd
# echo "Clean out string..."

# # Clean out the macOS build state before building for vicOS
# echo "Cleaning out build state from macOS build"
# git clean -dffx .
# echo "git clean -dffx . done"
# git submodule foreach --recursive 'git clean -dffx .'
# echo "done - git submodule foreach --recursive 'git clean -dffx .'"

# Build for vicOS
echo "Building protobuf for deskmate (vicOS) ...."
# rm -rf cmake/build/release
./autogen.sh
mkdir -p cmake/build/release
pushd cmake/build/release
PROJECT_HOME=/home/lryain/deskmate-dev
${CMAKE_EXE} \
  -DCMAKE_TOOLCHAIN_FILE=${PROJECT_HOME}/cmake/mateos.oelinux.toolchain.cmake \
  -DMATEOS_SDK="${MATEOS_SDK_HOME}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
  -DCMAKE_C_FLAGS_RELEASE="${CMAKE_C_FLAGS}" \
  -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS}" \
  -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mateos \
  -Dprotobuf_BUILD_TESTS=FALSE \
  ../..

PATH=${DISTDIR}/mac/bin:$PATH make -j8 install
pwd
popd

${MAKE_DEP_ARCHIVE_SH} protobuf ${PROTOBUF_REVISION_TO_BUILD}

rm -rf ${DISTDIR}
