#!/usr/bin/env bash

set -e
set -u

: ${PROTOBUF_VERSION:="v3.5.1"}
: ${OPENCV_REVISION_TO_BUILD:="3.4.0"}

echo "Building opencv ${OPENCV_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
PATCHDIR=`pushd ${SCRIPT_PATH}/opencv-${OPENCV_REVISION_TO_BUILD}-patches >> /dev/null; pwd; popd >> /dev/null`
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       opencv \
       git@github.com:opencv/opencv.git \
       ${OPENCV_REVISION_TO_BUILD}

echo "Finding/Installing protobuf ${PROTOBUF_VERSION}"
DEPTOOLPY=${TOPLEVEL}/tools/lryabuild/deptool.py
# PROTOBUF_DIR=$(${DEPTOOLPY} --project deskmate --name protobuf --url-prefix ${S3_ASSETS_URL_PREFIX} --install ${PROTOBUF_VERSION} | tail -1)
# tools/build/dist/protobuf/mateos
PROTOBUF_DIR=${TOPLEVEL}/dist/protobuf
cd ${BUILDDIR}/opencv

# 手动打补丁
# Apply our Lrya local patches
# for f in ${PATCHDIR}/*.patch; do
#     git apply $f
# done

# Build for mateOS
echo "Building opencv ${OPENCV_REVISION_TO_BUILD} for deskmate (mateOS) ...."

# rm -rf build_mateos
# mkdir build_mateos
pushd build_mateos
# echo "-------------> ${PROTOBUF_DIR}/mateos/lib/libprotobuf.a"

# 编译器在toolchain中已经指定为clang
${CMAKE_EXE} \
  -DCMAKE_TOOLCHAIN_FILE=/home/pi/deskbuddy/cmake/mateos.raspi.toolchain.cmake \
  -DCMAKE_INSTALL_PREFIX=${DISTDIR}/mateos \
  -DENABLE_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_DOCS=OFF \
  -DBUILD_FAT_JAVA_LIB=OFF \
  -DBUILD_JASPER=OFF \
  -DBUILD_OPENEXR=OFF \
  -DBUILD_PACKAGE=OFF \
  -DBUILD_PERF_TESTS=OFF \
  -DBUILD_opencv_java=OFF \
  -DBUILD_TESTS=OFF \
  -DBUILD_TIFF=OFF \
  -DBUILD_WITH_DEBUG_INFO=OFF \
  -DBUILD_opencv_apps=OFF \
  -DBUILD_opencv_java=OFF \
  -DBUILD_opencv_python2=OFF \
  -DBUILD_opencv_world=OFF \
  -DCMAKE_C_FLAGS_RELEASE="-O3 -DNDEBUG -fvisibility=hidden -ffunction-sections -fstack-protector-all" \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -fvisibility=hidden  -ffunction-sections -fstack-protector-all -fvisibility-inlines-hidden" \
  -DENABLE_PRECOMPILED_HEADERS=OFF \
  -DWITH_EIGEN=OFF \
  -DWITH_JASPER=OFF \
  -DWITH_OPENEXR=OFF \
  -DWITH_TIFF=ON \
  -DWITH_TBB=ON \
  -DWITH_CUDA=OFF \
  -DWITH_CUFFT=OFF \
  -DWITH_GTK=OFF \
  -DWITH_WEBP=OFF \
  -DWITH_CAROTENE=OFF \
  -DMATEOS_CPP_FEATURES='rtti exceptions' \
  -DBUILD_PROTOBUF=OFF \
  -DPROTOBUF_UPDATE_FILES=ON \
  -DProtobuf_LIBRARY=${PROTOBUF_DIR}/mateos/lib/libprotobuf.a \
  -DProtobuf_INCLUDE_DIR=${PROTOBUF_DIR}/mateos/include \
  -DProtobuf_PROTOC_EXECUTABLE=${PROTOBUF_DIR}/mateos/bin/protoc \
  -DCMAKE_BUILD_WITH_INSTALL_RPATH=ON ..

make opencv_dnn -j8
make -j8 install

mkdir -vp ${DISTDIR}/mateos/3rdparty/lib
# for cross compile
# cp -vrf install/* ${DISTDIR}/mateos/
cp -vrf unix-install/* ${DISTDIR}/mateos/
cp -vrf 3rdparty/lib ${DISTDIR}/mateos/3rdparty

mkdir -p ${DISTDIR}/modules

cp -vr ../modules/* ${DISTDIR}/modules
popd

${MAKE_DEP_ARCHIVE_SH} opencv ${OPENCV_REVISION_TO_BUILD}
# rm -rf ${DISTDIR}
