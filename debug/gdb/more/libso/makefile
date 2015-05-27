#makefile
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFLAGS=-g
LDLIBS=-ltest #changed
AR=ar

SRCS=main.cc functions.cc testobj.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: main
main: $(OBJS) libtest.so
	$(CXX) $(LDFLAGS) -o main $(OBJS) -L. $(LDLIBS) 
    
main.o: main.cc functions.h testobj.h

functions.o: functions.h functions.cc

libtest.so:
	$(CXX) -g -c -fPIC -o testobj.o testobj.cc
	$(CXX) -g -shared -o libtest.so testobj.o
	
clean:
	$(RM) $(OBJS)

all-clean: clean
	$(RM) libtest.so main
