#!/usr/bin/env python
# encoding: utf-8
#
# Copyright (c) 2008 Doug Hellmann All rights reserved.
#
"""Traverse a directory tree to find a string needed.
"""

__version__ = "$Id$"
#end_pymotw_header

import os
import os.path
import pprint
import re

def visit(arg, dirname, names):
    print ("find in directory : %s" % dirname)
    for name in names:
        subname = os.path.join(dirname, name)
        if subname.endswith("pyc"):
            continue
        
        match = re.search(arg, subname)
        if match:
            print("-------notice------ find string %s in filename :  %s \n %s" % (arg, subname, subname))

        if os.path.isfile(subname):
            print ("find in file %s" % name)
            file = open(subname)
            for line in file:
                match = re.search(arg, line)
                if match:
                    print("-------notice------ find string %s in file %s \n %s" % (arg, subname, line))
        else:
            print ("find in file %s : %s is not a file." % (name, name))
    print

target="linuxmint"
#"ubi-console-setup"
#"KeyboardQuery"
#"append_page"
#"add_history"
#"optional_widgets"
#"filter_class"
#"PartmanCommit"
#"FilteredCommand"
#"gtk_widget_size_allocate"
#"752"
#"debconf.conf"
#"templatedb"
#"debconf-copydb"
#"ui_class"
#"ubiquity/text"
#"debconf"
#"Mint"
#"ubiquity/text/use_crypto_desc"
#"PageGtk"
#"ubi-partman"
os.path.walk('/home/xifei/.mozilla', visit, target)
#/usr/lib/ubiquity
