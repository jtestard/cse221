#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 13
#define NUM_OF_STRIDES 4

void make_array(int* &array, unsigned long size, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	srand(time(NULL));
	
	clock_gettime(CLOCK_REALTIME,&ts_start);

	array = (int *) malloc(sizeof(int) * size);
	for (unsigned long ul=0; ul < size; ul++) {
		array[ul] = rand(); //Fill the array with some random values.
	}
	clock_gettime(CLOCK_REALTIME,&ts_end);

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	
	unsigned long size_kB = (size * sizeof(int)) / KILOBYTE;
	//Collect data
	if (file != NULL) {
	    process_mem_usage(vm, rss, page_size);
	    cout << "List size: " << size_kB << "kB; VM: " << vm << "kB; RAM: " << rss << "kB;";
	    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	    fprintf(file,"%lu %li %li %lu\n", size,(long)vm,(long)rss, time_taken);
	}
}

void read_array(int* &array, unsigned long size, unsigned long iterations, unsigned int stride_size, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
    int temp = 0;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);

	unsigned long stride = 0;
	for (unsigned int k=0 ; k < REPEAT_COUNT; k++) { //Make multiple experiments.
	    for (unsigned long ul=0; ul < iterations; ul++) {
            temp = array[stride];
	    	stride = (stride + stride_size) % (size + temp - temp); //dummy use of temp
	    }
	}
	clock_gettime(CLOCK_REALTIME,&ts_end);

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec / (iterations*REPEAT_COUNT); //The time taken is the time spent on a single read.
	
	//Collect data
	unsigned long size_kB = (size * sizeof(int)) / KILOBYTE;
	if (file != NULL) {
	    process_mem_usage(vm, rss, page_size);
	    cout << "List size: " << size_kB << "kB; Stride size: "<< stride_size << "; VM: " << vm << "kB; RAM: " << rss << "kB;";
	    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	    fprintf(file,"%lu %u %li %li %lu\n", size_kB, stride_size,(long)vm,(long)rss, time_taken);
	}
}

void experimentOne() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentOne.csv";
	unsigned long array_sizes[ARRAYS_COUNT];// Array size will be 2^(8+array_size);
//	int* arrays[ARRAYS_COUNT * NUM_OF_STRIDES];
	unsigned int stride_sizes[NUM_OF_STRIDES];
	stride_sizes[0] = 16; // Even with the smallest stride size, we explore the entire array for every array.
	stride_sizes[1] = 64;
	stride_sizes[2] = 256;
	stride_sizes[3] = 1024;
	array_sizes[0] = 4 * KILOBYTE;
	for (unsigned int i = 1; i < ARRAYS_COUNT; i++) {
	    array_sizes[i] = array_sizes[i-1] << 1;
	}
	unsigned long iterations = 2 * MEGABYTE; //1 Million iterations.
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment one...with %d total arrays\n", ARRAYS_COUNT * NUM_OF_STRIDES);
//    for (unsigned int j = 0; j < NUM_OF_STRIDES; j++) {
//        for (unsigned int i = 0; i < ARRAYS_COUNT; i++) {
//            make_array(arrays[i + j * ARRAYS_COUNT],array_sizes[i], NULL);
//        }
//	}
	for(unsigned int j = 0; j < NUM_OF_STRIDES; j++) {
		for (unsigned int i = 0; i < ARRAYS_COUNT; i++) {
			int* array = NULL;
			make_array(array, array_sizes[i], NULL);
			read_array(
				array,
				array_sizes[i],
				iterations,
				stride_sizes[j],
				file
			);
			free(array);
		}
	}
//	for (unsigned int i = 0; i < ARRAYS_COUNT * NUM_OF_STRIDES; i++) {
//		free(arrays[i]);
//	}
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentOne();
	return 0;
}
