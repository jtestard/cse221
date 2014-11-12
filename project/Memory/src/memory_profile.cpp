#include <iostream>
#include "memory_utils.h"

/*
In experiment 1, we allocate a relatively small amount of memory to read from (25MB).
We then successively attempt to read 1KB, 2KB, 4KB, ..., 16MB (2^10 to 2^24 bytes).
During that time, we do some profiling.
*/
void experimentOne() {
	using std::cout;
	using std::endl;
	FILE *file;
	unsigned int arrays_count = 8; 
	char filename[] = "experimentOne.csv";
	unsigned long array_sizes[arrays_count];// Array size will be 2^(8+array_size);
	int* arrays[arrays_count];
	unsigned int stride_sizes[4];
	stride_sizes[0] = 1;
	stride_sizes[1] = 8;
	stride_sizes[2] = 64;
	stride_sizes[3] = 512;
	array_sizes[0] = 256 * KILOBYTE;
	for (unsigned int i = 1; i < arrays_count; i++) {
	    array_sizes[i] = array_sizes[i-1] << 1;
	}
	unsigned long iterations = 1000000; //1 Million iterations.
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment one...\n");
	for (unsigned int i = 0; i < arrays_count; i++) {
		make_array(arrays[i],array_sizes[i],NULL);
	}
	for(unsigned int j = 0; j < 4; j++) {
		for (unsigned int i = 0; i < arrays_count; i++) {
			read_array(
				arrays[i],
				array_sizes[i],
				iterations,
				stride_sizes[j],
				file
			);
		}
	}
	for (unsigned int i = 0; i < arrays_count; i++) {
		free(arrays[i]);
	}
	fclose(file);
}

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

/*
Experiment three : use cgroups to limit available physical memory, forcing 
the process to page more.
*/
void experimentThree() {
	using std::cout;
	using std::endl;
	FILE *file;
	char** mallocs;
	unsigned int write_size = 150;
	char filename[] = "experimentThree.csv";
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment three...\n");
	unsigned int written = write_megabyte(&mallocs, write_size, file);
	if (written!=write_size) {
		printf("Error! Less than %dMB written, aborting experiment", written);
		exit(1);
	}	
	
	read_all_memory(&mallocs, write_size, file);
	
	//Free the memory
	printf("Now freeing...\n");
	for (unsigned int i = 0; i < write_size; i++) {
		free(mallocs[i]);
	}
	free(mallocs);
	fclose(file);
}


int main(int argc, char **argv, char **envp) {
	experimentOne();
	experimentTwo();
	//experimentThree();
	return 0;
}
