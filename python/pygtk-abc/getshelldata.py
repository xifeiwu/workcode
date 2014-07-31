import subprocess
import time
import tempfile

#fn = tempfile.NamedTemporaryFile()
#print "file name:", fn.name
#popen = subprocess.Popen(['sudo', 'apt-get', 'install', 'cos-info'], stdout = fn)
#cnt=0
#while popen.poll()==None:
#    fn.seek(0)
#    fn.readlines()
#    time.sleep(1)
#print 'lines:', fn.readlines()

#tmpfile="/tmp/shelloutput"
#fpwrite = open(tmpfile, 'w')
#popen = subprocess.Popen(['sudo', 'apt-get', 'install', 'cos-info'], stdout = fpwrite)
#cnt=0
#while popen.poll()==None:
#    cnt = cnt+1
#    print cnt
#    fpread = open(tmpfile, 'r')
#    lines = fpread.readlines()
#    for line in fpread.readlines():
#        print line
#    fpread.close()

#tmpfile="/tmp/shelloutput"
#fpwrite = open(tmpfile, 'a')
#popen = subprocess.Popen(['sudo', 'apt-get', 'install', 'cos-info'], stdout = fpwrite)
#cnt=0
#while popen.poll()==None:
#    subprocess.call(['cat', tmpfile])
#    time.sleep(0.05)
#subprocess.call(['cat', tmpfile])

popen = subprocess.Popen(['sudo', 'apt-get', 'install', 'cos-upgrade'], stdout = subprocess.PIPE)
#while popen.poll() == None:
#    out = popen.communicate()[0]
#    print out
#    time.sleep(0.1)
out = popen.communicate()[0]
print out

#with subprocess.Popen(["ifconfig"], stdout=subprocess.PIPE) as proc:
#    print (proc.stdout.read())

#popen = subprocess.Popen(['sudo', 'apt-get', 'install', 'cos-upgrade'], stdout = subprocess.PIPE)
#while popen.poll() == None:
#    print popen.stdout.readline()
#    time.sleep(1)
#print popen.stdout.read()
#print 'return code:', popen.returncode
