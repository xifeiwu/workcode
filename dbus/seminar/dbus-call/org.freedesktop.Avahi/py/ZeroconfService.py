"""Simple Zeroconf service publication. Copyright 2008, Pierre of
stackp.online.fr. License appears to be "Do whatever you want".
Original code at http://stackp.online.fr/?p=35
"""

from __future__ import with_statement

import threading
import functools

import avahi
import gobject
import dbus
from dbus import DBusException
from dbus.mainloop.glib import DBusGMainLoop

__all__ = ["ZeroconfService"]


class ZeroconfPublisher(object):
    """A simple class to publish a network service with zeroconf using
    avahi.
    """
    def __init__(self, name, port, stype="_http._tcp",
                 domain="", host="", text=[]):
        self.name = name
        self.stype = stype
        self.domain = domain
        self.host = host
        self.port = port
        self.text = text
        self.bus = dbus.SystemBus()
        self.server = dbus.Interface(
            self.bus.get_object(
                avahi.DBUS_NAME,
                avahi.DBUS_PATH_SERVER),
            avahi.DBUS_INTERFACE_SERVER)

    def hostname(self):
        return self.server.GetHostNameFqdn()

    def publish(self):
        g = dbus.Interface(
            self.bus.get_object(avahi.DBUS_NAME,
                           self.server.EntryGroupNew()),
            avahi.DBUS_INTERFACE_ENTRY_GROUP)
        
        g.AddService(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC,dbus.UInt32(0),
                     self.name, self.stype, self.domain, self.host,
                     dbus.UInt16(self.port), self.text)

        g.Commit()
        self.group = g
    
    def unpublish(self):
        self.group.Reset()


class ZeroconfBrowser(object):
    """A simple class to listen and browse for zeroconf services.
    """
    def __init__(self, loop=None):
        """Initialise the browser, with the given event loop. If no
        external loop is provided, create one.
        """
        self.local_loop = False
        if loop is None:
            gobject.threads_init()
            loop = DBusGMainLoop()
            self.local_loop = True
        self.bus = dbus.SystemBus(mainloop=loop)

        self.server = dbus.Interface(
            self.bus.get_object(avahi.DBUS_NAME, '/'),
            'org.freedesktop.Avahi.Server')

        self.lock = threading.Lock()

    def __call__(self):
        if self.local_loop:
            gobject.MainLoop().run()

    def browse(self, type,
               interface=avahi.IF_UNSPEC,
               protocol=avahi.PROTO_UNSPEC,
               domain='local',
               flags=dbus.UInt32(0),
               handler=None,
               removal=None,
               allfornow=None,
               error=None):
        """Browse for a service asynchronously, calling either the
        supplied handler function, or calling this object's
        self.handler() method for every result found.
        """
        with self.lock:
            sbrowser = dbus.Interface(
                self.bus.get_object(
                    avahi.DBUS_NAME,
                    self.server.ServiceBrowserNew(
                        interface,
                        protocol,
                        type,
                        domain,
                        flags)
                    ),
                avahi.DBUS_INTERFACE_SERVICE_BROWSER)

            handler = functools.partial(self.discovery, handler=handler)
            removal = functools.partial(self.removal, handler=removal)
            allfornow = functools.partial(self.allfornow, handler=allfornow)
            error = functools.partial(self.browse_error, handler=error)
            
            sbrowser.connect_to_signal("ItemNew", handler)
            sbrowser.connect_to_signal("ItemRemove", removal)
            sbrowser.connect_to_signal("AllForNow", allfornow)
            sbrowser.connect_to_signal("Failure", error)

    def resolve(self, interface, protocol, name,
                type, domain, aprotocol, flags=dbus.UInt32(0),
                reply_handler=None, error_handler=None):
        """Resolve a service asynchronously, calling either the
        supplied handlers or this object's self.resolved() and
        self.error() methods.
        """
        with self.lock:
            if reply_handler is None:
                reply_handler = self.resolved

            error_handler = functools.partial(self.resolve_error, handler=error_handler)

            self.server.ResolveService(
                interface, protocol, name,
                stype, domain, avahi.PROTO_UNSPEC, dbus.UInt32(0), 
                reply_handler=reply_handler, error_handler=error_handler)

    def resolved(self, interface, protocol, name, type,
                 domain, host, aprotocol, address,
                 port, txt, flags, handler):
        """Callback used when a service has been resolved
        successfully. By default, simply prints 'Service resolved'.

        Parameters:

        interface - Interface number on this machine
        protocol - Protocol number (0 == TCP)
        name - Human-readable name for the service
        type - Service type (e.g. _http._tcp)
        domain - DNS domain of service (typically 'local')
        host - FQDN of service host
        aprotocol - ??
        address - IP address of service
        port - TCP/UDP port number of service
        txt - Array of text records for the service
        flags - avahi flags
        handler - ??
        """
        text = []
        for t in txt:
            x = "".join((chr(c) for c in t))
            text.append(x)
        
        if handler is not None:
            handler(int(interface), int(protocol), str(name), str(type),
                    str(domain), str(host), int(aprotocol), str(address),
                    int(port), text, int(flags)
                    )

    def resolve_error(self, exception, handler):
        """Callback used when a service has not been resolved.
        """
        if handler is not None:
            handler(exception)
        else:
            print "Resolution error:", exception

    def browse_error(self, *args, **kwargs):
        """Callback used when a browse request fails.
        """
        print "Browse Error:", args, kwargs

    def discovery(self, interface, protocol, name, stype, domain, flags, handler):
        """Callback used when a service has been found during service
        discovery. By default, attempts to resolve the service name.
        """
        if handler is not None:
            handler(int(interface), int(protocol), str(name),
                    str(stype), str(domain), int(flags))
        else:
            #print "Service found:", interface, protocol, name, stype, domain, flags
            reply_handler = functools.partial(self.resolved, handler=None)
            error_handler = functools.partial(self.resolve_error, handler=None)
            self.server.ResolveService(interface, protocol, name, stype,
                                       domain, avahi.PROTO_UNSPEC, dbus.UInt32(0),
                                       reply_handler=reply_handler,
                                       error_handler=error_handler)

    def removal(self, interface, protocol, name, type, domain, flags, handler):
        """Callback used when a service has been removed, during a
        browse operation.
        """
        if handler is not None:
            handler(int(interface), int(protocol), str(name),
                    str(type), str(domain), int(flags))

    def allfornow(self, handler):
        """Callback used when we think we have all of the services
        currently present on the local net.
        """
        if handler is not None:
            handler()
