#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4

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
	unsigned int written = write_megabyte(&mallocs, write_size, NULL);
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
	experimentThree();
	return 0;
}
