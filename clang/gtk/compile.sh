#!/bin/sh
if [ -z $1 ] ; then
    echo "input file name"
    exit 1
fi

gcc -g `pkg-config --cflags gtk+-3.0 cairo` $1 -o binary `pkg-config --libs gtk+-3.0 cairo`
