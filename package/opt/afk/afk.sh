#!/usr/bin/env bash
BOT=1
DONE=0

while ! [ $DONE == 1 ]; do
  FILE=/opt/afk/bot$BOT
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

touch /opt/afk/bot$BOT
DISPLAY=:$BOT /opt/afk/afk $(lsblk --nodeps -no serial /dev/sda) $USER > /dev/null 2>&1
rm /opt/afk/bot$BOT