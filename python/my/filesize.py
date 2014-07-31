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
import decimal

def visit(arg, dirname, names):
    print ("find in directory : %s" % dirname)
    total = 0
    for name in names:
        subname = os.path.join(dirname, name)
        if subname.endswith("pyc"):
            continue

        cnt = decimal.Decimal('0')
        if os.path.isfile(subname):
            cnt = decimal.Decimal('0')
            file = open(subname)
            for line in file:
                cnt = cnt + 1
            print ("%s `s lines: %d" % (subname, cnt))
            total += cnt
        else:
            print ("%s is not a file." % (subname))
    print
    print("total lines : %s" % total)

target="debconf.conf"
os.path.walk('/usr/lib/ubiquity', visit, target)
