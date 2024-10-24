CC = clang
CPP = clang++
CFLAGS = -Wall -g
EXE = freeinfo
HEADERS = FreeInfo.h InfoGetters.hpp InfoStrParser.hpp argparse/include/argparse/argparse.hpp

$(EXE) : InfoGetters.o FreeInfo.o
	$(CPP) $(CFLAGS) $^ -o $@

FreeInfo.o : FreeInfo.cpp InfoGetters.o $(HEADERS) 
	$(CPP) $(CFLAGS) $< -c -o $@

InfoGetters.o : InfoGetters.c FreeInfo.h
	$(CC) $(CFLAGS) $< -c -o $@

install : $(EXE)
	mv $< ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
