CC=clang++
CFLAGS=-Wall -g -std=c++1y
LIB=../../lib
INCLUDE=../../include
BIN=../../bin

test: all
	$(CC) $(CFLAGS) -D DEBUG -I$(INCLUDE) routing_tree_test.cpp -L$(LIB) -ldynrouting -o $(BIN)/test_routing

all: sources
	ar -rcs $(LIB)/libdynrouting.a *.o && \
		rm *.o

sources: format
	$(CC) $(CFLAGS) -c tree.cpp key.cpp _tree_iter.cpp _tree_node.cpp

format:
	clang-format -style=Google -i *.cpp *.h

.PHONY: clean
clean:
	rm *.o *.gch $(LIB)/libdynrouting.a
