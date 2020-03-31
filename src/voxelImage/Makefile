all:  voxelImageConvert Ufraw2Uc voxelToFoamPar voxelToFoam 
# warning type clean, dependencies are not handelled yet


USE_ZLIB=1
USE_TIFF=1
psDir ?= $(abspath ..)
include  $(psDir)/Makefile.in



#psCFLAGS := $(psCFLAGS)  -std=c++11 -DGCC_COMPILER -O3 -Wall -pedantic 


voxelToFoam.o:	voxelToFoam.cpp
	$(psCXX)  $(psCFLAGS) -O3 -Wall   -std=c++11  -pedantic -c voxelToFoam.cpp
voxelToFoam:	voxelToFoam.o
	$(psCXX)	-DGCC_COMPILER   -std=c++11  voxelToFoam.o  $(psLFLAGS) -o	$(psBinDir)/voxelToFoam$(OPT)

Ufraw2Uc.o:	Ufraw2Uc.cpp
	$(psCXX) $(psCFLAGS) -DGCC_COMPILER -O3 -c Ufraw2Uc.cpp
Ufraw2Uc:	Ufraw2Uc.o
	$(psCXX) -DGCC_COMPILER  Ufraw2Uc.o  $(psLFLAGS) -o $(psBinDir)/Ufraw2Uc$(OPT)

voxelToFoamPar.o:	voxelToFoamPar.cpp
	$(psCXX)  $(psCFLAGS) -DGCC_COMPILER  -c voxelToFoamPar.cpp
voxelToFoamPar:	voxelToFoamPar.o
	$(psCXX)	-DGCC_COMPILER voxelToFoamPar.o $(psLFLAGS) -o	$(psBinDir)/voxelToFoamPar$(OPT)



voxelImageConvert.o:	voxelImageConvert.cpp
	$(psCXX)  $(psCFLAGS)   -c voxelImageConvert.cpp
voxelImageConvert:	voxelImageConvert.o 
	$(psCXX) -DGCC_COMPILER  voxelImageConvert.o $(psLFLAGS) -o $(psBinDir)/voxelImageConvert$(OPT)





clean:
	rm  -f *.o
