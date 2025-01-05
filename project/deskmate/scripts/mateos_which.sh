#!/usr/bin/env bash

if [ $# -eq 0 ]; then
	echo "Need an argument"
	exit 1
fi

GIT_PROJ_ROOT=`git rev-parse --show-toplevel`
MATEOS_SDK=`${GIT_PROJ_ROOT}/tools/build/tools/ankibuild/mateos.py --find 1.1.0-r04`

if [ ! -d "$MATEOS_SDK" ]; then
	echo "Could not find mateos SDK"
	exit 1
fi

MATEOS_BIN=$MATEOS_SDK/prebuilt/bin
LINUX_VERSION=$MATEOS_BIN/arm-oe-linux-gnueabi-$1
if [ -f "$LINUX_VERSION" ]; then
	echo "$LINUX_VERSION"
	exit 0
fi

if [ -f "$MATEOS_BIN/$1" ]; then
	echo "$MATEOS_BIN/$1"
	exit 0
fi

echo "Could not find executable $1 in $MATEOS_BIN"
exit 1
