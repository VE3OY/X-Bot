#!/bin/sh

TST1=`ps -A | grep conversd`
TST2=`ps -A | grep x-bot`


if [ "$TST1" = "" ] || [ "$TST2" = ""  ]; then
    killall conversd
    killall x-bot
    sleep 1
    `which conversd`
    sleep 5
    /usr/local/bin/x-bot
fi
