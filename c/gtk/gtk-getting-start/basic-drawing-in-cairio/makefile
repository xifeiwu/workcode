#
# Simple Makefile for the libdbus example that will send the "Display
# Note dialog" RPC message. You need to run the example in the SDK or
# a compatible device.
#
# Define a list of pkg-config packages we want to use
pkg_packages := gtk+-3.0 cairo 
PKG_CFLAGS  := $(shell pkg-config --cflags $(pkg_packages))
PKG_LDFLAGS := $(shell pkg-config --libs $(pkg_packages))
# Additional flags for the compiler:
#    -g : Add debugging symbols
# -Wall : Enable most gcc warnings
ADD_CFLAGS := -g -Wall
# Combine user supplied, additional, and pkg-config flags
CFLAGS  := $(PKG_CFLAGS) $(ADD_CFLAGS) $(CFLAGS)
LDFLAGS := $(PKG_LDFLAGS) $(LDFLAGS)
targets =lines fill pen_dashes line_op
.PHONY: all clean
all: $(targets)
lines: lines.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
fill: fill.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
pen_dashes: pen_dashes.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
line_op: line_op.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
clean:
	$(RM) $(targets)
