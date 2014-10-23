#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <time.h>
#define STACK_SIZE 4096
#define ITERATIONS 10

// Slow fibonacci function
unsigned int fibonacci (unsigned int n) {
	if ( n == 0)
		return 0;
	else if ( n == 1)
		return 1;
	else
		return fibonacci(n-1) + fibonacci(n-2);
}

// Function run by the kernel thread
int process_fn(void* data) {
    unsigned int n = 30;
    //printf("Process Fibonacci n=%u", n);
    fibonacci(n);
    //printf("Process Fibonacci done");
    return 0;
}

void GetElapsedTime(unsigned long *times) {
	int i;
    for (i = 0; i < ITERATIONS; i++) {
	    // Data required for thread creation.
	    void * child_stack = malloc(STACK_SIZE);
		
        /*********************************************
         * Code to be benchmarked goes here
         *********************************************/
		clock_t start, end;
		start = clock();
		clone(&process_fn, child_stack+STACK_SIZE, 0, NULL);
		end = clock();

        times[i] = end - start;
    }
    
    return;
}

int main (void) {
	unsigned long *times;
	unsigned long average_time = 0;
	times = malloc(ITERATIONS*sizeof(unsigned long));
	GetElapsedTime(times);
    int i;
	for (i = 0; i < ITERATIONS; i++) {
        average_time += times[i];
        printf("The elapsed time is: %lu\n", times[i]);
    }
    average_time /= ITERATIONS;
    printf("The average elapsed time is: %lu\n", average_time);
	return 0;
}
