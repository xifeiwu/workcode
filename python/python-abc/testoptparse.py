#! /usr/bin/python3
import optparse
VERSION = '2.14.8-1linuxmint2'
import os

def main(oem_config):
    usage = '%prog [options] [frontend]'
    parser = optparse.OptionParser(usage=usage, version=VERSION)
    parser.set_defaults(
        debug=('UBIQUITY_DEBUG' in os.environ),
        debug_pdb=False,
        cdebconf=False,
        automatic=False,
        query=False)
    parser.add_option('-d', '--debug', dest='debug', action='store_true',
                      help='debug mode (warning: passwords will be logged!)')
    parser.add_option('--pdb', dest='debug_pdb', action='store_true',
                      help='drop into Python debugger on a crash')
    parser.add_option('--cdebconf', dest='cdebconf', action='store_true',
                      help='use cdebconf instead of debconf (experimental)')
    parser.add_option('--automatic', dest='automatic', action='store_true',
                      help='do not ignore the "seen" flag (useful for '
                           'unattended installations).')
    parser.add_option('--only', dest='only', action='store_true',
                      help='tell the application that it is the only desktop '
                           'program running so that it can customize its UI '
                           'to better suit a minimal environment.')
    parser.add_option('-q', '--query', dest='query', action='store_true',
                      help='find out which frontend will be used by default')
    parser.add_option('-g', '--greeter', dest='greeter', action='store_true',
                      help='allow the user to leave the installer and enter '
                           'a live desktop (for the initial boot).')
    parser.add_option('-b', '--no-bootloader', dest='bootloader',
                      default=True, action='store_false',
                      help='Do not install a bootloader.')
    parser.add_option('--ldtp', dest='ldtp',
                      action='store_true',
                      help='Name widgets in ATK by their GtkBuilder names, '
                           'to support LDTP testing.')
    parser.add_option('--no-webcam', dest='webcam',
                      default=True, action='store_false',
                      help='Disable the webcam page.')

    (options, args) = parser.parse_args()
    print("options : %s - args : %s" % (options, args))
    if args:
        print("args in main.")
    else:
        print("no args in main.")
def find_target():
    paths=[]
    with open('/proc/mounts') as mounts:
        for line in mounts:
            path=line.split(' ')[1]
            paths.append(path)
    print paths

if __name__ == "__main__":
    oem_config = False
    main(oem_config)
    find_target()
