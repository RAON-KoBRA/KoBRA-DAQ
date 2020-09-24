./kill_daq.sh

gnome-terminal --disable-factory --working-directory='/home/kobradaq/online/data/'  -x bash -c "source /home/kobradaq/online/data/test_ybackup.sh; exec bash"\ &

	echo $! > ~/online/yb.pid

odbedit -c clean

mhttpd -D
mlogger -D

sleep 2 

# xterm -e ./frontend & 

./frontend &
# gnome-terminal --disable-factory -e ./frontend & 
gnome-terminal --disable-factory -e  ./analyzer &
#gnome-terminal --disable-factory -e  bash ~/online/data/test_ybackup.sh &

#./frontend &

#mlogger -D

#~/packages/roody/bin/roody -Hlocalhost

#echo Please point your web browser to http://kobradaq.test:8081
echo Please point your web browser to https://10.1.51.135:8443 (id: midas, pwd: kobradaq)
#echo run: firefox http://kobradaq.test:8081 &
echo run: firefox http://10.1.51.135:8443 &
echo To look at live histogram, run: roody -Hlocalhost



