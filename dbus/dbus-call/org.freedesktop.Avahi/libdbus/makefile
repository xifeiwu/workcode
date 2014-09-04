#
# Simple Makefile for the libdbus example that will send the "Display
# Note dialog" RPC message. You need to run the example in the SDK or
# a compatible device.
#
# Define a list of pkg-config packages we want to use
pkg_packages := dbus-glib-1 dbus-1
PKG_CFLAGS  := $(shell pkg-config --cflags $(pkg_packages))
PKG_LDFLAGS := $(shell pkg-config --libs $(pkg_packages))
# Additional flags for the compiler:
#    -g : Add debugging symbols
# -Wall : Enable most gcc warnings
ADD_CFLAGS := -g -Wall  --std=c99
# Combine user supplied, additional, and pkg-config flags
CFLAGS  := $(PKG_CFLAGS) $(ADD_CFLAGS) $(CFLAGS)
LDFLAGS := $(PKG_LDFLAGS) $(LDFLAGS)
targets = call-avahi 
.PHONY: all clean
all: $(targets)

gen_marshal: marshal.list
	glib-genmarshal --header --prefix "_avahi_marshal" $^  > marshal.h
	glib-genmarshal --body $^  --prefix "_avahi_marshal" > marshal.c

call-avahi: call-avahi.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(targets)
