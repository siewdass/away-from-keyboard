#!/usr/bin/env bash
Xephyr -br -ac -noreset -screen 1012x642 -title Tibia$1 :$1 &
sleep .2
#xseticon -id $(xdotool search --name Tibia$1) /opt/afk/t$1.png
export DISPLAY=:$1
timeout 4 /opt/tibia/Tibia

sleep 4
CLIENTS=$(xdotool search --name Tibia)
for CLIENT in $CLIENTS; do
  xdotool windowmove $CLIENT 0 0
  xdotool windowsize $CLIENT 1012 642
done
