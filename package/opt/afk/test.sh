#!/usr/bin/env bash
BOT=1
NAME=$(DISPLAY=:$BOT xwininfo -root -children | grep Tibia | cut -d '"' -f 2 | sed s/"Tibia - "// | sed 's/ //g')
DISPLAY=:$BOT /opt/afk/test $(lsblk --nodeps -no serial /dev/sda) $USER $NAME
