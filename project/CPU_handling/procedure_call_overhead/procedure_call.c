#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#define ITERATIONS 100
#define STACK_SIZE (1024 * 1024)

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);} while (0)
struct timespec ts_start,ts_end,test_of_time;
struct timespec diff(struct timespec start, struct timespec end);
unsigned long  *times;

void procedure_0 () {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_1 (char a1) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_2 (char a1,char a2) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_3 (char a1,char a2,char a3) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_4 (char a1,char a2,char a3,char a4) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_5 (char a1,char a2,char a3,char a4,char a5) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void procedure_6 (char a1,char a2,char a3,char a4,char a5,char a6) {
	clock_gettime(CLOCK_REALTIME,&ts_end);
	test_of_time = diff(ts_start,ts_end);
}

void procedure_7 (char a1,char a2,char a3,char a4,char a5,char a6,char a7) {
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	test_of_time = diff(ts_start,ts_end);
}

void inline GetElapsedTime(unsigned long iterations, int j) {
    int i;
	pid_t pid = getpid();
    for (i = 0; i < iterations; i++) {
		switch (j) {
			case 0:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_0();
				break;
			case 1:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_1('a');
				break;
			case 2:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_2('a','b');
				break;
			case 3:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_3('a','b','c');
				break;
			case 4:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_4('a','b','c','d');
				break;
			case 5:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_5('a','b','c','d','e');
				break;
			case 6:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_6('a','b','c','d','e','f');
				break;
			case 7:
		        clock_gettime(CLOCK_REALTIME,&ts_start);
		        procedure_7('a','b','c','d','e','f','g');
				break;
			default:
				break;
		}
    	times[i] = test_of_time.tv_nsec;
    }
    return;
}

int main( int argc, const char* argv[] ){

    int i,j;
	unsigned long average_time = 0;
	unsigned long iterations = ITERATIONS;
	if (argc > 1)
		iterations = (unsigned long) atoi(argv[1]);
	
    times = malloc(iterations*sizeof(unsigned long*));

    if (!times) {
        printf("Failed to allocate times memory\n");
        return 0;
    }

	//Launch the timing
	for (j = 0 ; j < 8 ; j++) {
        GetElapsedTime(iterations, j);
        for (i = 0; i < iterations; i++) {
            if (i != 0)
				average_time += times[i];
        }
        average_time /= iterations - 1;
        printf("%i %lu\n", j , average_time);
	}
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
