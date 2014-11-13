all: context_switch_kthread

context_switch_kthread: context_switch_kthread.o
	gcc -o context_switch_kthread context_switch_kthread.o

context_switch_kthread.o: context_switch_kthread.c
	gcc -O0 -c context_switch_kthread.c

clean:
	rm context_switch_kthread context_switch_kthread.o
