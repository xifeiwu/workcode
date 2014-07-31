#!/usr/bin/env python

import codecs
import os
import sys
import textwrap

FILEDIR = os.path.dirname(__file__)
CURDIR = os.getcwd()
ROOTDIR = os.path.abspath(CURDIR)
DEMODIR = os.path.join(ROOTDIR, 'demos')

print("file name : %s" %  __file__)
print("dirname : %s" % FILEDIR)
print("cur dir : %s" % CURDIR)
print("abspath : %s" % ROOTDIR)
print("demo dir : %s" % DEMODIR)

print("relpath for DEMODIR in ROOTDIR : %s" % (os.path.relpath(DEMODIR, ROOTDIR)))
