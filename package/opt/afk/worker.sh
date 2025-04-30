#!/usr/bin/env bash
Xvfb :$1 -screen 0 1012x642x24 -extension RANDR >/dev/null 2>&1 &
sleep 0.1

x11vnc -display :$1 -forever -shared -rfbport 590$1 >/dev/null 2>&1 &
sleep 0.1

DISPLAY=:$1 /opt/tibia/Tibia >/dev/null 2>&1 &

while true; do
	CLIENT=$(DISPLAY=:$1 xdotool search --onlyvisible --name Tibia)
	if [ -n "$CLIENT" ]; then
		DISPLAY=:$1 xdotool windowmove $CLIENT 0 0
		break 
	fi
	sleep 0.3
done