#!/bin/sh
PS3="Enter option: "
select option in "Display disk space" "Display logged on users" "Display memory usage" "Exit program"
do
    case $option in
    "Exit Program")
    break;;
    "Display disk space")
    echo diskspace;;
    "Display logged on users")
    echo whoseon;;
    "Display memory usage")
    echo memusage;;
    *)
    clear
    echo Sorry, wrong selection
    ;;
    esac
done
