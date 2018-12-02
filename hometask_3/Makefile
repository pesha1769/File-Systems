CC=g++
CFLAGS=-std=c++11 -c
 
all: FAT16

FAT16: main.o Application.o FileSystemFAT16.o
	$(CC) main.o Application.o FileSystemFAT16.o -o FAT16

main.o:	main.cpp
	$(CC) $(CFLAGS) main.cpp

Application.o:
	$(CC) $(CFLAGS) Application.cpp
FileSystemFAT16.o:
	$(CC) $(CFLAGS) FileSystemFAT16.cpp 
run:
	./FAT16

clean:
	rm -rf *.o FAT16
