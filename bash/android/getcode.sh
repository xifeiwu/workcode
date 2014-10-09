#!/bin/bash
cnt=0
while true
do
    ((cnt++))
    echo try$cnt
#    curl https://storage.googleapis.com/git-repo-downloads/repo > ./repo
#    repo init -u https://android.googlesource.com/platform/manifest
#    repo init -u https://android.googlesource.com/platform/manifest -b android-4.4.2_r2
    repo sync
    if [ $? == 0 ]; then
        break
    else
        continue
    fi
    sleep 1
done
