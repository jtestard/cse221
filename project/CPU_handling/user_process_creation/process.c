#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define STACK_SIZE (1024 * 1024)
#define ITERATIONS 1

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);} while (0)
struct timespec diff(struct timespec start, struct timespec end);

void inline GetElapsedTime(unsigned long  *times) {
    int i;
	struct timespec ts_start,ts_end,test_of_time;

    for (i = 0; i < ITERATIONS; i++) {
		pid_t pid;

		clock_gettime(CLOCK_REALTIME,&ts_start);	

		if ((pid = fork()) == 0) {
			clock_gettime(CLOCK_REALTIME,&ts_end);
			test_of_time = diff(ts_start,ts_end);
    		printf("%lu\n", test_of_time.tv_nsec);
			exit(0);
		}

		clock_gettime(CLOCK_REALTIME,&ts_end);	
        
		test_of_time = diff(ts_start,ts_end);

        times[i] = test_of_time.tv_nsec;
    }
    
    return;
}

int main( int argc, const char* argv[] ){

    int i;
    unsigned long *times;
	unsigned long average_time = 0;

    times = malloc(ITERATIONS*sizeof(unsigned long*));

    if (!times) {
        printf("Failed to allocate times memory\n");
        return 0;
    }

	//Launch the timing
    GetElapsedTime(times);

    for (i = 0; i < ITERATIONS; i++) {
        average_time += times[i];
    }

    average_time /= ITERATIONS;
    return 0;
}

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
