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
gtk.gdk.threads_init()
def show_pkg_info_window(text): 
    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.set_title("Package Information")
    window.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
    window.set_default_size(400, 250)
    window.set_position(gtk.WIN_POS_CENTER)
    vbox = gtk.VBox()
    label = gtk.Label(text)
    scrolledWindow = gtk.ScrolledWindow()
    scrolledWindow.set_shadow_type(gtk.SHADOW_IN)
    scrolledWindow.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
#    scrolledWindow.add(label)
    hbuttonbox = gtk.HButtonBox()
    hbuttonbox.set_spacing(50)
    hbuttonbox.set_layout(gtk.BUTTONBOX_CENTER)
    btn_ok = gtk.Button("OK")
    def btn_ok_clicked(button):
        window.hide()
    btn_ok.connect("clicked", btn_ok_clicked)
    hbuttonbox.pack_start(btn_ok)
    vbox.pack_start(label)
    vbox.pack_start(hbuttonbox, False, False)
    window.add(vbox)
    window.show_all()
    gtk.main()

show_pkg_info_window("test")
