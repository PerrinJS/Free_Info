CC = clang
CPP = clang++
CFLAGS = -Wall -g

freeinfo : InfoGetters.o FreeInfo.o
	$(CPP) $(CFLAGS) $^ -o $@

FreeInfo.o : FreeInfo.cpp InfoGetters.o
	$(CPP) $(CFLAGS) $< -c -o $@

InfoGetters.o : InfoGetters.c
	$(CC) $(CFLAGS) $< -c -o $@

install : systeminfo
	mv freeinfo ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
