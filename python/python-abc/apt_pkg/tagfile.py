#!/usr/bin/python
import apt_pkg

def main():
    tagf = apt_pkg.TagFile(open('/var/lib/dpkg/status'))
    for section in tagfile:
        print section['Package']
    sl = apt_pkg.SourceList()
    print sl.list
