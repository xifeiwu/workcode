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
    dialog = gtk.Dialog(None, None, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, (gtk.STOCK_OK, gtk.RESPONSE_ACCEPT, gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT))
    dialog.set_title("Update")
#    dialog.add_button(gtk.STOCK_OK, gtk.RESPONSE_OK)    
    dialog.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
    dialog.set_default_size(480, 320)

    removals = ['1.aaa', '2.bbb', '3.ccc', '4.ddd', '5.eee', '6.fff']
    label = gtk.Label()
    if len(removals) == 1:
        label.set_text(_("The following package will be removed:"))
    else:
        label.set_markup("<b>The following %d packages will be removed:</b>" % len(removals))
    label.set_alignment(0, 0.5)
    scrolledWindow = gtk.ScrolledWindow()
    scrolledWindow.set_shadow_type(gtk.SHADOW_IN)
    scrolledWindow.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    treeview = gtk.TreeView()

    cr = gtk.CellRendererToggle()
#    cr.connect("toggled", toggled, treeview_update, statusbar, context_id)
    column1 = gtk.TreeViewColumn("Upgrade", cr)
    column1.set_cell_data_func(cr, celldatafunction_checkbox)

    column2 = gtk.TreeViewColumn("Operation", gtk.CellRendererText(), text=0)
    column2.set_sort_column_id(0)
    column2.set_resizable(True)

#    treeview.append_column(column1)
    treeview.append_column(column2)

    treeview.set_headers_clickable(False)
    treeview.set_reorderable(False)
    treeview.set_headers_visible(False)

    model = gtk.TreeStore(str)
    removals.sort()
    for pkg in removals:
        iter = model.insert_before(None, None)
#        model.set_value(iter, 0, "true")
        model.set_value(iter, 0, pkg)
    treeview.set_model(model)
    treeview.show()
    scrolledWindow.add(treeview)
    dialog.set_has_separator(True)
    dialog.set_default_response(gtk.RESPONSE_ACCEPT)
    dialog.vbox.pack_start(label, False, False, 10)
    dialog.vbox.add(scrolledWindow)
    print("1:\t%r\n2:\t%r\n" % (dialog.action_area.get_children()[0], dialog.action_area.get_children()[1]))
    dialog.action_area.set_child_packing(dialog.action_area.get_children()[0], True, True, 30, gtk.PACK_START)
    dialog.action_area.set_child_packing(dialog.action_area.get_children()[1], True, True, 30, gtk.PACK_START)
    dialog.show_all()
    if dialog.run() == gtk.RESPONSE_ACCEPT:
        print "gtk.RESPONSE_ACCEPT"
        proceed = True
    else:
        proceed = False
    dialog.destroy()

opendialog()
