#!/usr/bin/env python
# coding: utf-8
import gtk
import tempfile
import subprocess
import time
import threading
import gobject
import os
gtk.gdk.threads_init()

class ChooseVBox(gtk.VBox):
    def __init__(self, w, h):
        gtk.VBox.__init__(self)
        self.set_size_request(w, h)
        self.pack()

    def celldatafunction_checkbox(self, column, cell, model, iter):
        cell.set_property("activatable", True)
        checked = model.get_value(iter, 0)
        if (checked == "true"):
            cell.set_property("active", True)
        else:
            cell.set_property("active", False)

    def toggled(self, renderer, path, treeview):
        model = treeview.get_model()
        iter = model.get_iter(path)
        if (iter != None):
            checked = model.get_value(iter, 0)
            if (checked == "true"):
                model.set_value(iter, 0, "false")
            else:
                model.set_value(iter, 0, "true")    

    def btn_accept_clicked(self, button, treeview):
        print button.get_label()
        model = treeview.get_model()
        iter = model.get_iter_first()
        operations = []
        while (iter != None):
            checked = model.get_value(iter, 0)
            if (checked == "true"):
                operations.append(model.get_value(iter, 1))
            iter = model.iter_next(iter)
        print "Your selection:", operations
        t = threading.Thread(target=redirect_vbox)
        t.start()

    def btn_cancel_clicked(self, button):
        print button.get_label()
        window.hide()
        pid = os.getpid()    
        os.system("kill -9 %s &" % pid)

    def pack(self):
        label = gtk.Label()
        label.set_markup("<b>The following custom will be exexuted:</b>")
        label.set_alignment(0, 0.5)

        self.treeview_choose = gtk.TreeView()
        cr = gtk.CellRendererToggle()
        cr.connect("toggled", self.toggled, self.treeview_choose)
        column1 = gtk.TreeViewColumn("Order", cr)
        column1.set_cell_data_func(cr, self.celldatafunction_checkbox)
        column2 = gtk.TreeViewColumn("Description", gtk.CellRendererText(), text=1)
        column2.set_sort_column_id(1)
        column2.set_resizable(True)
        self.treeview_choose.append_column(column1)
        self.treeview_choose.append_column(column2)
        operations = ['aaa', 'bbb', 'ccc', 'ddd', 'eee', 'fff']
        model = gtk.TreeStore(str, str)
        for oper in operations:
            iter = model.insert_before(None, None)
            model.set_value(iter, 0, "true")
            model.set_value(iter, 1, oper)
        self.treeview_choose.set_model(model)
        self.treeview_choose.set_headers_clickable(False)
        self.treeview_choose.set_reorderable(False)

        scrolledWindow = gtk.ScrolledWindow()
        scrolledWindow.set_shadow_type(gtk.SHADOW_IN)
        scrolledWindow.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        scrolledWindow.add(self.treeview_choose)

        hbuttonbox = gtk.HButtonBox()
        hbuttonbox.set_spacing(50)
        hbuttonbox.set_layout(gtk.BUTTONBOX_CENTER)
        btn_accept = gtk.Button("accept")
        btn_accept.connect("clicked", self.btn_accept_clicked, self.treeview_choose)
        btn_cancel = gtk.Button("cancel")
        btn_cancel.connect("clicked", self.btn_cancel_clicked)
        hbuttonbox.pack_start(btn_accept)
        hbuttonbox.pack_end(btn_cancel)

        self.pack_start(label, False, False, 10)
        self.pack_start(scrolledWindow, True, True, 0)
        self.pack_start(hbuttonbox, False, False, 5)


class ProcessVBox(gtk.VBox):
    def __init__(self, w, h):
        gtk.VBox.__init__(self)
        self.text = 0
        self.set_size_request(w, h)
        self.pack()
    def thread2refresh_textbuf(self, textview):
        textbuf = textview.get_buffer()
        while self.text < 40:
            self.text = self.text + 1
            print "text:", self.text
            gtk.gdk.threads_enter()
            textbuf.insert_at_cursor(("%s%d\n" % (u"你好，世界～", self.text)))
            end_mark = textbuf.get_insert()
            textview.scroll_to_mark(end_mark, 0.0)
            gtk.gdk.threads_leave()
    def btn_accept_clicked(self, button, textview):
        print button.get_label()
        t = threading.Thread(target=self.thread2refresh_textbuf, args=(textview,))
        t.start()     
    def btn_cancel_clicked(self, button):
        print button.get_label()
        pid = os.getpid()    
        os.system("kill -9 %s &" % pid)
    def pack(self):
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
        btn_accept.connect("clicked", self.btn_accept_clicked, textview)
        btn_cancel = gtk.Button("cancel")
        btn_cancel.connect("clicked", self.btn_cancel_clicked)
        hbuttonbox.pack_start(btn_accept)
        hbuttonbox.pack_end(btn_cancel)

        self.pack_start(label, False, False, 10)
        self.pack_start(scrolledWindow, True, True, 0)
        self.pack_start(hbuttonbox, False, False, 5)

global fix
global choose_x
global process_x
global vbox_choose
global vbox_process
global width
global height

def redirect_vbox():
    global fix
    global choose_x
    global process_x
    global vbox_choose
    global vbox_process
    global width
    global height

    step = 120
    while(process_x > 0):
        choose_x = choose_x - step
        process_x = process_x - step
        gtk.gdk.threads_enter()
        fix.move(vbox_choose, choose_x, 0)
        fix.move(vbox_process, process_x, 0)
        gtk.gdk.threads_leave()
        time.sleep(0.1)
        #print choose_x, process_x
    if(process_x < 0):
        choose_x = -1 * width
        process_x = 0
        gtk.gdk.threads_enter()
        fix.move(vbox_choose, choose_x, 0)
        fix.move(vbox_process, process_x, 0)
        gtk.gdk.threads_leave()
        

width = 600
height = 400
choose_x = 0
process_x = width

vbox_choose = ChooseVBox(width, height)
vbox_process = ProcessVBox(width, height)
fix = gtk.Fixed()
fix.put(vbox_process, process_x, 0)
fix.put(vbox_choose, choose_x, 0)
fix.show()

window = gtk.Window(gtk.WINDOW_TOPLEVEL)
window.set_title("Update")
window.set_icon_from_file("/usr/lib/linuxmint/mintUpdate/icons/base.svg")
window.set_default_size(width, height)
window.set_geometry_hints(window, width, height, width, height)
window.set_position(gtk.WIN_POS_CENTER)
window.add(fix)
window.show_all()
gtk.main()

#hbuttonbox.set_sensitive(False)
