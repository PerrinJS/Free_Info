CC = gcc
CFLAGS = -Wall

systemInfo : system\ info\ with\ settings\ 2.0.c
	$(CC) SystemInfo.c $(CFLAGS) -o systemInfo

install : systemInfo
	mv systemInfo ~/.logingreeting

clean : 
	rm -rf *.o systemInfo
