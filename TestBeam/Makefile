CC=g++ -g -fPIC -Wall -O -ansi -D_GNU_SOURCE -g -O2 -m64
ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags`
LIB=`${ROOTSYS}/bin/root-config --libs`
GLIB=`${ROOTSYS}/bin/root-config --glibs`

OBJECTS=txt2ntuple.o 

all:txt2ntp.exe

txt2ntuple.o : txt2ntuple.cc txt2ntuple.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIB)

txt2ntp.exe : main.cc $(OBJECTS)
	$(CC) -o $@ $< $(OBJECTS) $(ROOTFLAG) $(LIB) $(GLIB)

clean : 
	rm -rf *.o txt2ntp.exe
