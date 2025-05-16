#!/usr/bin/env bash

set -e
set -u

: ${FLATBUFFERS_REVISION_TO_BUILD:="v1.5.0"}

echo "Building flatbuffers ${FLATBUFFERS_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       flatbuffers \
       https://github.com/google/flatbuffers \
       ${FLATBUFFERS_REVISION_TO_BUILD}
cd ${BUILDDIR}/flatbuffers
# Build host prebuilts
# HOST_PREBUILTS_DIR=${DISTDIR}/host-prebuilts
# echo "================> HOST_PREBUILTS_DIR: ${HOST_PREBUILTS_DIR}"
# mkdir -p ${HOST_PREBUILTS_DIR}
# pushd ${HOST_PREBUILTS_DIR}
# # mkdir ${FLATBUFFERS_REVISION_TO_BUILD}
# # rm ${HOST_PREBUILTS_DIR}/current
# ln -s ${FLATBUFFERS_REVISION_TO_BUILD} current
# HOST_PREBUILTS_CURRENT_DIR=${HOST_PREBUILTS_DIR}/current
# popd

# CMAKE_C_FLAGS="-O3 -DNDEBUG -fvisibility=hidden -ffunction-sections -fstack-protector-all -Wno-error -fPIC"
# # CMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -fvisibility-inlines-hidden -fPIC"
# # 看是否需要加上libc++
# CMAKE_CXX_FLAGS="${CMAKE_C_FLAGS} -fvisibility-inlines-hidden -std=c++14 -stdlib=libc++ -fPIC"

# Build for vicOS
echo "Build flatbuffers ${FLATBUFFERS_REVISION_TO_BUILD} for deskmate (vicOS)"
rm -rf build_mateos
mkdir build_mateos
pushd build_mateos

${CMAKE_EXE} \
  -DCMAKE_TOOLCHAIN_FILE=/home/orangepi/dev/deskbuddy/cmake/mateos.toolchain.cmake \
  -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mateos \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DFLATBUFFERS_BUILD_TESTS=OFF ..
make -j8
mkdir -p ${DISTDIR}/mateos
cp libflatbuffers.a ${DISTDIR}/mateos/
popd

# Copy the includes
echo "Copy the flatbuffers include directory to the distribution directory"
cp -pvR ./include ${DISTDIR}/include

${MAKE_DEP_ARCHIVE_SH} flatbuffers ${FLATBUFFERS_REVISION_TO_BUILD}

# rm -rf ${DISTDIR}
