#!/usr/bin/env bash

set -e
set -u

: ${PROTOBUF_REVISION_TO_BUILD:="v3.5.1"}

echo "Building protobuf ${PROTOBUF_REVISION_TO_BUILD} for victor ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       protobuf \
       git@github.com:protocolbuffers/protobuf.git \
       ${PROTOBUF_REVISION_TO_BUILD}

cd ${BUILDDIR}/protobuf

# Common build settings
CMAKE_C_FLAGS="-O3 -DNDEBUG -fvisibility=hidden -ffunction-sections -fstack-protector-all -Wno-error -fPIC"
CMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -fvisibility-inlines-hidden -fPIC"

# Build for mateos
echo "Building protobuf for victor (mateos) ...."
# ./autogen.sh
rm -rf cmake/build/release
mkdir -p cmake/build/release
pushd cmake/build/release

${CMAKE_EXE} \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
  -DCMAKE_C_FLAGS_RELEASE="${CMAKE_C_FLAGS}" \
  -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS}" \
  -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mateos \
  -Dprotobuf_BUILD_TESTS=FALSE \
  ../..
#   -Dprotobuf_BUILD_SHARED_LIBS=OFF \

make -j8 install
cp $(find . -name js_embed -type f) ${DISTDIR}/mateos/bin/js_embed
popd

${MAKE_DEP_ARCHIVE_SH} protobuf ${PROTOBUF_REVISION_TO_BUILD}

# rm -rf ${DISTDIR}
