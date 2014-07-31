#!/usr/bin/env python
import gtk
import gtk.glade
import tempfile
import subprocess
import time
import threading
import gobject
#gobject.threads_init()

gtk.gdk.threads_init()
def opendialog():
# | gtk.DIALOG_DESTROY_WITH_PARENT

    gtk.gdk.threads_enter()
    dialog = gtk.Dialog(None, None, gtk.DIALOG_MODAL, (gtk.STOCK_OK, gtk.RESPONSE_ACCEPT, gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT))
    dialog.set_title("Update")
    dialog.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
    dialog.set_default_size(480, 320)

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
    textview.show()

    scrolledWindow.add(textview)
    dialog.set_has_separator(True)
    dialog.set_default_response(gtk.RESPONSE_ACCEPT)
    dialog.vbox.pack_start(label, False, False, 10)
    dialog.vbox.add(scrolledWindow)
#    print("1:\t%r\n2:\t%r\n" % (dialog.action_area.get_children()[0], dialog.action_area.get_children()[1]))
#    dialog.action_area.set_child_packing(dialog.action_area.get_children()[0], True, True, 30, gtk.PACK_START)
#    dialog.action_area.set_child_packing(dialog.action_area.get_children()[1], True, True, 30, gtk.PACK_START)
    dialog.show_all()

    gtk.gdk.threads_leave()    
#    f = tempfile.NamedTemporaryFile()
#    popen = subprocess.Popen(['ping', 'www.baidu.com'], stdout=f)
    gtk.gdk.threads_enter()
    if dialog.run() == gtk.RESPONSE_ACCEPT:
        print "gtk.RESPONSE_ACCEPT"
        proceed = True
    else:
        proceed = False
    dialog.destroy()
    gtk.gdk.threads_leave()
    return

#dialog = threading.Thread(target=opendialog)
#dialog.start()

def worker():
    """thread worker function"""
    print 'Worker'
    cnt = 0
    while True:
        print "in while worker"
        time.sleep(0.3)
        cnt = cnt + 1
        if(cnt > 5):
            break
    return

for i in range(3):
    t = threading.Thread(target=worker)
    t.start()
opendialog()
#dialog = threading.Thread(target=opendialog)
#dialog.start()

#def refresh_text():
#    testcnt=0
#    text = "hello, world."
#    while True:
#        text = text + str(testcnt) + "\n"
##        textbuf.set_text(text)
#        testcnt = testcnt + 1
#        time.sleep(0.5)
#        print text
#t = threading.Thread(target=refresh_text)
#t.start()

