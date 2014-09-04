from _dbus_glib_bindings import DBusGMainLoop
import dbus, avahi
import gobject
from datetime import *
class ServiceBrowser():
    def __init__(self, service):
        loop = DBusGMainLoop(set_as_default=True)

        bus = dbus.SystemBus(mainloop=loop)

        self._server = dbus.Interface(bus.get_object(avahi.DBUS_NAME, avahi.DBUS_PATH_SERVER), avahi.DBUS_INTERFACE_SERVER)

        browser = self._server.ServiceBrowserNew(avahi.IF_UNSPEC, avahi.PROTO_INET, service, '', 0)
        listener = dbus.Interface(bus.get_object(avahi.DBUS_NAME, browser), avahi.DBUS_INTERFACE_SERVICE_BROWSER)

        listener.connect_to_signal("ItemNew", self.item_new_handler)
        listener.connect_to_signal("ItemRemove", self.item_remove_handler)
        listener.connect_to_signal("AllForNow", self.all_for_now_handler)
        listener.connect_to_signal("CacheExhausted", self.cache_exhausted_handler)
        listener.connect_to_signal("Failure", self.failure_handler)

        self._mainloop = gobject.MainLoop()


    def service_resolved(self, *args):
        print 'New service: name = %s; address = %s; port = %s' % (args[2], args[7], args[8])


    def print_error(self, *args):
        print 'error_handler'
        print args[0]

    def item_new_handler(self, interface, protocol, name, stype, domain, flags):
        self._server.ResolveService(interface, protocol, name, stype, domain, avahi.PROTO_UNSPEC, dbus.UInt32(0), reply_handler=self.service_resolved, error_handler=self.print_error)


    def item_remove_handler(self, interface, protocol, name, stype, domain, flags):
        print "Removed service: %s" % name


    def all_for_now_handler(self):
        print "that's all for now"
        self._mainloop.quit()


    def cache_exhausted_handler(self):
        print "cache exhausted"


    def failure_handler(self, error):
        print "failure: %s" % error


    def browse(self):
        self._mainloop.run()


def browse_avahi(service):
    browser = ServiceBrowser(service)
    browser.browse()

def main():
    t0 = datetime.utcnow()
    print "browsing ..."
    browse_avahi('_http._tcp')
    print "done"
    print datetime.utcnow()-t0

if __name__ == '__main__':
    main()
