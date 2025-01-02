#!/bin/sh
rm -rf /data/burn/
killall -9 burn
mkdir -p /data/burn/
/lrya/menuman/burn 0 &
/lrya/menuman/burn 1 &
/lrya/menuman/burn 2 &
/lrya/menuman/burn 3 &
/lrya/menuman/burn 0 disk &
/lrya/menuman/burn 1 disk &
/lrya/menuman/burn 2 disk &
/lrya/menuman/burn 3 disk &
