SRCPATH = ./src/
BINPATH = ./bin/
CC = g++
CFLAGS = -Wall -g  -o0
CXXFLAGS = $(CFLAGS)

all: $(SRCPATH)memory_profile.o $(SRCPATH)memory_utils.o
	$(CC) $(CXXFLAGS) $(SRCPATH)memory_utils.o $(SRCPATH)memory_profile.o -o $(BINPATH)memory_profile -lrt


$(SRCPATH)memory_profile.o: $(SRCPATH)memory_profile.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)memory_profile.cpp -o $(SRCPATH)memory_profile.o -lrt

$(SRCPATH)memory_utils.o: $(SRCPATH)memory_utils.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)memory_utils.cpp -o $(SRCPATH)memory_utils.o -lrt

clean:
	rm $(SRCPATH)memory_utils.o $(SRCPATH)memory_profile.o
