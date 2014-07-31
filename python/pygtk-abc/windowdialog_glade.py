#!/usr/bin/env python
# coding: utf-8
import os
import commands
import sys
import string
import gtk
import gtk.glade
import gobject
import tempfile
import threading
import time
import gettext
import fnmatch
import urllib2
global text
text = u""

def window_close(widget, wTree):
    global text
    textview = wTree.get_widget("updating_textview")
    textbuf = textview.get_buffer()
    textview.set_buffer(textbuf)
    # coding=gtk
    for i in range(40):
        text = text + u"hello, world.世界\n"
    textbuf.set_text(text)
    end_mark = textbuf.get_insert()
    textview.scroll_to_mark(end_mark, 0.0)
#    print "end_iter:", end_iter.get_line()
#    textbuf.set_text(text.encode('ascii', 'xmlcharrefreplace'))

def openUpdatingDialog():
    gladefile = "/usr/lib/linuxmint/mintUpdate/mintUpdate.glade"
    wTree = gtk.glade.XML(gladefile, "updating_window")
    wTree.get_widget("updating_window").set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")

    wTree.get_widget("button_close").connect("clicked", window_close, wTree)
    textview = wTree.get_widget("updating_textview")
    textbuf = gtk.TextBuffer()
    textview.set_buffer(textbuf)
    textbuf.set_text("hello, world.")
    textview.show()

    # Get the window socket (needed for synaptic later on)    
    if os.getuid() != 0 :
        # If we're not in root mode do that (don't know why it's needed.. very weird)
        socket = gtk.Socket()
        vbox.pack_start(socket, True, True, 0)
        socket.show()
        window_id = repr(socket.get_id())
    wTree.get_widget("updating_window").show_all()
    gtk.main()

#def worker():
#    """thread worker function"""
#    print 'Worker'
#    cnt = 0
#    while True:
#        print "in while worker"
#        time.sleep(1)
#        cnt = cnt + 1
#        if(cnt > 10):
#            break
#    return
#
#threads = []
#for i in range(5):
#    t = threading.Thread(target=worker)
#    threads.append(t)
#    t.start()

#t = threading.Thread(target=openUpdatingDialog)
#t.start()
openUpdatingDialog()
