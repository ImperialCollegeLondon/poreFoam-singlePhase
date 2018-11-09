all:  voxelImageConvert Ufraw2Uc rawToFoamPar rawToFoam 
# warning type clean, dependencies are not handelled yet


USE_ZLIB=1
USE_TIFF=1
psDir ?= $(abspath ..)
include  $(psDir)/Makefile.in



#psCFLAGS := $(psCFLAGS)  -std=c++0x -DGCC_COMPILER -O3 -Wall -pedantic 


rawToFoam.o:	rawToFoam.cpp
	$(psCXX)  $(psCFLAGS) -O3 -Wall   -std=c++0x  -pedantic -c rawToFoam.cpp
rawToFoam:	rawToFoam.o
	$(psCXX)	-DGCC_COMPILER   -std=c++0x  rawToFoam.o  $(psLFLAGS) -o	$(psBinDir)/rawToFoam$(OPT)

Ufraw2Uc.o:	Ufraw2Uc.cpp
	$(psCXX) $(psCFLAGS) -DGCC_COMPILER -O3 -c Ufraw2Uc.cpp
Ufraw2Uc:	Ufraw2Uc.o
	$(psCXX) -DGCC_COMPILER  Ufraw2Uc.o  $(psLFLAGS) -o $(psBinDir)/Ufraw2Uc$(OPT)

rawToFoamPar.o:	rawToFoamPar.cpp
	$(psCXX)  $(psCFLAGS) -DGCC_COMPILER  -c rawToFoamPar.cpp
rawToFoamPar:	rawToFoamPar.o
	$(psCXX)	-DGCC_COMPILER rawToFoamPar.o $(psLFLAGS) -o	$(psBinDir)/rawToFoamPar$(OPT)



voxelImageConvert.o:	voxelImageConvert.cpp
	$(psCXX)  $(psCFLAGS)   -c voxelImageConvert.cpp
voxelImageConvert:	voxelImageConvert.o 
	$(psCXX) -DGCC_COMPILER  voxelImageConvert.o $(psLFLAGS) -o $(psBinDir)/voxelImageConvert$(OPT)





clean:
	rm  -f *.o
