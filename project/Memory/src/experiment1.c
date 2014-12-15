#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MEGABYTE 1048576
#define KILOBYTE 1024
#define ALLOC_SIZE MEGABYTE/1024
#define REPEAT_COUNT 1
#define ARRAYS_COUNT 13
#define NUM_OF_STRIDES 4

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

void memory_latency(unsigned long size, unsigned long iterations, unsigned long stride_size, FILE* file) {
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken, ul, k;
    register long unsigned it = iterations/100;
	char** p = NULL;
	char** array = (char **) malloc(sizeof(char*) * size);
	for (k = 0; k < size; k++) {
	    // Randomize to avoid cache prefetch effects
		array[k] = (char*) &array[(k/stride_size)*stride_size+rand()%stride_size)%size];
//		array[k] = (char*) &array[(k+rand()%size)%size];
	}
	p = array;
	long unsigned count = size / (stride_size * 100) + 1;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);
	for (ul=0; ul < it; ul++) {
		for (k=0; k < count; k++) {
		    #define ONE p = (char**)*p;
		    #define TEN ONE ONE ONE ONE ONE ONE ONE ONE ONE ONE
		    #define HUNDRED TEN TEN TEN TEN TEN TEN TEN TEN TEN TEN
		    HUNDRED;
		}
	}
	clock_gettime(CLOCK_REALTIME,&ts_end);
	free(array);

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec / (iterations*count); //The time taken is the time spent on a single read.
	
	//Collect data
	unsigned long size_kB = (size * sizeof(char*)) / KILOBYTE;
	if (file != NULL) {
	    fprintf(file,"%lu %lu %lu\n", size_kB, stride_size, time_taken);
	    printf("%lukB %lu %luns %lu %lu\n", size_kB, stride_size, time_taken, iterations, count);
	}
}

void experimentOne() {
	FILE *file;
	char filename[] = "experimentOne.csv";
	unsigned long array_sizes[ARRAYS_COUNT];// Array size will be 2^(8+array_size);
	unsigned long stride_sizes[NUM_OF_STRIDES];
	unsigned long i,j;
	stride_sizes[0] = 16; // Even with the smallest stride size, we explore the entire array for every array.
	stride_sizes[1] = 64;
	stride_sizes[2] = 256;
	stride_sizes[3] = 1024;
	array_sizes[0] = KILOBYTE;
	for (i = 1; i < ARRAYS_COUNT; i++) {
	    array_sizes[i] = array_sizes[i-1] << 1;
	}
	unsigned long iterations = 1000000; // 2 Million iterations.
	file = fopen(filename,"w+");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment one...with %d total arrays\n", ARRAYS_COUNT * NUM_OF_STRIDES);
	for(j = 0; j < NUM_OF_STRIDES; j++) {
		for (i = 0; i < ARRAYS_COUNT; i++) {
			memory_latency(
				array_sizes[i],
				iterations,
				stride_sizes[j],
				file
			);
		}
	}
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentOne();
	return 0;
}
