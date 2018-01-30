all:  Ufraw2Uc rawToFoamPar rawToFoam  voxelImageConvert 
# warning type clean, dependencies are not handelled yet

myBinDir ?= ../bin

CXX ?= g++
#ZLIBDIR=../thirdparty/zlib-1.2.11/
#ZLIBCFLAGS=-DZLIB -I$(ZLIBDIR) -I$(ZLIBDIR)/contrib/iostream3/
#ZLIBLFLAGS=zfstream.o -L$(ZLIBDIR) -lz
#ZLIBSTREAM=zfstream.o

rawToFoam.o:	rawToFoam.cpp
	$(CXX)  -DGCC_COMPILER -O3 -Wall   -std=c++0x  -pedantic -c rawToFoam.cpp
rawToFoam:	rawToFoam.o $(ZLIBSTREAM)
	$(CXX)	-DGCC_COMPILER   -std=c++0x   $(ZLIBLFLAGS) -o	$(myBinDir)/rawToFoam rawToFoam.o 

rawToFoamPar.o:	rawToFoamPar.cpp
	$(CXX)  -DGCC_COMPILER  -O3 -Wall   -std=c++0x -pedantic $(ZLIBCFLAGS) -c rawToFoamPar.cpp
rawToFoamPar:	rawToFoamPar.o $(ZLIBSTREAM)
	$(CXX)	-DGCC_COMPILER -O3 -Wall  -std=c++0x $(ZLIBLFLAGS) -o	$(myBinDir)/rawToFoamPar rawToFoamPar.o 



zfstream.o:	$(ZLIBDIR)/contrib/iostream3/zfstream.cc
	$(CXX) -DGCC_COMPILER -O3 -Wall -std=c++0x -pedantic $(ZLIBCFLAGS) -c $(ZLIBDIR)/contrib/iostream3/zfstream.cc

voxelImageConvert.o:	voxelImageConvert.cpp
	$(CXX) -DGCC_COMPILER -O3 -Wall -std=c++0x -pedantic $(ZLIBCFLAGS) -c voxelImageConvert.cpp
voxelImageConvert:	voxelImageConvert.o $(ZLIBSTREAM)
	$(CXX) -DGCC_COMPILER -O3 -Wall  voxelImageConvert.o $(ZLIBLFLAGS) -o $(myBinDir)/voxelImageConvert 


Ufraw2Uc.o:	Ufraw2Uc.cpp
	$(CXX) -DGCC_COMPILER -O3 -Wall -std=c++0x -pedantic $(ZLIBCFLAGS) -c Ufraw2Uc.cpp
Ufraw2Uc:	Ufraw2Uc.o  $(ZLIBSTREAM)
	$(CXX) -DGCC_COMPILER -O3 -Wall  Ufraw2Uc.o  $(ZLIBLFLAGS) -o $(myBinDir)/Ufraw2Uc



clean:
	rm  -f *.o
