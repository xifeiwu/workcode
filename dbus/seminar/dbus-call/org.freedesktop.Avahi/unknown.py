class AvahiServicePublisher():

    def __init__(self):
        self.group = None
        bus = dbus.SystemBus()
        server = dbus.Interface(
            bus.get_object(
                avahi.DBUS_NAME,
                avahi.DBUS_PATH_SERVER),
            avahi.DBUS_INTERFACE_SERVER)

        self.group = dbus.Interface(
            bus.get_object(avahi.DBUS_NAME,
                           server.EntryGroupNew()),
            avahi.DBUS_INTERFACE_ENTRY_GROUP)

    def publish(self, name, port, stype="_http._tcp", domain="", host="", text=""):
        self.group.AddService(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC, dbus.UInt32(0),
                              name, stype, domain, host,
                              dbus.UInt16(port), text)

        self.group.Commit()

    def unpublish(self):
        self.group.Reset()


from _dbus_glib_bindings import DBusGMainLoop
import avahi
import logging
import avahi
import dbus
import gobject
from threading import Thread

__author__ = 'michael'


class AvahiServiceDetector():
    TYPE = "_http._tcp"

    def __init__(self, name, on_resolve_callback):
        self.on_resolve_callback = on_resolve_callback
        self.name = name
        self.logger = logging.getLogger(self.__class__.__name__)
        self.logger.setLevel(logging.DEBUG)
        gobject.threads_init()
        loop = DBusGMainLoop()
        bus = dbus.SystemBus(mainloop=loop)
        self.server = dbus.Interface(bus.get_object(avahi.DBUS_NAME, '/'),
                                     'org.freedesktop.Avahi.Server')

        sbrowser = dbus.Interface(bus.get_object(avahi.DBUS_NAME,
                                                 self.server.ServiceBrowserNew(avahi.IF_UNSPEC,
                                                                          avahi.PROTO_UNSPEC, self.TYPE, 'local',
                                                                          dbus.UInt32(0))),
                                  avahi.DBUS_INTERFACE_SERVICE_BROWSER)

        sbrowser.connect_to_signal("ItemNew", self.myhandler)
        thread = Thread(None, self.run_loop)
        thread.daemon = True
        thread.start()

    def get_available_ips(self):
        pass

    def run_loop(self):
        gobject.MainLoop().run()

    def myhandler(self, interface, protocol, name, stype, domain, flags):
        print "Found service '%s' type '%s' domain '%s' " % (name, stype, domain)

        if flags & avahi.LOOKUP_RESULT_LOCAL:
            # local service, skip
            pass

        self.server.ResolveService(interface, protocol, name, stype,
                                   domain, avahi.PROTO_UNSPEC, dbus.UInt32(0),
                                   reply_handler=self.service_resolved, error_handler=self.print_error)
    def service_resolved(self, *args):
        name = args[2]
        address = args[7]
        port = args[8]
        self.logger.info("Service is resolved. Name is {}, address is {}, port is {}".format(name, address, port))

        if not self.is_mac(address) and name == self.name:
            self.on_resolve_callback(name, address, port)

    def is_mac(self, address):
        return ':' in address

    def print_error(self, *args):
        print 'error_handler'
        print args[0]
