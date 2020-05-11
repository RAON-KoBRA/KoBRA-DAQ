./kill_daq.sh

odbedit -c clean

mhttpd -D

sleep 2 

# xterm -e ./frontend & 

./frontend &
# gnome-terminal -e ./frontend & 
gnome-terminal -e ./analyzer &
#./frontend &
mlogger -D

echo Please point your web browser to http://kobradaq.test:8081
echo run: firefox http://kobradaq.test:8081 &
echo To look at live histogram, run: roody -Hlocalhost



