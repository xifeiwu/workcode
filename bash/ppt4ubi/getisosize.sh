#!/bin/sh
cd /mnt
#files=(`cat md5sum.txt | awk '{print $2}'`)
files=(`ls`)
for file in ${files[@]}; do
    size=`du -hs ${file} | awk '{print $1}'`
    printf "%-6s%s\n" "${size}" "${file}"
done
