#!/bin/bash

set -e
set -u

# Go to directory of this script
SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_NAME=$(basename ${0})
GIT=`which git`
if [ -z $GIT ]
then
    echo git not found
    exit 1
fi
: ${TOPLEVEL:=`$GIT rev-parse --show-toplevel`}

source ${SCRIPT_PATH}/deskmate_env.sh

# Settings can be overridden through environment
: ${VERBOSE:=0}
: ${FORCE_RSYNC_BIN:=0}
: ${FORCE_DEPLOY:=0}
: ${IGNORE_COMPATIBILITY_MISMATCH:=0}
: ${IGNORE_VERSION_MISMATCH:=0}
: ${LRYA_BUILD_TYPE:="Debug"}
: ${INSTALL_ROOT:="/lrya"}
: ${DEVTOOLS_INSTALL_ROOT:="/lrya-devtools"}
: ${DEVICE_RSYNC_BIN_DIR:="${DEVTOOLS_INSTALL_ROOT}/bin"}
: ${DEVICE_RSYNC_CONF_DIR:="/run/systemd/system"}

function usage() {
  echo "$SCRIPT_NAME [OPTIONS]"
  echo "options:"
  echo "  -h                      print this message"
  echo "  -v                      print verbose output"
  echo "  -r                      force-install rsync binary on robot"
  echo "  -f                      force rsync to (re)deploy all files"
  echo "  -i                      ignore compatibility mismatch. Do not use this. Update your robot/branch!"
  echo "  -b                      ignore version mismatch.  Do not use this.  Update your robot/branch!"
  echo "  -c CONFIGURATION        build configuration {Debug,Release}"
  echo "  -s LRYA_ROBOT_HOST      hostname or ip address of robot"
  echo ""
  echo "environment variables:"
  echo '  $LRYA_ROBOT_HOST        hostname or ip address of robot'
  echo '  $LRYA_BUILD_TYPE        build configuration {Debug,Release}'
  echo '  $INSTALL_ROOT           orangepi dir of installed files on target'
  echo '  $STAGING_DIR            directory that holds staged artifacts before deploy to robot'
}

function logv() {
  if [ $VERBOSE -eq 1 ]; then
    echo -n "[$SCRIPT_NAME] "
    echo $*;
  fi
}

while getopts "hvrfibc:s:" opt; do
  case $opt in
    h)
      usage && exit 0
      ;;
    v)
      VERBOSE=1
      ;;
    r)
      FORCE_RSYNC_BIN=1
      ;;
    f)
      FORCE_DEPLOY=1
      ;;
    i)
      IGNORE_COMPATIBILITY_MISMATCH=1
      ;;
    b)
      IGNORE_VERSION_MISMATCH=1
      ;;
    c)
      LRYA_BUILD_TYPE="${OPTARG}"
      ;;
    s)
      LRYA_ROBOT_HOST="${OPTARG}"
      ;;
    *)
      usage && exit 1
      ;;
  esac
done

robot_set_host

if [ -z "${LRYA_ROBOT_HOST+x}" ]; then
  echo "ERROR: unspecified robot target. Pass the '-s' flag or set LRYA_ROBOT_HOST"
  usage
  exit 1
fi

# echo "LRYA_BUILD_TYPE: ${LRYA_BUILD_TYPE}"
echo "LRYA_ROBOT_HOST: ${LRYA_ROBOT_HOST}"
echo "   INSTALL_ROOT: ${INSTALL_ROOT}"

: ${PLATFORM_NAME:="mateos"}
: ${LIB_INSTALL_PATH:="${INSTALL_ROOT}/lib"}
: ${BIN_INSTALL_PATH:="${INSTALL_ROOT}/bin"}
: ${RSYNC_BIN_DIR="${TOPLEVEL}/tools/rsync"}
: ${STAGING_DIR:="${TOPLEVEL}/_build/staging/${LRYA_BUILD_TYPE}"}

set +e
( # TRY deploy
logv "start deploy"
echo "3. start deploy..."

set -e

#
# Stop any mateos services. If services are allowed to run during 
# deployment, exe and shared library files can't be replaced.
#
logv "stop mateos services"
echo "4. stop mateos services..."

# robot_sh "/bin/systemctl stop mateos.target"
echo "5. create target dirs..."
logv "create target dirs"
mkdir -p "${INSTALL_ROOT}"
echo "5.1. done create ${INSTALL_ROOT} dir"
mkdir -p "${INSTALL_ROOT}/etc"
echo "5.2. done create ${INSTALL_ROOT}/etc dir"
mkdir -p "${LIB_INSTALL_PATH}"
echo "5.3. done create ${LIB_INSTALL_PATH}/etc dir"
mkdir -p "${BIN_INSTALL_PATH}"
echo "5.4. done create ${BIN_INSTALL_PATH}/etc dir"
mkdir -p "${DEVICE_RSYNC_BIN_DIR}"
echo "5.5. done create ${DEVICE_RSYNC_BIN_DIR}/etc dir"

# install rsync binary and config if needed
logv "install rsync if necessary"
echo "6. install rsync if necessary..."

cp -r /home/orangepi/dev/deskbuddy/_build/mateos/Debug/etc ${INSTALL_ROOT}
cp -r /home/orangepi/dev/deskbuddy/_build/mateos/Debug/lib ${INSTALL_ROOT}
cp -r /home/orangepi/dev/deskbuddy/_build/mateos/Debug/data ${INSTALL_ROOT}

logv "finish deploy"
echo "10. finish deploy!"

exit $RSYNC_RESULT
) # End TRY deploy

DEPLOY_RESULT=$?
set -e
echo "11. DEPLOY_RESULT: ${DEPLOY_RESULT}"

if [ $DEPLOY_RESULT -eq 0 ]; then
  logv "deploy succeeded"
else
  logv "deploy FAILED"
fi

cleanup

exit $DEPLOY_RESULT
