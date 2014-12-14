all: procedure_call

procedure_call: procedure_call.o
	gcc -o procedure_call procedure_call.o

procedure_call.o: procedure_call.c
	gcc -c procedure_call.c

clean:
	rm procedure_call procedure_call.o
