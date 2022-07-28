#!/usr/bin/env bash
#NAME=$(xwininfo -root -children | grep Tibia | cut -d '"' -f 2 | sed s/"Tibia - "// | sed 's/ //g')
WIDTH=$(xdpyinfo | grep dimensions | cut -d ' ' -f 7 | cut -d 'x' -f 1)
HEIGHT=$(xdpyinfo | grep dimensions | cut -d ' ' -f 7 | cut -d 'x' -f 2)
/opt/afk/test $(lsblk --nodeps -no serial /dev/sda) $USER default $WIDTH $HEIGHT
