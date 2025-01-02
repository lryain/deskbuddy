#!/usr/bin/env bash

set -e
set -u

DEPNAME=${1}
GITURL=${2}
GITREV=${3}

: ${MATEOS_SDK_VERSION:="1.1.0-r04"}
: ${S3_ASSETS_URL_PREFIX:="https://sai-general.s3.amazonaws.com/build-assets/deps/deskmate/"}

SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_PATH_ABSOLUTE=`pushd ${SCRIPT_PATH} >> /dev/null; pwd; popd >> /dev/null`

export S3_ASSETS_URL_PREFIX
export TOPLEVEL=`pushd ${SCRIPT_PATH}/../.. >> /dev/null; pwd; popd >> /dev/null`
export BUILDDIR=${TOPLEVEL}/build-deskmate-deps/build-${DEPNAME}
export DISTTOP=${TOPLEVEL}/dist
export DISTDIR=${DISTTOP}/${DEPNAME}
export MAKE_DEP_ARCHIVE_SH=${TOPLEVEL}/deps/make-dep-archive.sh

echo "Finding/Installing mateos-sdk ${MATEOS_SDK_VERSION}"
# MATEOSPY=${TOPLEVEL}/tools/lryabuild/mateos.py
# export MATEOS_SDK_HOME=$(${MATEOSPY} --install ${MATEOS_SDK_VERSION} | tail -1)
export MATEOS_SDK_HOME=/home/lryain/.lrya/mateos-sdk/dist/1.1.0-r04

echo "Finding/Installing cmake 3.9.6 ......"
CMAKEPY=${TOPLEVEL}/tools/lryabuild/cmake.py
# export CMAKE_EXE=$(${CMAKEPY} --install-cmake 3.9.6 | tail -1)
export CMAKE_EXE=/usr/bin/cmake
# Create a fresh and clean build directory
# rm -rf ${BUILDDIR}
# mkdir -p ${BUILDDIR}
echo "BUILDDIR: ${BUILDDIR}"
# /home/lryain/deskmate-dev/tools/build/build-deskmate-deps/build-deskmate-deps/build-protobuf
# Clone source code from git
cd ${BUILDDIR}
echo "BUILDDIR: ${BUILDDIR}"

# git clone ${GITURL}

# Checkout the desired revision
# cd ${DEPNAME}
echo "DEPNAME: ${DEPNAME}"
# git checkout ${GITREV}
echo "GITREV: ${GITREV}"
# git submodule update --init --recursive
