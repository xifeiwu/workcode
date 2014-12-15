import avahi
import dbus

__all__ = ["ZeroconfService"]

class ZeroconfService:
    """A simple class to publish a network service with zeroconf using
    avahi.

    """

    def __init__(self, name, port, stype="_http._tcp",
                 domain="", host="", text="username:xifei"):
        self.name = name
        self.stype = stype
        self.domain = domain
        self.host = host
        self.port = port
        self.text = text

    def publish(self):
        bus = dbus.SystemBus()
        server = dbus.Interface(
                         bus.get_object(
                                 avahi.DBUS_NAME,
                                 avahi.DBUS_PATH_SERVER),
                        avahi.DBUS_INTERFACE_SERVER)

        g = dbus.Interface(
                    bus.get_object(avahi.DBUS_NAME,
                                   server.EntryGroupNew()),
                    avahi.DBUS_INTERFACE_ENTRY_GROUP)
        print "avahi.DBUS_INTERFACE_ENTRY_GROUP: ", avahi.DBUS_INTERFACE_ENTRY_GROUP
        g.AddService(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC,dbus.UInt32(0),
                     self.name, self.stype, self.domain, self.host,
                     dbus.UInt16(self.port), self.text)
        print "state:" ,g.GetState()
        g.Commit()
        self.group = g

    def unpublish(self):
        self.group.Reset()


def test():
    userA = ZeroconfService(name="Test-UserA", port=8080)
    userA.publish()
    userB = ZeroconfService(name="Test-UserB", port=7777)
    userB.publish()
    raw_input("Press any key to unpublish the service ")
    userA.unpublish()
    userB.unpublish()


if __name__ == "__main__":
    test()
