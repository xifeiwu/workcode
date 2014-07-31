#!/usr/bin/python
class abc:
    i=12345
    def f(self, p):
        if(p == "True"):
            print "value of i:", self.i
        return self.i
    @staticmethod
    def gfunc(v):
        print "static method:\t", v
        print "static method: %s", f(True)
def outfunc():
    a = abc()
    a.f('True')
    abc.gfunc('test')
    print "in outfunc."
    return a
outfunc()
