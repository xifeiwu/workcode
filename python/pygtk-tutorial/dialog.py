#!/usr/bin/env python

# message.py -- example program illustrating use of message dialog widget 

import pygtk
pygtk.require('2.0')
import gtk

if __name__ == "__main__":
    dialog = gtk.Dialog(title="gtk-dialog", parent=None, flags=gtk.DIALOG_MODAL)
#    dialog.set_default_size(600, 400)
    label = gtk.Label("Dialog ABC")
    dialog.vbox.pack_start(label, True, True, 0)
    btnok = gtk.Button("button-ok")
    dialog.action_area.pack_start(btnok, True, True, 0)
    btncancel = gtk.Button("button-cancel")
    dialog.action_area.pack_start(btncancel, True, True, 0)
    dialog.show_all()
    dialog.run()
