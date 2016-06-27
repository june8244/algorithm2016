all: cadb153

cadb153: block_def.o layer_def.o netMgr.o utility.o main.o
	g++ block_def.o layer_def.o netMgr.o utility.o main.o -o cadb153

utility.o: utility.cpp
	g++ -c utility.cpp

block_def.o: block_def.cpp
	g++ -c block_def.cpp

layer_def.o: layer_def.cpp
	g++ -c layer_def.cpp

netMgr.o: netMgr.cpp
	g++ -c netMgr.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *o cadb153
