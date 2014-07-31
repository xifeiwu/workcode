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
def worker():
    """thread worker function"""
    print 'Worker'
    cnt = 0
    while True:
        print "in while worker"
        time.sleep(1)
        cnt = cnt + 1
        if(cnt > 10):
            break
    return

threads = []
for i in range(5):
    t = threading.Thread(target=worker)
    threads.append(t)
    t.start()
