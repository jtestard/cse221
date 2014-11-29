ITERATIONS=100000
RADDR="127.0.0.1"
RPORT="2000"

all: rtt

rtt: rtt.o
	gcc -o rtt rtt.o

rtt.o: rtt.c
	gcc -O0 -c rtt.c -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS)

clean:
	rm rtt rtt.o
