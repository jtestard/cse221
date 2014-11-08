all: looptiming

looptiming: looptiming.o
	gcc -o looptiming looptiming.o

looptiming.o: looptiming.c
	gcc -O0 -c looptiming.c

clean:
	rm looptiming looptiming.o
