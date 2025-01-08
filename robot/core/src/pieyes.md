# eyes

安装桌面支持
sudo apt-get install --no-install-recommends xserver-xorg

cd /home/pi/deskbuddy/robot/core/src/
make clean && make -j4
sudo fbx2 -i

cd /boot/Pi_Eyes;xinit /home/lryain/.env/doly/bin/python eyes.py --radius 240 :0 &

/home/lryain/.env/doly/bin/python /boot/Pi_Eyes/eyes.py --radius 240 :0 &
