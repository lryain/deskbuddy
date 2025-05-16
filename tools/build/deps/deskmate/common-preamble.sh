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
# echo "Finding/Installing mateos-sdk ${MATEOS_SDK_VERSION}"
# MATEOSPY=${TOPLEVEL}/tools/lryabuild/mateos.py
# export MATEOS_SDK_HOME=$(${MATEOSPY} --install ${MATEOS_SDK_VERSION} | tail -1)

# echo "Finding/Installing cmake 3.9.6 ......"
# CMAKEPY=${TOPLEVEL}/tools/lryabuild/cmake.py
# export CMAKE_EXE=$(${CMAKEPY} --install-cmake 3.9.6 | tail -1)
export CMAKE_EXE=cmake

# Create a fresh and clean build directory
# rm -rf ${BUILDDIR}

# 下载
# mkdir -p ${BUILDDIR}
# # Clone source code from git
# cd ${BUILDDIR}
# echo "-------------> BUILDDIR: ${BUILDDIR}"
# echo "xxxxxxxxx -------------> GITURL: ${GITURL}"
# # git clone ${GITURL}
# git clone --depth=1 -b ${GITREV} ${GITURL}

# # Checkout the desired revision
# cd ${DEPNAME}
# echo "-------------> GITREV: ${GITREV}"
# git checkout ${GITREV}
# echo "-------------> DEPNAME: ${DEPNAME}"
# git submodule update --init --recursive

# 不下载
mkdir -p ${BUILDDIR}

# Clone source code from git
cd ${BUILDDIR}
echo "-------------> BUILDDIR: ${BUILDDIR}"
echo "xxxxxxxxx -------------> GITURL: ${GITURL}"
# git clone ${GITURL}
# git clone --depth=1 -b ${GITREV} ${GITURL}

# Checkout the desired revision
cd ${DEPNAME}
echo "-------------> GITREV: ${GITREV}"
# git checkout ${GITREV}
echo "-------------> DEPNAME: ${DEPNAME}"
# git submodule update --init --recursive
