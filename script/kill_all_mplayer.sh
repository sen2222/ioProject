#!/bin/sh

while pgrep mplayer > /dev/null
do
    pkill -9 mplayer
    sleep 1
done

echo "All mplayer processes have been terminated."

