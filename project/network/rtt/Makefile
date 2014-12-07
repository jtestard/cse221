ITERATIONS=10000
RADDR="192.168.1.74"
RPORT="2000"

# Default Ping Packet data size is 56
# We also want our data size to match in order to 
# perform a comparison of RTT between TCP and Ping (ICMP).
DATA_BYTES=56

all: rtt

rtt: rtt.o
	gcc -o rtt rtt.o

rtt.o: rtt.c
	gcc -O0 -c rtt.c -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS) -D DATA_BYTES=$(DATA_BYTES)

clean:
	rm rtt rtt.o
