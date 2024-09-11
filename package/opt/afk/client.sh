#!/usr/bin/env bash
Xephyr -br -ac -noreset -screen 1012x642 -title Tibia$1 :$1 &
sleep .2
xseticon -id $(xdotool search --name Tibia$1) /opt/afk/t$1.png
DISPLAY=:$1 /opt/tibia/Tibia > /dev/null 2>&1 &
sleep 4

CLIENTS=$(DISPLAY=:$1 xdotool search --name Tibia)
if [[ -z $CLIENTS ]]; then
  DISPLAY=:$1 /opt/tibia/Tibia > /dev/null 2>&1 &
  sleep 4
fi

CLIENTS=$(DISPLAY=:$1 xdotool search --name Tibia)
for CLIENT in $CLIENTS; do
  DISPLAY=:$1 xdotool windowmove $CLIENT 0 0
  DISPLAY=:$1 xdotool windowsize $CLIENT 1012 642
done