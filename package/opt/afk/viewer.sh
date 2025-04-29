#!/usr/bin/env bash
/opt/afk/vncviewer -viewonly 0 -menukey '' 127.0.0.1:590$1 >/dev/null 2>&1 &

HOSTNAME=$(hostname)

while true; do
    CLIENT=$(xdotool search --onlyvisible --name "$HOSTNAME:$1 - TigerVNC")
    if [ -n "$CLIENT" ]; then
        wmctrl -r "$HOSTNAME:$1 - TigerVNC" -T "Tibia $1"
        break 
    fi
    sleep 0.3
done
