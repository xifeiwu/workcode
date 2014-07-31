#!/usr/bin/env python
# encoding: utf-8
#
# Copyright (c) 2008 Doug Hellmann All rights reserved.
#
"""Creating and waiting for a thread.
"""
#end_pymotw_header

import threading
import time
class inherit(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.cnt=0
    def run(self):
        while(self.cnt < 10):
            print self.cnt
            self.cnt+=1
            time.sleep(2)
i=inherit("abc")
i.start()
