#!/usr/bin/env python
from globalParameter import *
print model_strlevel
class value:
    def __init__(self, main):
        self.value = "value in value."
        self.main = main
    def callmain(self):
        self.main.printv()
class mainclass:
    def __init__(self):
        self.v = value(self)
        self.v.callmain()
    def printv(self):
        print self.v.value
main = mainclass()
