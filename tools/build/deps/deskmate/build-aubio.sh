#!/usr/bin/env bash

set -e
set -u

: ${AUBIO_REVISION_TO_BUILD:="d4a1d0fb"}

echo "Building aubio ${AUBIO_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       aubio \
       https://github.com/aubio/aubio.git \
       ${AUBIO_REVISION_TO_BUILD}

cd ${BUILDDIR}/aubio

# Build for macOS
echo "Building aubio ${AUBIO_REVISION_TO_BUILD} for deskmate (macOS) ...."
DEST_DIR_MAC="${BUILDDIR}/mac"
./scripts/get_waf.sh
RANLIB=/usr/bin/ranlib \
AR=/usr/bin/ar \
./waf --verbose configure build install --destdir=$DEST_DIR_MAC
mv $DEST_DIR_MAC/usr/local/* $DEST_DIR_MAC
rm -r $DEST_DIR_MAC/usr

# Copy artifacts to distribution
mkdir -p ${DISTDIR}/mac
cp -av ${DEST_DIR_MAC}/include ${DISTDIR}/mac/
cp -av ${DEST_DIR_MAC}/bin ${DISTDIR}/mac/
cp -av ${DEST_DIR_MAC}/lib ${DISTDIR}/mac/

# Clean out the macOS build state before building for vicOS
echo "Cleaning out build state from macOS build"
git clean -dffx .
git submodule foreach --recursive 'git clean -dffx .'

# Build for vicOS
echo "Building aubio ${AUBIO_REVISION_TO_BUILD} for deskmate (vicOS) ...."
MATEOS_TOOLCHAIN_ROOT=${MATEOS_SDK_HOME}/prebuilt
MATEOS_TOOLCHAIN_NAME=""
MATEOS_TOOLCHAIN_PREFIX=${MATEOS_TOOLCHAIN_ROOT}/bin/${MATEOS_TOOLCHAIN_NAME}-

MATEOS_CC="${MATEOS_TOOLCHAIN_PREFIX}clang"
MATEOS_RANLIB="${MATEOS_TOOLCHAIN_PREFIX}ranlib"
MATEOS_AR="${MATEOS_TOOLCHAIN_PREFIX}ar"
MATEOS_NM="${MATEOS_TOOLCHAIN_PREFIX}nm"

DEST_DIR_MATEOS="${BUILDDIR}/mateos"
rm -rf $DEST_DIR_MATEOS
./scripts/get_waf.sh

WAF_OPTS="--disable-avcodec --disable-samplerate --disable-jack --disable-sndfile --disable-accelerate"
CFLAGS="-Os" \
CC=$MATEOS_CC \
RANLIB=$MATEOS_RANLIB \
AR=$MATEOS_AR \
NM=$MATEOS_NM \
./waf --verbose configure build install \
  --notests \
  --destdir=$DEST_DIR_MATEOS \
  --with-target-platform=linux \
  $WAF_OPTS

# move files to more logical locations
mv $DEST_DIR_MATEOS/usr/local/* $DEST_DIR_MATEOS
rm -r $DEST_DIR_MATEOS/usr

# Copy artifacts to distribution directory
mkdir -p ${DISTDIR}/mateos
cp -av ${DEST_DIR_MATEOS}/include ${DISTDIR}/mateos/
cp -av ${DEST_DIR_MATEOS}/lib ${DISTDIR}/mateos/

# Build distribution archive
${MAKE_DEP_ARCHIVE_SH} aubio ${AUBIO_REVISION_TO_BUILD}

rm -rf ${DISTDIR}
