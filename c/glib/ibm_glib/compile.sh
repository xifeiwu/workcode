#!/bin/sh
if [ -z $1 ] ; then
    echo "input file name"
    exit 1
fi

gcc -g `pkg-config --cflags glib-2.0 gthread-2.0` $1 -o binary `pkg-config --libs glib-2.0 gthread-2.0`
