#!/usr/bin/python
import globalParameter as g
def callfunc():
    print "CONTEXT_ID:", g.CONTEXT_ID
    print "in call function."
    #printglobal()

def printglobal():
    global model_check
    global model_name
    print "model_check:", model_check
    print "model_name:", model_name


#global model_check
#global model_name
#model_check = True
#model_name = "cos-update"
#callfunc()
