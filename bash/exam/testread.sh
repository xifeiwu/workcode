#!/bin/bash
while read line
do
#    echo $line
    for word in $line
    do
        echo $word
    done
done < names.txt

while true
do
    echo "夜不眠"
    sleep 10
done
