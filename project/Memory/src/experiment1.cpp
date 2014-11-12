#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 15
#define NUM_OF_STRIDES 4

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
