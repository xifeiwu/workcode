#!/usr/bin/env python
import gtk
import pygtk

CDOS_LABEL = "CDOS"
model_check = 0
model_name = 1
model_levelpix = 2
model_oldversion = 3
model_newversion = 4
model_size = 5
model_strsize = 6
model_strlevel = 7
model_des = 8
model_warning = 9
model_extrainfo = 10
pkginfodict={}

#icon_busy = ''
#icon_up2date = ''
#icon_updates = ''
#icon_error = ''
#icon_unknown = ''
#icon_apply = ''        
icon_busy = "/usr/lib/linuxmint/mintUpdate/icons/base.svg"
icon_up2date = "/usr/lib/linuxmint/mintUpdate/icons/base-apply.svg"
icon_updates = "/usr/lib/linuxmint/mintUpdate/icons/base-info.svg"
icon_error = "/usr/lib/linuxmint/mintUpdate/icons/base-error2.svg"
icon_unknown = "/usr/lib/linuxmint/mintUpdate/icons/base.svg"
icon_apply = "/usr/lib/linuxmint/mintUpdate/icons/base-exec.svg"


LOGDIR = "/tmp/mintUpdate/"
LOG = ''
LOGFILE = ''

APP_HIDDEN = True
MODE = ''
PID = 0

STATUSBAR = gtk.Statusbar()
CONTEXT_ID = 5

def error_dialog(message):
    dialog = gtk.MessageDialog(None, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK, None)
    dialog.set_title(_("ERROR"))
    dialog.set_markup("<b>" + message + "</b>")
    label = gtk.Label(_("Contact us: cdos_support@iscas.ac.cn"))
    dialog.vbox.pack_start(label)
    dialog.set_default_size(400, 300)
    dialog.show_all()
    dialog.set_position(gtk.WIN_POS_CENTER)
    dialog.run()
    dialog.destroy()
    return False

def warning_dialog(message):
    dialog = gtk.MessageDialog(None, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_WARNING, gtk.BUTTONS_OK, None)
    dialog.set_title(_("WARNING"))
    dialog.set_markup("<b>" + message + "</b>")
    dialog.set_default_size(400, 300)
    dialog.show_all()
    dialog.set_position(gtk.WIN_POS_CENTER)
    dialog.run()
    dialog.destroy()
    return False
