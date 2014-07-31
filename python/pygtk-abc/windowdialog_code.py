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

global text
text = 0

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

def thread2refresh_textbuf(textview):
    textbuf = textview.get_buffer()
    global text
    while text < 40:
        text = text + 1
        print "text:", text
        gtk.gdk.threads_enter()
        textbuf.insert_at_cursor(("%s%d\n" % (u"你好，世界～", text)))
        end_mark = textbuf.get_insert()
        textview.scroll_to_mark(end_mark, 0.0)
        gtk.gdk.threads_leave()

def btn_accept_clicked(button, textview):
    print button.get_label()
    t = threading.Thread(target=thread2refresh_textbuf, args=(textview,))
    t.start()
 
def btn_cancel_clicked(button):
    print button.get_label()
    pid = os.getpid()    
    os.system("kill -9 %s &" % pid)

def openUpdatingDialog():
#    gtk.gdk.threads_enter()
    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.set_title("Update")
    window.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
    window.set_default_size(600, 400)

    vbox = gtk.VBox()
    label = gtk.Label()
    label.set_markup("<b>Status output:</b>")
    label.set_alignment(0, 0.5)
    scrolledWindow = gtk.ScrolledWindow()
    scrolledWindow.set_shadow_type(gtk.SHADOW_IN)
    scrolledWindow.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    textview = gtk.TextView()
    textbuf = gtk.TextBuffer()
    textview.set_buffer(textbuf)
    textbuf.set_text("hello, world.")
    scrolledWindow.add(textview)

    hbuttonbox = gtk.HButtonBox()
    hbuttonbox.set_spacing(50)
    hbuttonbox.set_layout(gtk.BUTTONBOX_CENTER)
    btn_accept = gtk.Button("accept")
    btn_accept.connect("clicked", btn_accept_clicked, textview)
    btn_cancel = gtk.Button("cancel")
    btn_cancel.connect("clicked", btn_cancel_clicked)
    hbuttonbox.pack_start(btn_accept)
    hbuttonbox.pack_end(btn_cancel)

    vbox.pack_start(label, False, False, 10)
    vbox.pack_start(scrolledWindow, True, True, 0)
    vbox.pack_start(hbuttonbox, False, False, 5)
    window.add(vbox)
    window.set_position(gtk.WIN_POS_CENTER)
    window.show_all()
    gtk.main()
#    gtk.gdk.threads_leave()

def worker():
    """thread worker function"""
    print 'Worker'
    cnt = 0
    while True:
        print "in while worker"
        time.sleep(0.3)
        cnt = cnt + 1
        if(cnt > 15):
            break
    return

#threads = []
#for i in range(5):
#    t = threading.Thread(target=worker)
#    threads.append(t)
#    t.start()

#t = threading.Thread(target=openUpdatingDialog)
#t.start()

t = threading.Thread(target=worker)
t.start()

openUpdatingDialog()
