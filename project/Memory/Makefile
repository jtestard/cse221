SRCPATH = ./src/
BINPATH = ./bin/
CC = g++
GCC = gcc
CFLAGS = -Wall -g  -o0
CXXFLAGS = $(CFLAGS)

all: $(BINPATH)experiment1 $(BINPATH)experiment2 $(BINPATH)experiment3

$(BINPATH)experiment1: $(SRCPATH)experiment1.o
	$(GCC) $(CFLAGS) $(SRCPATH)experiment1.o -o $(BINPATH)experiment1 -lrt

$(BINPATH)experiment2: $(SRCPATH)experiment2.o $(SRCPATH)memory_utils.o
	$(CC) $(CXXFLAGS) $(SRCPATH)memory_utils.o $(SRCPATH)experiment2.o -o $(BINPATH)experiment2 -lrt

$(BINPATH)experiment3: $(SRCPATH)experiment3.o $(SRCPATH)memory_utils.o
	$(CC) $(CXXFLAGS) $(SRCPATH)memory_utils.o $(SRCPATH)experiment3.o -o $(BINPATH)experiment3 -lrt

$(SRCPATH)experiment1.o: $(SRCPATH)experiment1.c
	$(GCC) $(CFLAGS) -c $(SRCPATH)experiment1.c -o $(SRCPATH)experiment1.o -lrt

$(SRCPATH)experiment2.o: $(SRCPATH)experiment2.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)experiment2.cpp -o $(SRCPATH)experiment2.o -lrt

$(SRCPATH)experiment3.o: $(SRCPATH)experiment3.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)experiment3.cpp -o $(SRCPATH)experiment3.o -lrt

$(SRCPATH)memory_utils.o: $(SRCPATH)memory_utils.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)memory_utils.cpp -o $(SRCPATH)memory_utils.o -lrt

clean:
	rm $(SRCPATH)memory_utils.o $(SRCPATH)experiment1.o $(SRCPATH)experiment2.o $(SRCPATH)experiment3.o
