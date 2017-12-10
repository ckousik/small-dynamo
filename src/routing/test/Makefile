CC=clang++
CFLAGS=-std=c++1y -Wall -g -D DEBUG
LIB=../../../lib
INC=-I../../../include -I../
BIN=../../../bin

all: format
	$(CC) $(CFLAGS) $(INC) *.cpp -L$(LIB) -ldynrouting -o $(BIN)/routing_test

format:
	clang-format -style=Google -i *.cpp
