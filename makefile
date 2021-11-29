CC = gcc
CFLAGS = -Wall

freeinfo : FreeInfo.c
	$(CC) $?  $(CFLAGS) -o freeinfo

install : systeminfo
	mv freeinfo ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
