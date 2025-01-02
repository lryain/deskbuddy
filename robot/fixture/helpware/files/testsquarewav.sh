#!/bin/sh
killall -9 pcm && exit
/etc/initscripts/lrya-audio-init
export LD_LIBRARY_PATH=/lrya/menuman
/lrya/menuman/pcm -f 440 --square &