CC = clang
CPP = clang++
CFLAGS = -Wall

freeinfo : InfoGetters.o FreeInfo.cpp
	$(CPP) $?  $(CFLAGS) -o $@

InfoGetters.o : InfoGetters.c
	$(CPP) $? $(CFLAGS) -c -o $@

install : systeminfo
	mv freeinfo ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
