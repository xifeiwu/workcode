import subprocess
subp=subprocess.Popen('python -u ./output_per_second.py',shell=True,stdout=subprocess.PIPE)
while subp.poll()==None:
    print subp.stdout.readline()
print subp.returncode
