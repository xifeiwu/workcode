#!/bin/sh
function notice()
{
    echo -e "\033[2;32m-$@\033[0m" | tee -a ${LOGFILE}
}
function warning()
{
    echo -e "\033[2;33m-$@\033[0m" | tee -a ${LOGFILE}
}
function error()
{
    echo -e "\033[2;31m-$@\033[0m" | tee -a ${LOGFILE}
    exit 1
}

if [ ! $# -eq 2 ]; then
    echo "usage: sh ubi-diff.sh fromdir todir"
    error
fi
if [ ! -d $1 ]; then
    echo "$1 is not directory."
    error
fi
if [ ! -d $2 ]; then
    echo "$2 is not directory."
    error
fi
hostdir=`pwd`
from=`cd $1; pwd`
to=`cd $2; pwd`
echo "from: ${from}"
echo "to: ${to}"

declare -a files
declare -a dirs
cd ${from}
all=(`ls`)
for f in ${all[@]}; do
    if [ -f ${f} ]; then
        files=(${files[@]} ${f})
    fi
    if [ -d ${f} ]; then
        dirs=(${dirs[@]} ${f})
    fi
done
notice "files found: ${files[@]}"
notice "dir found: ${dirs[@]}"

cd ${hostdir}
if [ ! -d diff ]; then
    mkdir diff
fi
rm ./diff/* > /dev/null 2>&1
for dir in ${dirs[@]}; do
    notice "diff -uNra ${from}/${dir} ${to}/${dir} > ./diff/${dir}.diff"
#    read -p "go on[Y/n]?" yn
    yn="y"
    if [ ${yn} == "y" ]; then
        diff -uNra ${from}/${dir} ${to}/${dir} > ./diff/${dir}.diff
    else
        warning "diff ignored..."
    fi
done
