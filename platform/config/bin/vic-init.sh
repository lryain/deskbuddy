#!/bin/sh -f
#
# platform/config/bin/mate-init.sh
#
# Perform any cleanup & init tasks not handled by the OS.
#
# This script runs each time the service group (lrya-robot.target)
# is started, BEFORE any individual mate-blah services.
#

#
# Forward log events from rampost, if any
#
RAMPOST_LOG=/dev/rampost.log

if [ -f ${RAMPOST_LOG} ]; then
  /lrya/bin/mate-log-forward rampost ${RAMPOST_LOG}
  /bin/rm -f ${RAMPOST_LOG}
fi

#
# Forward kernel panics, if any
#
PANICS_DIR="/data/panics"
if [ -d "${PANICS_DIR}" ]; then
  shopt -s nullglob
  for i in "${PANICS_DIR}"/*
  do
    echo "/lrya/bin/mate-log-kernel-panic ${i}"
    if /lrya/bin/mate-log-kernel-panic "${i}" ; then
      echo "/bin/rm -f ${i}"
      /bin/rm -f "${i}"
    fi
  done
  shopt -u nullglob
fi

#
# Clear fault code, if any
#
/bin/fault-code-clear
