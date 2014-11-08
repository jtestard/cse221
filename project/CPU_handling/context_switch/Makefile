all: context_switch

context_switch: context_switch.o
	gcc -o context_switch context_switch.o

context_switch.o: context_switch.c
	gcc -O0 -c context_switch.c

clean:
	rm context_switch context_switch.o
