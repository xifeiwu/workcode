#!/usr/bin/env python
# encoding: utf-8
#
# Copyright (c) 2008 Doug Hellmann All rights reserved.
#
"""
"""

__version__ = "$Id$"
#end_pymotw_header

import imp
from imp_get_suffixes import module_types

print 'Package:'
f, filename, description = imp.find_module('example')
print module_types[description[2]], filename
print

print 'Sub-module:'
f, filename, description = imp.find_module('submodule', [filename])
print module_types[description[2]], filename

print 'gtk:'
f, filename, description = imp.find_module('gtk')
print module_types[description[2]], filename

print 'dl:'
f, filename, description = imp.find_module('dl')
print module_types[description[2]], filename

print 'urllib2:'
f, filename, description = imp.find_module('urllib2')
print module_types[description[2]], filename

print 'apt'
f, filename, description = imp.find_module('apt')
print module_types[description[2]], filename

print 'apt_pkg:'
f, filename, description = imp.find_module('apt_pkg')
print module_types[description[2]], filename

if f: f.close()
