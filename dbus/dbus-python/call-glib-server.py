#!/usr/bin/env python

# Copyright (C) 2004-2006 Red Hat Inc. <http://www.redhat.com/>
# Copyright (C) 2005-2007 Collabora Ltd. <http://www.collabora.co.uk/>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

import sys
from traceback import print_exc

import dbus

def main():
    bus = dbus.SessionBus()

    try:
        remote_object = bus.get_object("org.ztb.Test",
                                       "/TestObj")

        # you can either specify the dbus_interface in each call...
        cnt_value = remote_object.Get(dbus_interface = "org.fmddlmyy.Test.Basic")
        add_result = remote_object.Add(4, 5, dbus_interface = "org.fmddlmyy.Test.Basic")
    except dbus.DBusException:
        print_exc()
        sys.exit(1)

    print (cnt_value)
    print (add_result)

    # ... or create an Interface wrapper for the remote object
#    iface = dbus.Interface(remote_object, "com.example.SampleInterface")
#    hello_reply_tuple = iface.GetTuple()
#    print hello_reply_tuple
#    hello_reply_dict = iface.GetDict()
#    print hello_reply_dict
    # D-Bus exceptions are mapped to Python exceptions
#    try:
#        iface.RaiseException()
#    except dbus.DBusException as e:
#        print str(e)

    # introspection is automatically supported
    print remote_object.Introspect(dbus_interface="org.freedesktop.DBus.Introspectable")

    if sys.argv[1:] == ['--exit-service']:
        iface.Exit()

if __name__ == '__main__':
    main()
