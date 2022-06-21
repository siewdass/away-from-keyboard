#!/usr/bin/env bash
BOT=1
DONE=0

while ! [ $DONE == 1 ]; do
  FILE=/tmp/afk$BOT
  if [ -e "$FILE" ]; then
    if [ $BOT == 1 ]; then
      BOT=2
    elif [ $BOT == 2 ]; then
      BOT=3
    else
      BOT=4
    fi
  else
    DONE=1
  fi
done

NAME=$(DISPLAY=:$BOT xwininfo -root -children | grep Tibia | cut -d '"' -f 2 | sed s/"Tibia - "// | sed 's/ //g')

touch /tmp/afk$BOT
DISPLAY=:$BOT /opt/afk/afk $(lsblk --nodeps -no serial /dev/sda) $USER $NAME
rm /tmp/afk$BOT