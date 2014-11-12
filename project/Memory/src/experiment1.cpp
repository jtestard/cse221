#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4

void experimentOne() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentOne.csv";
	unsigned long array_sizes[ARRAYS_COUNT];// Array size will be 2^(8+array_size);
	int* arrays[ARRAYS_COUNT * NUM_OF_STRIDES];
	unsigned int stride_sizes[NUM_OF_STRIDES];
	stride_sizes[0] = 1;
	stride_sizes[1] = 8;
	stride_sizes[2] = 64;
	stride_sizes[3] = 512;
	array_sizes[0] = 256 * KILOBYTE;
	for (unsigned int i = 1; i < ARRAYS_COUNT; i++) {
	    array_sizes[i] = array_sizes[i-1] << 1;
	}
	unsigned long iterations = 1000000; //1 Million iterations.
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment one...with %d total arrays\n", ARRAYS_COUNT * NUM_OF_STRIDES);
    for (unsigned int j = 0; j < NUM_OF_STRIDES; j++) {
        for (unsigned int i = 0; i < ARRAYS_COUNT; i++) {
            make_array(arrays[i + j * ARRAYS_COUNT],array_sizes[i], NULL);
        }
	}
	for(unsigned int j = 0; j < NUM_OF_STRIDES; j++) {
		for (unsigned int i = 0; i < ARRAYS_COUNT; i++) {
			read_array(
				arrays[i + j * ARRAYS_COUNT],
				array_sizes[i],
				iterations,
				stride_sizes[j],
				file
			);
		}
	}
	for (unsigned int i = 0; i < ARRAYS_COUNT; i++) {
		free(arrays[i]);
	}
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentOne();
	return 0;
}
