#from gi.repository import Gtk, Gdk, GObject, GLib
#title = ('This installer must be run with administrative '
#         'privileges, and cannot continue without them.')
#dialog = Gtk.MessageDialog(
#    None, Gtk.DialogFlags.MODAL,
#    Gtk.MessageType.ERROR, Gtk.ButtonsType.CLOSE, title)
#dialog.run()

#!/usr/bin/env python
import gtk
import gtk.glade

def opendialog():
    dialog = gtk.MessageDialog(None, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK, None)
    dialog.set_title("ERROR")
    dialog.set_markup("<b>" + "Package cos-upgrade is not install correct.\nContact us: cos_ibp@iscas.ac.cn" + "</b>")
    dialog.set_default_size(400, 300)
    dialog.show_all()
    dialog.run()
    dialog.destroy()
    return False
opendialog()
