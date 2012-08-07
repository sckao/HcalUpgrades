CC=g++ -g -fPIC -Wall -O -ansi -D_GNU_SOURCE -g -O2 -m64
ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags`
LIB=`${ROOTSYS}/bin/root-config --libs`
GLIB=`${ROOTSYS}/bin/root-config --glibs`

OBJECTS=AnaInput.o hDraw.o Ntuple.o HcalAna.o 

all:test.exe

AnaInput.o : AnaInput.cc AnaInput.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIB)

hDraw.o : hDraw.cc hDraw.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIB)

Ntuple.o : Ntuple.cc Ntuple.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIB)

HcalAna.o : HcalAna.cc HcalAna.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIB)

test.exe : main.cc $(OBJECTS)
	$(CC) -o $@ $< $(OBJECTS) $(ROOTFLAG) $(LIB) $(GLIB)

clean : 
	rm -rf *.o test.exe
