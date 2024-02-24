#!/usr/bin/env bash
Xephyr -br -ac -noreset -screen 1012x642 -title Tibia$1 :$1 &
sleep .2
xseticon -id $(xdotool search --name Tibia$1) /opt/afk/t$1.png
DISPLAY=:$1 /opt/tibia/Tibia > /dev/null 2>&1
DONE=0

while ! [ $DONE == 1 ]; do
  CLIENT=$(DISPLAY=:$1.0 xdotool search --class Tibia)
  XY=$(DISPLAY=:$1.0 xdotool search --class Tibia getwindowgeometry | awk -F. '{printf($1)}' | cut -d " " -f5)

  DISPLAY=:$1 xdotool windowmove $CLIENT 0 0
  DISPLAY=:$1 xdotool windowsize $CLIENT 1012 642

  if [ $XY == "0,0" ]; then
    DONE=1
  fi
  sleep .1
done