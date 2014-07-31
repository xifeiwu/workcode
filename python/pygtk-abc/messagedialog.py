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
def celldatafunction_checkbox(column, cell, model, iter):
    cell.set_property("activatable", True)
    checked = model.get_value(iter, 0)
    if (checked == "true"):
        cell.set_property("active", True)
    else:
        cell.set_property("active", False)

def opendialog():
# gtk.MESSAGE_WARNING 
    dialog = gtk.MessageDialog(None, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_ERROR, gtk.BUTTONS_OK_CANCEL, None)
    dialog.set_title("package cos-upgrade ERROR")
    dialog.set_markup("<b>" + "Package cos-upgrade is not install correct." + "</b>")
    #dialog.format_secondary_markup("<i>" + _("All available upgrades for this package will be ignored.") + "</i>")
    dialog.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
    dialog.set_default_size(640, 480)

    removals = ['aaa', 'bbb', 'ccc', 'ddd', 'eee', 'fff']
    label = gtk.Label()
    if len(removals) == 1:
        label.set_text(_("The following package will be removed:"))
    else:
        label.set_text("The following %d packages will be removed:" % len(removals))
    label.set_alignment(0, 0.5)
    scrolledWindow = gtk.ScrolledWindow()
    scrolledWindow.set_shadow_type(gtk.SHADOW_IN)
    scrolledWindow.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    treeview = gtk.TreeView()

    cr = gtk.CellRendererToggle()
#    cr.connect("toggled", toggled, treeview_update, statusbar, context_id)
    column1 = gtk.TreeViewColumn("Upgrade", cr)
    column1.set_cell_data_func(cr, celldatafunction_checkbox)

    column2 = gtk.TreeViewColumn("Operation", gtk.CellRendererText(), text=1)
    column2.set_sort_column_id(1)
    column2.set_resizable(True)

    treeview.append_column(column1)
    treeview.append_column(column2)

    treeview.set_headers_clickable(False)
    treeview.set_reorderable(False)
    treeview.set_headers_visible(False)

    model = gtk.TreeStore(str, str)
    removals.sort()
    for pkg in removals:
        iter = model.insert_before(None, None)
        model.set_value(iter, 0, "true")
        model.set_value(iter, 1, pkg)
    treeview.set_model(model)
    treeview.show()
    scrolledWindow.add(treeview)
#    dialog.vbox.add(label)
    dialog.vbox.add(scrolledWindow)

    dialog.show_all()
    if dialog.run() == gtk.RESPONSE_OK:
        proceed = True
    else: 
        proceed = False
    dialog.destroy()

opendialog()
