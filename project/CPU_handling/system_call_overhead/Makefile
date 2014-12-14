all: sys_call

sys_call: sys_call.o
	gcc -o sys_call sys_call.o

sys_call.o: sys_call.c
	gcc -c sys_call.c

clean:
	rm sys_call sys_call.o
