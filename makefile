CC = gcc
CFLAGS = -Wall

systeminfo : SystemInfo.c
	$(CC) $?  $(CFLAGS) -o systeminfo

install : systeminfo
	mv systeminfo ~/.logingreeting

clean : 
	rm -rf *.o systeminfo
