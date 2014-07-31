#!/usr/bin/python
#from myclass import abc
#from myclass import outfunc

#print abc.i
#a = abc()
#tmp=a.f("True")
#print "value of tmp:", tmp
#tt = outfunc()i
def callfunc():
    print "in call function."
    printglobal()

def printglobal():
    global model_check
    global model_name
    print "model_check:", model_check
    print "model_name:", model_name

class callclassa():
    def call(self):
        print "call callfunc()"
        callfunc()

class callclassb():
    def call(self):
        print "in call class 2"
global model_check
global model_name
model_check = True
model_name = "cos-update"
#c = callfunction()
#c.call()

#callfunc()
