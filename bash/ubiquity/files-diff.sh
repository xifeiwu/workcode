#!/bin/sh
pkgname=
fullname=$1
if [ "$#" -ne 1 ]; then
    echo "error, file name of deb is needed."
    echo "only one parameter is needed."
    exit 1
else
    if [ -d ./data ]; then
        rm -rf ./data
    fi
    dpkg -x $1 ./data
    if [ "$?" -eq 0 ]; then
        pkgname=`echo ${fullname%%_*} | sed 's/\-/_/g'`
        echo "Content of $1 is extracted to ./data"
        echo "package name is: ${pkgname}"
    else
        echo "extract fail"
        exit 1
    fi
fi
if [ ! -d ./data ]; then
    echo "directory data is not found."
    exit 0
fi
if [ ! -d ./results ]; then
    mkdir ./results
fi
echo "" > ./results/${pkgname}.diff
for file in `find ./data -type f`
do
    sysfile=`echo ${file} | sed 's:./data::'`
#    echo  ${file} : ${sysfile}
    diff -u ${file} ${sysfile} >> ./results/${pkgname}.diff
done
echo "Result of diff is stored in ./results/${pkgname}.diff"
echo "remove unused directory, use command below:"
echo "rm -rf ./data"
