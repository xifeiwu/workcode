#!/bin/sh
if [ -z $1 ] ; then
    echo "input file name"
    exit 1
fi

gcc -g `pkg-config --cflags gobject-2.0` $1 -o binary `pkg-config --libs gobject-2.0`
