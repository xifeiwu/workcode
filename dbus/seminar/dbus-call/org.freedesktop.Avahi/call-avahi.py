# http://avahi.org/wiki/PythonBrowseExample
import dbus, gobject, avahi
from dbus import DBusException
from dbus.mainloop.glib import DBusGMainLoop

# Looks for iTunes shares

TYPE = "_http._tcp"

def service_resolved(*args):
    print 'service resolved'
    print 'name:', args[2]
    print 'address:', args[7]
    print 'port:', args[8]

def print_error(*args):
    print 'error_handler'
    print args[0]

def ItemAdd(interface, protocol, name, stype, domain, flags):
    print "Found service '%s' type '%s' domain '%s' " % (name, stype, domain)

    if flags & avahi.LOOKUP_RESULT_LOCAL:
            # local service, skip
            pass

    server.ResolveService(interface, protocol, name, stype, 
        domain, avahi.PROTO_UNSPEC, dbus.UInt32(0), 
        reply_handler=service_resolved, error_handler=print_error)
    print "===================================="
    print "parameters in ResolveService:%d, %d, %s, %s, %s, %d, %d" % (interface, protocol, name, stype, domain, avahi.PROTO_UNSPEC, dbus.UInt32(0))
    print "===================================="

def ItemRemove(interface, protocol, name, stype, domain, flags):
    print "service '%s' type '%s' domain '%s' Remove" % (name, stype, domain)
    print "Addation: interface '%r' protocol '%r' flags '%r'" % (interface, protocol, flags)

loop = DBusGMainLoop()

bus = dbus.SystemBus(mainloop=loop)

server = dbus.Interface( bus.get_object(avahi.DBUS_NAME, '/'),
        'org.freedesktop.Avahi.Server')
print "parameters in ServiceBrowserNew:%d, %d, %s, %s, %d" % (avahi.IF_UNSPEC, avahi.PROTO_UNSPEC, TYPE, 'local', dbus.UInt32(0))
service_browser_path = server.ServiceBrowserNew(avahi.IF_UNSPEC,
            avahi.PROTO_UNSPEC, TYPE, 'local', dbus.UInt32(0))
sbrowser = dbus.Interface(bus.get_object(avahi.DBUS_NAME,service_browser_path),
        avahi.DBUS_INTERFACE_SERVICE_BROWSER)

print "avahi.DBUS_NAME: %s" % (avahi.DBUS_NAME)
print "avahi.DBUS_PATH_SERVER: %s" % (avahi.DBUS_PATH_SERVER)
print "avahi.DBUS_INTERFACE_SERVER: %s" % (avahi.DBUS_INTERFACE_SERVER)
print "Service Browser Path: %s" % (service_browser_path)
print "avahi.DBUS_INTERFACE_SERVICE_BROWSER: %s" % (avahi.DBUS_INTERFACE_SERVICE_BROWSER)
print "avahi.PROTO_UNSPEC: %s" % (avahi.PROTO_UNSPEC)

sbrowser.connect_to_signal("ItemNew", ItemAdd)
sbrowser.connect_to_signal("ItemRemove", ItemRemove)

gobject.MainLoop().run()
