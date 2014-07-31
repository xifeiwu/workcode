#!/usr/bin/python
import os
import sys
import apt
def printinfo(version):
    print("architecture:\t%s" % version.architecture)
    print("dependencies:\t%s" % version.dependencies)
    print("description:\t%s" % version.description)
    print("downloadable:\t%s" % version.downloadable)
    print("enhances:\t%s" % version.enhances)
    print("filename:\t%s" % version.filename)
    print("homepage:\t%s" % version.homepage)
    print("installed_size:\t%s" % version.installed_size)
    print("md5:\t%s" % version.md5)
    print("sha1:\t%s" % version.sha1)
    print("priority:\t%s" % version.priority)
    print("provides:\t%s" % version.provides)
    print("section:\t%s" % version.section)
    print("size:\t%s" % version.size)
    print("source_name:\t%s" % version.source_name)
    print("source_version:\t%s" % version.source_version)
    print("summary:\t%s" % version.summary)
    print("uris:\t%s" % version.uris)
    print("uri:\t%s" % version.uri)
    print("version:\t%s" % version.version)
    print("recommends:\t%s" % version.recommends)
    print
    print
    print "origin info:"   
    ori=version.origins[0]
    print("archive:\t%s" % ori.archive)
    print("component:\t%s" % ori.component)
    print("label:\t%s" % ori.label)
    print("origin:\t%s" % ori.origin)
    print("site:\t%s" % ori.site)
    print("trusted:\t%s" % ori.trusted)
    print

def main():
    print "in main function"
    cache = apt.Cache()
    cache.open(None)
    cache.upgrade()

    pkg=cache["cdos-upgrade"]
    newVersion = pkg.candidate
    oldVersion = pkg.installed
#    printinfo(newVersion)
    printinfo(oldVersion)

if __name__ == "__main__":
    main()
