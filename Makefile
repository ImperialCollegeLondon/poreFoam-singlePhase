
# makefile for top directory

all:
	mkdir -p lib
	mkdir -p bin
	mkdir -p include
	(cd thirdparty && make -j$(nproc))
	(cd src && ./AllMake)


clean: 
	(cd thirdparty && make clean)
	(cd src && ./AllClean)

distclean: 
	(cd thirdparty && make clean)
	(cd src && ./AllDistClean)
	echo && echo "Warning: removing  bin, include, lib and share  folders" && echo press Ctrl+c to stop && echo && sleep 6
	rm -rf   src/../bin  src/../include  src/../lib  src/../share log.make
	(cd ./thirdparty/foam-miniext-3.4/ && ./AllDistclean)
