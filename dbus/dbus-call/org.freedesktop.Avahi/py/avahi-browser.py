# http://avahi.org/wiki/PythonBrowseExample
import dbus, gobject, avahi
from dbus import DBusException
from dbus.mainloop.glib import DBusGMainLoop

# Looks for iTunes shares

TYPE = "_http._tcp"

def service_resolved(*args):
    result = args
    print("\n== Device Info ==")
    print('interface: %s' % (result[0]))
    print('protocol: %s' % result[1])
    print('name: %s' % result[2])
    print('stype: %s' % result[3])
    print('domain: %s' % result[4])
    print('host: %s' % result[5])
    print('aprotocol: %s' % result[6])
    print('address: %s' % result[7])
    print('port: %s' % result[8])
    txtByteArray = result[9]
    txtString = ''
    for array in txtByteArray:
        txtString += ( ''.join([chr(byte) for byte in array]) + '; ')    
    print('txt: %s' % txtString)
    print('flags: %s' % result[10])

def print_error(*args):
    print 'error_handler'
    print args[0]

def service_resolve_hostname(*args):
    print 'service resolved'
    print 'name:', args[0]
    print 'address:', args[1]
    print 'port:', args[2]
    print 'port:', args[3]
    print 'port:', args[4]
    print 'port:', args[5]

def ItemAdd(interface, protocol, name, stype, domain, flags):
    print "ItemNew: %d, %d, %s, %s, %s, %d.\n" % (interface, protocol, name, stype, domain, flags)
    if flags & avahi.LOOKUP_RESULT_LOCAL:
            # local service, skip
            pass
    server.ResolveService(interface, protocol, name, stype, 
        domain, avahi.PROTO_UNSPEC, dbus.UInt32(0), 
        reply_handler=service_resolved, error_handler=print_error)
#    server.ResolveHostName(interface, protocol, name,
#        avahi.PROTO_UNSPEC, dbus.UInt32(0),
#        reply_handler=service_resolve_hostname, error_handler=print_error)
#    print "===================================="
#    print "parameters in ResolveService:%d, %d, %s, %s, %s, %d, %d" % (interface, protocol, name, stype, domain, avahi.PROTO_UNSPEC, dbus.UInt32(0))
#    print "===================================="

def ItemRemove(interface, protocol, name, stype, domain, flags):
    print "Remove service '%s' type '%s' domain '%s'" % (name, stype, domain)

def AllForNow():
    print "that's all for now"

def CacheExhausted():
    print "cache exhausted"


def Failure( error):
    print "failure: %s" % error

loop = DBusGMainLoop()

bus = dbus.SystemBus(mainloop=loop)

server = dbus.Interface( bus.get_object(avahi.DBUS_NAME, '/'), 'org.freedesktop.Avahi.Server')

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
sbrowser.connect_to_signal("AllForNow", AllForNow)
sbrowser.connect_to_signal("CacheExhausted", CacheExhausted)
sbrowser.connect_to_signal("Failure", Failure)

gobject.MainLoop().run()
