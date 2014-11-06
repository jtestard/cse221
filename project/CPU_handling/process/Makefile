all: process

process: process.o
	gcc -o process process.o

process.o: process.c
	gcc -c process.c

clean:
	rm process process.o
