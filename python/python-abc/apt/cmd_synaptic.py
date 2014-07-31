#!/usr/bin/python
try:
    import os
    import commands
    import sys
    import string
    import gtk
    import gtk.glade
    import gobject
    import tempfile
    import threading
    import time
    import gettext
    import fnmatch
    import urllib2
    from user import home
    sys.path.append('/usr/lib/linuxmint/common')
    from configobj import ConfigObj
except Exception, detail:
    print detail
    pass

try:
    import pygtk
    pygtk.require("2.0")
except Exception, detail:
    print detail
    pass
from subprocess import Popen, PIPE

def main():
    gladefile = "/usr/lib/linuxmint/mintUpdate/mintUpdate.glade"
    wTree = gtk.glade.XML(gladefile, "window1")
    print wTree
    wTree.get_widget("window1").set_title("Update Manager")
    cmd = ["sudo", "/usr/sbin/synaptic", "--hide-main-window",  \
            "--non-interactive", "--parent-window-id", "%s" % wTree.get_widget("window1")]
    cmd.append("-o")
    cmd.append("Synaptic::closeZvt=true")
    cmd.append("--progress-str")
    cmd.append("\"" + ("Please wait, this can take some time") + "\"")
    cmd.append("--finish-str")
    cmd.append("\"" + ("Update is complete") + "\"")
    f = tempfile.NamedTemporaryFile()

    pkg="cos-upgrade"
    f.write("%s\tinstall\n" % pkg)
    cmd.append("--set-selections-file")
    cmd.append("%s" % f.name)
    f.flush()
    print("cmd:\t%s" % cmd)
    comnd = Popen(' '.join(cmd), shell=True)
    returnCode = comnd.wait()
    gtk.main()

if __name__ == "__main__":
    main()
