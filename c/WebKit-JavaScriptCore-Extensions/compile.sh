#!/bin/sh
if [ -z $1 ] ; then
    echo "input file name"
    exit 1
fi

gcc -g `pkg-config --cflags webkitgtk-3.0 cairo dbus-1 dbus-glib-1` $1 -o binary `pkg-config --libs webkitgtk-3.0 cairo dbus-1 dbus-glib-1`
