#!/usr/bin/python
"""Check the archive for missing dependencies"""
import apt_pkg


def fmt_dep(dep):
    """Format a Dependency object [of apt_pkg] as a string."""
    ret = dep.target_pkg.name
    if dep.target_ver:
        ret += " (%s %s)" % (dep.comp_type, dep.target_ver)
    return ret


def check_version(pkgver):
    """Check the version of the package"""
    missing = []

    for or_group in pkgver.depends_list.get("Pre-Depends", []) + \
                    pkgver.depends_list.get("Depends", []):
        if not any(dep.all_targets() for dep in or_group):
            # If none of the or-choices can be satisfied, add it to missing
            missing.append(or_group)

    if missing:
        print "Package:", pkgver.parent_pkg.name
        print "Version:", pkgver.ver_str
        print "Missing:",
        print ", ".join(" | ".join(fmt_dep(dep) for dep in or_group)
                        for or_group in missing)
        print


def main():
    """The main function."""
    apt_pkg.init_config()
    apt_pkg.init_system()

    cache = apt_pkg.Cache()
    print 'group_count:', cache.group_count
    print 'package_count:', cache.package_count
    print 'package_file_count:', cache.package_file_count
    print 'provides_count:', cache.provides_count
    print 'version_count:', cache.version_count
    print 'ver_file_count:', cache.ver_file_count
#    print("cache.packages: %r" % cache.packages)
#    for pkg in cache.packages:
#        print pkg.name
    pkg=cache['cos-upgrade']
    for version in pkg.version_list:
        print("arch:\t%s" % version.arch)
        print("depends_list_str:\t%s" % version.depends_list_str)
        pkgfile=version.file_list
        print("PackageFile.filename:\t%s" % pkgfile)
        print("id:\t%s" % version.id)
        print("installed_size:\t%s" % version.installed_size)
        print("size:\t%s" % version.size)
        print("ver_str:\t%s" % version.ver_str)
        print

    for pkg in sorted(cache.packages, key=lambda pkg: pkg.name):
        # pkg is from a list of packages, sorted by name.
        for version in pkg.version_list:
            # Check every version
            for pfile, _ in version.file_list:
                if (pfile.origin == "Debian" and pfile.component == "main" and
                    pfile.archive == "unstable"):
                    # We only want packages from Debian unstable main.
                    check_version(version)
                    break

if __name__ == "__main__":
    main()
