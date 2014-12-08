SRCPATH = ./src/
BINPATH = ./bin/
CC = gcc
CFLAGS = -Wall -g

all: $(SRCPATH)custom_lib.o $(BINPATH)random_access $(BINPATH)sequential_access $(BINPATH)fill_mem $(BINPATH)file_cache_size

$(BINPATH)file_cache_size: $(SRCPATH)file_cache_size.o
	$(CC) $(CFLAGS) $(SRCPATH)custom_lib.o $(SRCPATH)file_cache_size.o -o $(BINPATH)file_cache_size

$(SRCPATH)fill_mem.o: $(SRCPATH)file_cache_size.c
	$(CC) $(CFLAGS) -c $(SRCPATH)file_cache_size.c -o $(SRCPATH)file_cache_size.o
	
$(BINPATH)fill_mem: $(SRCPATH)fill_mem.o
	$(CC) $(CFLAGS) $(SRCPATH)fill_mem.o -o $(BINPATH)fill_mem

$(SRCPATH)fill_mem.o: $(SRCPATH)fill_mem.c
	$(CC) $(CFLAGS) -c $(SRCPATH)fill_mem.c -o $(SRCPATH)fill_mem.o

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
