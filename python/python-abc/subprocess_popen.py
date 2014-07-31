#!/usr/bin/python
import subprocess

pkgs='cos-upgrade cos-info'
popen = subprocess.Popen(['sudo', 'apt-get', 'install', pkgs], stdout = subprocess.PIPE)
out, error = popen.communicate()
print out, error
