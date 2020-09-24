inotifywait -e close_write --format "%f" --monitor . |
  while IFS= read -r line
  do
    \cp "$line" /mnt/sda/kobra_testrun/data  #this would not replicate directories
  done
