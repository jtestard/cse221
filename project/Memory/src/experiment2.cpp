#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4

void experimentTwo() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentTwo.csv";
	char mmap_file_name1[] = "/tmp/mmapped.bin";
	char mmap_file_name2[] = "/tmp/mmapped2.bin";
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
	//Allocate memory for arrays using mmap
	mmap_array(&mmapped_array1, memory_size, mmap_file_name1);
	mmap_array(&mmapped_array2, memory_size, mmap_file_name2);
	for (int j = 0; j < 8; j++)
		mmapped_array1[j] = j;
	// Use loop unrolling to copy integers from array1 to array2
	clock_gettime(CLOCK_REALTIME,&ts_start);	
	mmapped_array2[0] = mmapped_array1[0];
	mmapped_array2[1] = mmapped_array1[1];
	mmapped_array2[2] = mmapped_array1[2];
	mmapped_array2[3] = mmapped_array1[3];
	mmapped_array2[4] = mmapped_array1[4];
	mmapped_array2[5] = mmapped_array1[5];
	mmapped_array2[6] = mmapped_array1[6];
	mmapped_array2[7] = mmapped_array1[7];
	clock_gettime(CLOCK_REALTIME,&ts_end);	
	
	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	cout << "Time taken:" << time_taken << " nanoseconds;"<< endl;
	fprintf(file,"%lu\n", time_taken);
	
	free_mmapped_array(&mmapped_array1, memory_size);
	free_mmapped_array(&mmapped_array2, memory_size);
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentTwo();
	return 0;
}
