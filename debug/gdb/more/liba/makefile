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
main: $(OBJS) libtest.a
	$(CXX) $(LDFLAGS) -o main $(OBJS) -L. $(LDLIBS) 
    
main.o: main.cc functions.h testobj.h

functions.o: functions.h functions.cc

testobj.o: testobj.cc testobj.h
libtest.a: testobj.o
	ar rv libtest.a testobj.o
	
clean:
	$(RM) $(OBJS)

all-clean: clean
	$(RM) libtest.a main
