CC=g++
CFLAGS=-std=c++11 -c
 
all: tree

tree: main.o BTree.o
	$(CC) main.o BTree.o -o tree

main.o:	main.cpp
	$(CC) $(CFLAGS) main.cpp

BTree.o:
	$(CC) $(CFLAGS) BTree.cpp 
run:
	./tree

clean:
	rm -rf *.o tree
