#makefile
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFLAGS=-g
LDLIBS=
AR=ar

SRCS=main.cc functions.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: main
main: $(OBJS)
	$(CXX) $(LDFLAGS) -o main $(OBJS) -L. $(LDLIBS) 
    
main.o: main.cc functions.h

functions.o: functions.h functions.cc

clean:
	$(RM) $(OBJS)

all-clean: clean
	$(RM) main
