CC = gcc
CFLAGS = -Wall

freeinfo : InfoGetters.o FreeInfo.c
	$(CC) $?  $(CFLAGS) -o $@

InfoGetters.o : InfoGetters.c
	$(CC) $? $(CFLAGS) -c -o $@

install : systeminfo
	mv freeinfo ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
