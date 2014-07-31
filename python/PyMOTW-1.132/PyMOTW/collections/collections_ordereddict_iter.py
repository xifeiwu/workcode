#! /usr/bin/python3
# encoding: utf-8
#
# Copyright (c) 2010 Doug Hellmann.  All rights reserved.
#
"""Iterating over an OrderedDict
"""
#end_pymotw_header

import collections

print 'Regular dictionary:'
d = {}
d['中文简体'] = ('A', 'd')
d['b'] = ('B', 'd')
d['c'] = ('C', 'd')
d['d'] = ('D', 'd')
d['e'] = ('E', 'd')
for k, v in d.items():
    print k, v

print '\nOrderedDict:'
d = collections.OrderedDict()
d['a'] = ('A', 'd')
d['b'] = ('B', 'd')
d['c'] = ('C', 'd')
d['d'] = ('D', 'd')
d['e'] = ('E', 'd')
tmp=d.keys()
print("d : %s." % d)
print("tmp : %s." % tmp)

for k, v in d.items():
    print k, v
    
