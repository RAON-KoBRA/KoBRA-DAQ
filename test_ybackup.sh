inotifywait -m -r -e close_write "/home/kobradaq/online/data" --format "%f"  . |
  while IFS= read -r line
  do
    \cp "$line" /mnt/sda/kobra_testrun/data  #this would not replicate directories
    \scp "$line" daqcontrol@10.1.51.136:/mnt/sdb/kobra_testrun/data
  done
