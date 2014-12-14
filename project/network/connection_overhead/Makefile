ITERATIONS=1000
RADDR="192.168.1.74"
RPORT="2000"

all: setup_overhead teardown_overhead

setup_overhead: setup_overhead.o
	gcc -o setup_overhead setup_overhead.o

setup_overhead.o: setup_overhead.c
	gcc -O0 -c setup_overhead.c -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS)

teardown_overhead: teardown_overhead.o
	gcc -o teardown_overhead teardown_overhead.o

teardown_overhead.o: teardown_overhead.c
	gcc -O0 -c teardown_overhead.c -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS)

clean:
	rm setup_overhead setup_overhead.o teardown_overhead teardown_overhead.o
