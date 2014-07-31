#! /usr/bin/python3
from __future__ import print_function
import os
import fnmatch
import sys
import debconf
import subprocess
# Split a string on commas, stripping surrounding whitespace, and
# honouring backslash-quoting.
def split_choices(text):
    textlen = len(text)
    index = 0
    items = []
    item = ''

    while index < textlen:
        if text[index] == '\\' and index + 1 < textlen:
            if text[index + 1] == ',' or text[index + 1] == ' ':
                item += text[index + 1]
                index += 1
        elif text[index] == ',':
            items.append(item.strip())
            item = ''
        else:
            item += text[index]
        index += 1

    if item != '':
        items.append(item.strip())
    return items

#print("args %d\nargv0 : %s, argv1 : %s" % (len(sys.argv), sys.argv[0], split_choices(sys.argv[1])))

command=['log-output', '-t', 'PACKAGE', '--pass-stdout', '/usr/lib/ubiquity/localechooser/localechooser']
ret = subprocess.call(command)
print("localechooser/localechooser : %r" % ret)
#subp = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE,)


#PLUGIN_PATH = (os.environ.get('UBIQUITY_PLUGIN_PATH', False)
#               or '/usr/lib/ubiquity/plugins')
#modfiles = [x for x in os.listdir(PLUGIN_PATH) if fnmatch.fnmatch(x, '*.py')]
#print("modfiles : %s" % modfiles)
#for modfile in modfiles:
#    modname = os.path.splitext(modfile)
#    print("%s-%s" % modname)

#TARGET='/target'
#paths=[]
#with open('/proc/mounts') as mounts:
#    for line in mounts:
#        print("path:%s" % line)
#        path = line.split(' ')[1]
#        if path == TARGET or path.startswith(TARGET + '/'):
#            paths.append(path)
#paths.sort()
#paths.reverse()
#print("%s" % paths)

#VERSION = '2.14.8-1linuxmint2, append some contents.'
#with open('myfile', 'w') as version_file:
#    print('ubiquity %s' % VERSION, file=version_file)

