#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4

void experimentTwo() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentTwo.csv";
	//char mmap_file_name1[] = "/tmp/mmapped.bin";
	//char mmap_file_name2[] = "/tmp/mmapped2.bin";
	const unsigned long BILLION = 1000000000;
	int* mmapped_array1;
	int* mmapped_array2;
	unsigned int memory_size = 8 * MEGABYTE;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment two...\n");
	srand(time(NULL));
	//Allocate memory for arrays using mmap
	mmapped_array1 = (int*) malloc(memory_size * sizeof(int));
	mmapped_array2 = (int*) malloc(memory_size * sizeof(int));
	for (unsigned int j = 0; j < 8 * MEGABYTE; j++)
		mmapped_array1[j] = rand();
	// Use loop unrolling to copy integers from array1 to array2
	// Using memcpy instead of bcopy because it was deprecated.
	clock_gettime(CLOCK_REALTIME,&ts_start);
	memcpy(mmapped_array2, mmapped_array1, MEGABYTE);
	memcpy(mmapped_array2+ 1* MEGABYTE, mmapped_array1+ 1* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 2* MEGABYTE, mmapped_array1+ 2* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 3* MEGABYTE, mmapped_array1+ 3* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 4* MEGABYTE, mmapped_array1+ 4* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 5* MEGABYTE, mmapped_array1+ 5* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 6* MEGABYTE, mmapped_array1+ 6* MEGABYTE, MEGABYTE);
	memcpy(mmapped_array2+ 7* MEGABYTE, mmapped_array1+ 7* MEGABYTE, MEGABYTE);
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	
	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	cout << "Read+Write bandwith: " << (8 * BILLION) / time_taken << "MB/secs;"<< endl;
	fprintf(file,"%lu\n", time_taken);

	free(mmapped_array1);
	free(mmapped_array2);
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentTwo();
	return 0;
}
