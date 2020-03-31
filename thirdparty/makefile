
all: foam  zlib  tiff 

psInstall ?= $(abspath ..)
 $(info psInstall set to $(psInstall))

ifndef PS3rdParty
 # set PS3rdParty to this file directory
 PS3rdParty := $(abspath $(lastword $(MAKEFILE_LIST))/..)
 $(info PS3rdParty set to $(PS3rdParty))
endif

#hypre:
#	ls hypre-2.11.2/src/hypre/lib/libHYPRE.a || (cd hypre-2.11.2/src && ./configure --without-MPI   --without-fei  CC=$(preCC)gcc  F77=$(preCC)gfortran CPP=$(preCC)g++  CXX=$(preCC)g++  AR=$(preCC)ar\ -rcu   RANLIB=$(preCC)ranlib --host=$(psHost) ) || echo 
#	ls hypre-2.11.2/src/hypre/lib/libHYPRE.a || (cd hypre-2.11.2/src && make -j)

#hypreDbg:
#	(cp -r hypre-2.11.2 hypre-2.11.2Dbg)
#	(cd hypre-2.11.2Dbg/src && ./configure --enable-debug --without-MPI   --without-fei  CC=gcc-6  F77=gfortran CPP=g++-6  CXX=g++  AR=ar\ -rcu   RANLIB=ranlib ) 
#	(cd hypre-2.11.2Dbg/src && make)

#hypreWin:	 
#	(cp -r hypre-2.11.2 hypre-2.11.2MinGW)
#	(cd hypre-2.11.2MinGW/src && ./configure --without-MPI   --without-fei  CC=x86_64-w64-mingw32-gcc  F77=x86_64-w64-mingw32-gfortran CPP=x86_64-w64-mingw32-g++  CXX=x86_64-w64-mingw32-g++  AR=x86_64-w64-mingw32-ar\ -rcu   RANLIB=x86_64-w64-mingw32-ranlib --host x86_64-w64-mingw32 )
#	(cd hypre-2.11.2MinGW/src && make) 

hypre:
	ls $(psInstall)/lib/libHYPRE.a || (cd hypre-2.11.2/src/ && mkdir -p build)
	ls $(psInstall)/lib/libHYPRE.a || (cd hypre-2.11.2/src/build && cmake -DCMAKE_TOOLCHAIN_FILE=$(PS3rdParty)/toolchain$(OPT).txt -DCMAKE_INSTALL_PREFIX=$(psInstall)/ -DBUILD_SHARED_LIBS=False -DHYPRE_SEQUENTIAL=true ..)
	ls $(psInstall)/lib/libHYPRE.a || (cd hypre-2.11.2/src/build && make -j install)


zlib:
	ls $(psInstall)/lib/libzxx.a || (cd zlib-1.2.11 && mkdir -p build)
	ls $(psInstall)/lib/libzxx.a || (cd zlib-1.2.11/build && cmake -DCMAKE_TOOLCHAIN_FILE=$(PS3rdParty)/toolchain$(OPT).txt -DCMAKE_INSTALL_PREFIX=$(psInstall)/ -DBUILD_SHARED_LIBS=False ..)
	ls $(psInstall)/lib/libzxx.a || (cd zlib-1.2.11/build && make -j install)



tiff:
	ls $(psInstall)/lib/libtiffxx.a || (cd libtiff && mkdir -p build)
	ls $(psInstall)/lib/libtiffxx.a || (cd libtiff/build && cmake -DCMAKE_TOOLCHAIN_FILE=$(PS3rdParty)/toolchain$(OPT).txt -DCMAKE_INSTALL_PREFIX=$(psInstall)/ -DBUILD_SHARED_LIBS=False --disable-jbig ..)
	ls $(psInstall)/lib/libtiffxx.a || (cd libtiff/build && make -j install)
#	cp libtiff/build/libtiff/libtiff*.so* $(psDir)/bin/

foam:
	ls $(psInstall)/lib/libfiniteVolume*.so || (cd foamx3m && ./AllMake)
	ls $(psInstall)/lib/libfiniteVolume*.so || (cd foamx3m && ./AllInstall)



clean: 
	(rm -rf libtiff/build)
	(rm -rf zlib-1.2.11/build)
	(rm -rf hypre-2.11.2/src/build)
	(cd foamx3m && ./AllDistclean_keeplnInclude)

