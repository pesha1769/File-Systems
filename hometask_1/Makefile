CC=g++
CFLAGS=-std=c++11 -Wno-multichar -c 
 
all: conversion

conversion: main.o converter.o
	$(CC) main.o converter.o -o conversion

main.o:	main.cpp
	$(CC) $(CFLAGS) main.cpp

converter.o:
	$(CC) $(CFLAGS) converter.cpp 

clean:
	rm -rf *.o conversion

	







