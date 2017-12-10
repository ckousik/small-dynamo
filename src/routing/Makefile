CC=clang++
CFLAGS=-Wall -g -std=c++1y -D DEBUG
LIB=../../lib
INCLUDE=../../include
BIN=../../bin

test: all
	make -C test

all: sources
	ar -rcs $(LIB)/libdynrouting.a *.o && \
		rm *.o

sources: format
	$(CC) $(CFLAGS) -I$(INCLUDE) -c _tree.cpp key.cpp _tree_iter.cpp _tree_node.cpp \
		tree_table.cpp

format:
	clang-format -style=Google -i *.cpp *.h

.PHONY: clean
clean:
	rm *.o *.gch $(LIB)/libdynrouting.a
