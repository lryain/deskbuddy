#!/usr/bin/env bash

# Handy Victor Aliases from Al Chaussee
# =====================================
# They can be called from anywhere within the git project to build, deploy, and run on physical robots.
# Call source on this file from your .bash_profile if you always want the latest version of these aliases 
# or copy its contents into your .bash_profile.

alias GET_GIT_ROOT='export GIT_PROJ_ROOT=`git rev-parse --show-toplevel`'

alias deskmate_restart='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_restart.sh'
alias deskmate_start='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_start.sh'
alias deskmate_stop='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_stop.sh'

# shipping, the build that is delivered to customers
# release, a shipping build with custom options for development, e.g. profilers, webservices

alias deskmate_build_shipping='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_shipping.sh'
alias deskmate_build_userdev='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_userdev.sh'
alias deskmate_build_beta='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_beta.sh'
alias deskmate_build_release='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_release.sh'
alias deskmate_build_oskr='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_oskr.sh'
alias deskmate_build_release_with_profiling='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_release_with_profiling.sh'
alias deskmate_build_debug='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_debug.sh'
alias deskmate_build_debugo2='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_debugo2.sh'
alias deskmate_build_xcode='GET_GIT_ROOT; source ${GIT_PROJ_ROOT}/project/deskmate/scripts/usefulALiases.sh; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_xcode.sh'

alias deskmate_deploy_release='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_deploy.sh -c Release'
alias deskmate_deploy_debug='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_deploy.sh -c Debug'
alias deskmate_deploy_run='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_deploy_run.sh'
alias deskmate_build_run='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_build_run.sh'

alias deskmate_log='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_log.sh'
alias deskmate_addr2line='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/addr2line.sh'

# If you have lnav...
alias deskmate_lnav='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_log_lnav.sh'

alias mateos-which='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/mateos_which.sh'

#
# Log management
#
alias deskmate_log_upload='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_log_upload.sh'
alias deskmate_log_download='GET_GIT_ROOT; ${GIT_PROJ_ROOT}/project/deskmate/scripts/deskmate_log_download.sh'

