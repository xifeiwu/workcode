#!/usr/bin/python
"""Example for packages. Print all essential and important packages"""

import apt_pkg

def main():
    """Main."""
    apt_pkg.init_config()
    apt_pkg.init_system()
    cache = apt_pkg.Cache()
    print("depends_count:\t%s" % cache.depends_count)
    print("group_count:\t%s" % cache.group_count)
    print("package_count:\t%s" % cache.package_count)
    print("package_file_count:\t%s" % cache.package_file_count)
    print("provides_count:\t%s" % cache.provides_count)
    print("ver_file_count:\t%s" % cache.ver_file_count)
    print("version_count:\t%s" % cache.version_count)
    print "Essential packages:"
    for pkg in cache.packages:
        if pkg.essential:
            print " ", pkg.name
    print "Important packages:"
    for pkg in cache.packages:
        if pkg.important:
            print " ", pkg.name
    print
    
    for pkg in cache.packages:
        print pkg.id, pkg.name

    for pkgfile in cache.file_list:
        print 'Package-File:', pkgfile.filename
#        print 'Index-Type:', pkgfile.index_type # 'Debian Package Index'
#        if pkgfile.not_source:
#            print 'Source: None'
#        else:
#            if pkgfile.site:
#                # There is a source, and a site, print the site
#                print 'Source:', pkgfile.site
#            else:
#                # It seems to be a local repository
#                print 'Source: Local package file'
#        if pkgfile.not_automatic:
#            # The system won't be updated automatically (eg. experimental)
#            print 'Automatic: No'
#        else:
#            print 'Automatic: Yes'
        print 'id:', pkgfile.id
#        print 'archive:', pkgfile.archive
#        print 'architecture:', pkgfile.architecture
#        print 'label:', pkgfile.label
#        print 'origin:', pkgfile.origin
#        print 'size', pkgfile.size
#        print 'version:', pkgfile.version
        print
if __name__ == "__main__":
    main()
