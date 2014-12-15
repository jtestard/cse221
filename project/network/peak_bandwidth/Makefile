ITERATIONS=100
#RADDR="127.0.0.1"
RADDR="192.168.1.74"
RPORT="2200"
BUFSIZE=1048576
MSGSIZE = $(shell echo 5 \* $(BUFSIZE) | bc)
INPUTBUFFERSIZE= $(BUFSIZE)
VERBOSE=0

all: peak_bandwidth_reader peak_bandwidth_writer  

peak_bandwidth_reader: peak_bandwidth_reader.o
	gcc -o peak_bandwidth_reader peak_bandwidth_reader.o

peak_bandwidth_reader.o: peak_bandwidth_reader.c
	gcc -O0 -c peak_bandwidth_reader.c -D VERBOSE=$(VERBOSE) -D BUFSIZE=$(BUFSIZE) -D INPUTBUFFERSIZE=$(INPUTBUFFERSIZE) -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS) -D MSGSIZE=$(MSGSIZE)

peak_bandwidth_writer: peak_bandwidth_writer.o
	gcc -o peak_bandwidth_writer peak_bandwidth_writer.o

peak_bandwidth_writer.o: peak_bandwidth_writer.c
	gcc -O0 -c peak_bandwidth_writer.c -D BUFSIZE=$(BUFSIZE) -D RADDR=\"$(RADDR)\" -D RPORT=$(RPORT) -D ITERATIONS=$(ITERATIONS) -D MSGSIZE=$(MSGSIZE)

clean:
	rm peak_bandwidth_reader peak_bandwidth_reader.o peak_bandwidth_writer peak_bandwidth_writer.o
