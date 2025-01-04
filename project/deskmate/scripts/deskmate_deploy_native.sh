#!/usr/bin/env bash
set -e
set -u

export GIT_PROJ_ROOT=`git rev-parse --show-toplevel`

if [ $# -eq 1 -a $1 = -h ]; then
    ${GIT_PROJ_ROOT}/project/deskmate/scripts/deploy_native.sh "$@"
    exit 0
fi

${GIT_PROJ_ROOT}/project/deskmate/scripts/stage.sh "$@"

# Also stage mateos
if [ $? -eq 0 ]; then
    PLATFORM_NAME=mateos ${GIT_PROJ_ROOT}/project/deskmate/scripts/stage.sh -k "$@"
fi

${GIT_PROJ_ROOT}/project/deskmate/scripts/deploy_native.sh "$@"
