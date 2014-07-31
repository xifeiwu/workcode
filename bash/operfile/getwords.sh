#!/bin/sh
all=`cat 3500.txt`;
cnt=0
for word in ${all}
do
#    echo "==============$((cnt++))================="
#    echo $word ${#word}
    echo $word | grep '^[a-z][a-z]*[a-z]$' > /dev/null 2>&1
    if [ $? -eq 0 -a ${#word} -ge 6 ]; then
        echo $word
    fi
done
