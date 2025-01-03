#!/usr/bin/env bash

set -e
set -u

: ${OPUS_REVISION_TO_BUILD:="e04e86e0"}

echo "Building opus ${OPUS_REVISION_TO_BUILD} for deskmate ....."

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

source ${SCRIPT_PATH_ABSOLUTE}/common-preamble.sh \
       opus \
       git@github.com:xiph/opus.git \
       ${OPUS_REVISION_TO_BUILD}

cd ${BUILDDIR}/opus

# Build for vicOS
echo "Building opus for deskmate (vicOS) ...."
# MATEOS_TOOLCHAIN_ROOT=${MATEOS_SDK_HOME}/prebuilt
# MATEOS_TOOLCHAIN_NAME=""
# MATEOS_TOOLCHAIN_PREFIX=${MATEOS_TOOLCHAIN_ROOT}/bin/${MATEOS_TOOLCHAIN_NAME}-
MATEOS_TOOLCHAIN_PREFIX=""

MATEOS_CC="CC=${MATEOS_TOOLCHAIN_PREFIX}clang"
MATEOS_RANLIB="RANLIB=${MATEOS_TOOLCHAIN_PREFIX}ranlib"
MATEOS_AR="AR=${MATEOS_TOOLCHAIN_PREFIX}ar"
MATEOS_NM="NM=${MATEOS_TOOLCHAIN_PREFIX}nm"
# MATEOS_HOST="--host=${MATEOS_TOOLCHAIN_NAME}"
MATEOS_FLAGS="$MATEOS_CC $MATEOS_RANLIB $MATEOS_AR $MATEOS_NM"

./autogen.sh
PREFIX="${BUILDDIR}/mateos"
mkdir -p "${PREFIX}"
CFLAGS=-O3 ./configure --prefix=${PREFIX} $MATEOS_FLAGS --enable-fixed-point
make install

# Copy artifacts to distribution directory
mkdir -p "${DISTDIR}/mateos/lib"
cp -av "${BUILDDIR}/mateos/lib/"libopus* "${DISTDIR}/mateos/lib/"
cp -av "${BUILDDIR}/mateos/include" "${DISTDIR}/mateos/"

${MAKE_DEP_ARCHIVE_SH} opus ${OPUS_REVISION_TO_BUILD}

# rm -rf ${DISTDIR}
