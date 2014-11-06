all: timing

timing: timing.o
	gcc -o timing timing.o

timing.o: timing.c
	gcc -O0 -c timing.c

clean:
	rm timing timing.o
