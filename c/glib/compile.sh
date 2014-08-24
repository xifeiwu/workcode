#!/bin/sh
if [ -z $1 ] ; then
    echo "input file name"
    exit 1
fi

gcc `pkg-config --cflags glib-2.0 gthread-2.0 dbus-1 dbus-glib-1 gio-2.0` $1 -o binary `pkg-config --libs glib-2.0 gthread-2.0 dbus-1 dbus-glib-1 gio-2.0` --std=c99
