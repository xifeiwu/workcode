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
targets = example-service example-client example-signal-emitter example-signal-recipient
.PHONY: all clean
all: $(targets)

example-service: example-service.c
	$(shell dbus-binding-tool --prefix=test_obj --mode=glib-server --output=example-service-glue.h ./example-service.xml)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example-client: example-client.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example-signal-emitter: example-signal-emitter.c
	$(shell dbus-binding-tool --prefix=test_object --mode=glib-server --output=example-signal-emitter-glue.h ./example-signal-emitter.xml)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

example-signal-recipient: example-signal-recipient.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(targets) example-service-glue.h example-signal-emitter-glue.h
