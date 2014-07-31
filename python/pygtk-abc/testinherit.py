#!/usr/bin/env python
# coding: utf-8
import gtk
import gtk.glade
import tempfile
import subprocess
import time
import threading
import gobject
import os
class ChooseVBox(gtk.VBox):
    def __init__(self):
        gtk.VBox.__init__(self)
        self.pack()
    def pack(self):
        label = gtk.Label("hello, world.")
        label.set_markup("<b>The following custom will be exexuted:</b>")
        label.set_alignment(0, 0.5)
        self.pack_start(label)
        globalfunc()
    def testfunc():
        print "in test function."
def globalfunc():
    print "in global function."
window = gtk.Window()
#label = gtk.Label("test")
vbox = ChooseVBox()
window.add(vbox)
window.set_title("hello")
window.set_default_size(600, 400)
window.show_all()
gtk.main()
