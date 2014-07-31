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
import sys 
import re

def find_string_in_file(filename, string):
	print("argument1 : %s  --  argument2 : %s" % (filename, string))
	filename = sys.argv[1]
	string = sys.argv[2]
	file = open(filename)
	for line in file:
		match = re.search(string, line)
		if match:
			print("%s" % (line))
#            print(line)
	print("-------------------end----------------------")

filename = sys.argv[1]
string = sys.argv[2]
find_string_in_file(filename, string)
