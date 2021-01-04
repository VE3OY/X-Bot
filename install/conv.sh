#!/bin/sh

p1=$(pidof -s conversd)
p2=$(pidof -s XBot)

if [ "$p1" = "" ] || [ "$p2" = ""  ]; then
    echo "CONVERSD or XBot not found running ... starting CONVERSD and XBot"
    logger "CONVERSD or XBot not found running ... starting CONVERSD and XBot"
    #
    # just in case, kill any running process
    killall conversd > /dev/null 2>&1
    killall XBot > /dev/null 2>&1
    sleep 1
    /usr/local/sbin/conversd
    sleep 1
    /usr/local/bin/XBot
    touch /var/log/conversd.log
else
    echo "   CONVERSD is running on PID: `pidof conversd`"
    echo "   XBot is running on PID: `pidof XBot`"
fi
