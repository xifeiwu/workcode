#!/usr/bin/python
import os
import sys
import apt
def main():
    print "in main function"
    cache = apt.Cache()
    cache.open(None)
    cache.upgrade()
    changes = cache.get_changes()

#    pkg=cache["cos-upgrade"]
#    print("isinstalled:\t%s\tmarked_upgrade:\t%s" % (pkg.is_installed, pkg.marked_upgrade))
#    print("name:\t%s\nnew-ver:\t%s\nold-ver:\t%s" % (pkg.name, pkg.candidate.version, pkg.installed.version))

    pkgcnt=0
    for pkg in changes:
        if (pkg.is_installed and pkg.marked_upgrade):
            newVersion = pkg.candidate.version
            oldVersion = pkg.installed.version
            size = pkg.candidate.size
            sourcePackage = pkg.candidate.source_name
            description = pkg.candidate.description
            if (newVersion != oldVersion):    
                print("%s:\t%s\tsourcePackage:%s" % (pkg.id, pkg.name,sourcePackage))
                pkgcnt=pkgcnt+1
    print "cnt:\t", pkgcnt

#    for pkg in changes:
#        if (pkg.is_installed and pkg.marked_upgrade):
#            package = pkg.name
#            newVersion = pkg.candidate.version
#            oldVersion = pkg.installed.version
#            size = pkg.candidate.size
#            sourcePackage = pkg.candidate.source_name
#            description = pkg.candidate.description
#            if (newVersion != oldVersion):
#                resultString = u"UPDATE###%s###%s###%s###%s###%s###%s" % (package, newVersion, oldVersion, size, sourcePackage, description)
#                print resultString.encode('ascii', 'xmlcharrefreplace');

if __name__ == "__main__":
    main()
