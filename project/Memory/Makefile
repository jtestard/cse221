SRCPATH = ./src/
BINPATH = ./bin/
CC = g++
CFLAGS = -Wall -g
CXXFLAGS = $(CFLAGS)

all: $(SRCPATH)memory_profile.o $(SRCPATH)memory_utils.o
	$(CC) $(CXXFLAGS) $(SRCPATH)memory_utils.o $(SRCPATH)memory_profile.o -o $(BINPATH)memory_profile


$(SRCPATH)memory_profile.o: $(SRCPATH)memory_profile.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)memory_profile.cpp -o $(SRCPATH)memory_profile.o

$(SRCPATH)memory_utils.o: $(SRCPATH)memory_utils.cpp $(SRCPATH)memory_utils.h
	$(CC) $(CXXFLAGS) -c $(SRCPATH)memory_utils.cpp -o $(SRCPATH)memory_utils.o

clean:
	rm $(SRCPATH)memory_utils.o $(SRCPATH)memory_profile.o
