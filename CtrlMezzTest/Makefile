#base install folder for sub-20 library 
SUB20BASE=/home/sckao/HCAL_Upgrade/tool/moduleCheckSUB20/SUB-20-snap-110319/
#base install folder for libusb library
LIBUSBBASE=/home/sckao/HCAL_Upgrade/tool/moduleCheckSUB20/libusb-1.0.9/

SUPPORTCODE = uHTRPowerMezzInterface.cpp uHTRPowerMezzInterface.h
ifneq ($(words $(wildcard make.local)),0)
include make.local
endif
# end of the configurable region
# Directories to search for header files
SEARCHDIRS :=  -I. -I$(LIBUSBBASE)/include/libusb-1.0/ -I$(SUB20BASE)/lib -I/usr/include/readline
# variables
LINKER       := g++
DEPENDFLAGS  := -O3 -Wall ${SEARCHDIRS} -g
TOUCHHEADERS := ${MYCODEDIR}/*.h

# C
CC     := gcc
CFLAGS  = ${DEPENDFLAGS}

# C++
CXX      := g++
CXXFLAGS  = ${DEPENDFLAGS}

%.o : %.cc
	${CXX} ${CPPFLAGS} -c $< -o $@ ${CXXFLAGS}

%.o : %.cpp
	${CXX} ${CPPFLAGS} -c $< -o $@ ${CXXFLAGS}

all:OUTPUT

OUTPUT:uHTR_PowerMezz_Test.exe uHTR_ClockMezz_Test.exe uHTR_CtrlMezz_Test.exe sub20tool.exe

LIBSRCS := uHTRMezzInterface.cpp uHTRPowerMezzInterface.cpp uHTRClockMezzInterface.cpp uHTRCtrlMezzInterface.cpp uHTRMezzanines.cpp

LIBOBJS :=$(patsubst %.cpp,%.o,${LIBSRCS})

uHTR_PowerMezz_Test.exe: uHTR_PowerMezz_Test.o ${LIBOBJS}
	gcc -O2 -DLIBUSB_1_0 -I. -L$(LIBUSBBASE)/lib/ -L$(SUB20BASE)/lib/ $^ -lboost_thread -lreadline -lsub -lusb-1.0 -lstdc++ -lm -o $@

uHTR_ClockMezz_Test.exe: uHTR_ClockMezz_Test.o ${LIBOBJS}
	gcc -O2 -DLIBUSB_1_0 -I. -L$(LIBUSBBASE)/lib/ -L$(SUB20BASE)/lib/ $^ -lreadline -lsub -lusb-1.0 -lstdc++ -lm -o $@

uHTR_CtrlMezz_Test.exe: uHTR_CtrlMezz_Test.cpp uHTRCtrlMezzInterface.o ${LIBOBJS}
	gcc -O2 -DLIBUSB_1_0 -I. -I$(LIBUSBBASE)/include/libusb-1.0/ -I$(SUB20BASE)/lib -I/usr/include/readline -L$(LIBUSBBASE)/lib/ -L$(SUB20BASE)/lib/ -L/usr/lib/x86_64-linux-gnu/  $^ -lsub -lusb-1.0 -lreadline -lstdc++ -lm -o $@

sub20tool.exe: sub20tool.o ${LIBOBJS}
	gcc -O2 -DLIBUSB_1_0 -I. -L$(LIBUSBBASE)/lib/ -L$(SUB20BASE)/lib/ $^ -lsub -lusb-1.0 -lreadline -lstdc++ -lm -o $@

uHTR_ClockMezz_Test.o : ${LIBSRCS}

uHTR_PowerMezz_Test.o : ${LIBSRCS} uHTRMezzInterface.h uHTRPowerMezzInterface.h

uHTRMezzanines.o : ${LIBSRCS} uHTRMezzanines.h

uHTRCtrlMezzInterface.o : uHTRCtrlMezzInterface.cpp uHTRCtrlMezzInterface.h

clean:	
	rm -f uHTR_PowerMezz_Test.exe uHTR_ClockMezz_Test.exe uHTR_CtrlMezz_Test.exe uHTRClockMezzInterface.o uHTR_ClockMezz_Test.o uHTRMezzInterface.o uHTRPowerMezzInterface.o uHTR_PowerMezz_Test.o uHTRMezzanines.o uHTR_CtrlMezz_Test.o sub20tool.o

