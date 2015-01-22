#!/bin/bash
if [ $# == 0 ]; then
    echo "needs a command(parameter)."
    exit 1
fi
cmd=$1
case ${cmd} in
    "sleep")
    echo "Sleep for a while."
    sleep 3
    echo "mem" > /sys/power/state
    ;;
esac    
