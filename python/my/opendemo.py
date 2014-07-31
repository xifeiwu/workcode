#! /usr/bin/python3
import subprocess

def distribution():
    """Returns the name of the running distribution."""
    proc = subprocess.Popen(
        ['lsb_release', '-is'], stdout=subprocess.PIPE,
        universal_newlines=True)
    print("proc : %s" % (proc))
    return proc.communicate()[0].strip()
com = distribution()
print(com)
