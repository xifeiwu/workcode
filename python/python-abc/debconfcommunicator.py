# -*- coding: utf-8; Mode: Python; indent-tabs-mode: nil; tab-width: 4 -*-

# Copyright (C) 2005, 2006, 2007, 2008 Canonical Ltd.
# Written by Colin Watson <cjwatson@ubuntu.com>.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

import fcntl
import subprocess

import debconf
import sys
sys.path.insert(0, '/usr/lib/ubiquity')
from ubiquity import misc


class DebconfCommunicator(debconf.Debconf):
    def __init__(self, owner, title=None, cloexec=False, env=None):
        self.dccomm = subprocess.Popen(
            ['debconf-communicate', '-fnoninteractive', owner],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            close_fds=True, env=env, preexec_fn=misc.regain_privileges,
            universal_newlines=True)
        debconf.Debconf.__init__(self, title=title,
                                 read=self.dccomm.stdout,
                                 write=self.dccomm.stdin)
        if cloexec:
            fcntl.fcntl(self.read.fileno(), fcntl.F_SETFD, fcntl.FD_CLOEXEC)
            fcntl.fcntl(self.write.fileno(), fcntl.F_SETFD, fcntl.FD_CLOEXEC)

    def shutdown(self):
        if self.dccomm is not None:
            self.dccomm.stdout.close()
            self.dccomm.stdin.close()
            self.dccomm.wait()
            self.dccomm = None

    def __del__(self):
        self.shutdown()

db = DebconfCommunicator('ubiquity', cloexec=True)
print("ubiquity/show_shutdown_button : %s" % db.get('ubiquity/show_shutdown_button'))
print("ubiquity/hide_slideshow : %s" % db.get('ubiquity/hide_slideshow'))
try:
    finished_restart_only = db.get('ubiquity/finished_restart_only')
except debconf.DebconfError:
    finished_restart_only = False
print("ubiquity/finished_restart_only : %s" % finished_restart_only)
print("localechooser/languagelist : %s" % db.get('localechooser/languagelist'))
print("localechooser/languagelist-choices : %s" % db.metaget('localechooser/languagelist', 'choices'))
#db.set('ubiquity/show_alpha_warning', 'True')
print("localechooser/languagelist-choices-c : %s" % db.metaget('localechooser/languagelist', 'choices-c'))
#db.fset('localechooser/languagelist', 'seen', 'true')
db.shutdown()

#command=['log-output', '-t', 'PACKAGE', '--pass-stdout', '/usr/lib/ubiquity/localechooser/localechooser']
#subp = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,)
#print("localechooser/localechooser : %s" % subp.stdout.read())
