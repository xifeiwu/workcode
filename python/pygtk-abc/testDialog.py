#!/usr/bin/env python
# -*- coding:UTF-8 -*-
import gtk
 
label = gtk.Label(u"龙昌博客  http://www.xefan.com")
dialog = gtk.Dialog(u"测试对话框",
                   None,
                   gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
                   (gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT,
                    gtk.STOCK_OK, gtk.RESPONSE_ACCEPT))
dialog.vbox.pack_start(label)
label.show()
checkbox = gtk.CheckButton("Useless checkbox")
dialog.action_area.pack_end(checkbox)
checkbox.show()
response = dialog.run()
dialog.destroy()
if response == gtk.RESPONSE_ACCEPT.real:
    print u"确定"
if response == gtk.RESPONSE_REJECT.real:
    print u"取消"
