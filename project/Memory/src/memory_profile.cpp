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
	char** mallocs;
	unsigned int write_size = 150;
	unsigned int read_sample_size = 17;
	unsigned int read_sizes[read_sample_size];
	read_sizes[0] = 1;
	for (unsigned int i = 1; i < read_sample_size; i++) {
		read_sizes[i] = read_sizes[i-1] << 1;
	}
	char filename[] = "experimentOne.csv";
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment one...\n");
	unsigned int written = write_megabyte(&mallocs, write_size, file);
	if (written!=write_size) {
		printf("Error! Less than %dMB written, aborting experiment", written);
		exit(1);
	}	
	for (unsigned int i=0; i < read_sample_size; i++) {
		read_kilobytes(&mallocs, read_sizes[i], file, false);
	}
	
	//Free the memory
	printf("Now freeing...\n");
	for (unsigned int i = 0; i < write_size; i++) {
		free(mallocs[i]);
	}
	free(mallocs);
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
	char filename[] = "experimentTree.csv";
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
	//Experiment 1
	//experimentOne();
	experimentThree();
	return 0;
}
