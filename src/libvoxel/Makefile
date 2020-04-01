all:  voxelImageConvert Ufraw2Uc voxelToFoamPar voxelToFoam 
# warning type clean, dependencies are not handelled yet

msSrc ?= $(abspath ..)
USE_ZLIB=1
USE_TIFF=1
include  $(msSrc)/script/Makefile.in



#msCFLAGS := $(msCFLAGS)  -std=c++11 -DGCC_COMPILER -O3 -Wall -pedantic 


voxelToFoam.o:	voxelToFoam.cpp
	${msCXX}  $(msCFLAGS) -O3 -Wall   -std=c++11  -pedantic -c voxelToFoam.cpp
voxelToFoam:	voxelToFoam.o
	${msCXX}	-DGCC_COMPILER   -std=c++11  voxelToFoam.o  $(msLFLAGS) -o	$(msBinDir)/voxelToFoam$(OPT)

Ufraw2Uc.o:	Ufraw2Uc.cpp
	${msCXX} $(msCFLAGS) -DGCC_COMPILER -O3 -c Ufraw2Uc.cpp
Ufraw2Uc:	Ufraw2Uc.o
	${msCXX} -DGCC_COMPILER  Ufraw2Uc.o  $(msLFLAGS) -o $(msBinDir)/Ufraw2Uc$(OPT)

voxelToFoamPar.o:	voxelToFoamPar.cpp
	${msCXX}  $(msCFLAGS) -DGCC_COMPILER  -c voxelToFoamPar.cpp
voxelToFoamPar:	voxelToFoamPar.o
	${msCXX}	-DGCC_COMPILER voxelToFoamPar.o $(msLFLAGS) -o	$(msBinDir)/voxelToFoamPar$(OPT)



voxelImageConvert.o:	voxelImageConvert.cpp
	${msCXX}  $(msCFLAGS)   -c voxelImageConvert.cpp
voxelImageConvert:	voxelImageConvert.o 
	${msCXX} -DGCC_COMPILER  voxelImageConvert.o $(msLFLAGS) -o $(msBinDir)/voxelImageConvert$(OPT)





clean:
	rm  -f *.o
