SRCPATH = ./src/
BINPATH = ./bin/
CC = gcc
CFLAGS = -Wall -g

all: $(SRCPATH)custom_lib.o $(BINPATH)random_access $(BINPATH)sequential_access

$(BINPATH)sequential_access: $(SRCPATH)sequential_access.o
	$(CC) $(CFLAGS) $(SRCPATH)custom_lib.o $(SRCPATH)sequential_access.o -o $(BINPATH)sequential_access

$(SRCPATH)sequential_access.o: $(SRCPATH)sequential_access.c
	$(CC) $(CFLAGS) -c $(SRCPATH)sequential_access.c -o $(SRCPATH)sequential_access.o

$(BINPATH)random_access: $(SRCPATH)random_access.o
	$(CC) $(CFLAGS) $(SRCPATH)custom_lib.o $(SRCPATH)random_access.o -o $(BINPATH)random_access

$(SRCPATH)random_access.o: $(SRCPATH)random_access.c
	$(CC) $(CFLAGS) -c $(SRCPATH)random_access.c -o $(SRCPATH)random_access.o

$(SRCPATH)custom_lib.o: $(SRCPATH)custom_lib.c $(SRCPATH)custom_lib.h
	$(CC) $(CFLAGS) -c $(SRCPATH)custom_lib.c -o $(SRCPATH)custom_lib.o

clean:
	rm $(SRCPATH)random_access.o $(SRCPATH)sequential_access.o $(SRCPATH)custom_lib.o
