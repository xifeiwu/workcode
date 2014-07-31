#!/usr/bin/env python

"""Simple example of using os.fork to create a new child process.

"""

__module_id__ = "$Id$"
#end_pymotw_header

import os

pid = os.fork()
print "pid : ", pid
if pid:
    print 'Child process id:', pid
else:
    print 'I am the child'
