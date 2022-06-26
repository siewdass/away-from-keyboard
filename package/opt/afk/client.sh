#!/usr/bin/env bash
X=1
CLIENT=0
DONE=0

while ! [ $DONE == 1 ]; do
  FILE=/tmp/.X11-unix/X$X
  if [ -e "$FILE" ]; then
    if [ $X == 1 ]; then
      X=2
    elif [ $X == 2 ]; then
      X=3
    else
      X=4
    fi
  else
    DONE=1
    echo DISPLAY $X
  fi
done

Xephyr -keybd ephyr,,,xkbmodel=pc105,xkblayout=latam,us,xkbrules=evdev,xkboption=grp:alts_toogle -br -ac -noreset -screen 1012x642 -title Tibia$X :$X > /dev/null 2>&1 &
sleep 0.2
DONE=0
xseticon -id $(xdotool search --name Tibia$X) /opt/afk/t$X.png
#DISPLAY=:$X /opt/tibia/Tibia > /dev/null 2>&1
DISPLAY=:$X xset s off
DISPLAY=:$X vglrun -d :$X /opt/tibia/Tibia > /dev/null 2>&1

while ! [ $DONE == 1 ]; do
  CLIENT=$(DISPLAY=:$X xwininfo -root -children | grep Tibia | cut -d ' ' -f 6)
  if [ $CLIENT ]; then
    DONE=1
  fi
  sleep 0.3
done

DISPLAY=:$X xdotool windowmove $CLIENT 0 0
DISPLAY=:$X xdotool windowsize $CLIENT 1012 642