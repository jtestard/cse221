#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4
#define REPEAT_COUNT 5

void experimentTwo() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentTwo.csv";
	const unsigned long BILLION = 1000000000;
	unsigned char* mmapped_array1;
	unsigned char* mmapped_array2;
	unsigned int memory_size = 162 * KILOBYTE; // L3 cache is 3MB. Array is size is 9 times size of L3 cache.
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken, mb_per_sec;
	unsigned char* temp1;
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment two...\n");
	srand(time(NULL));
	mmapped_array1 = (unsigned char *) malloc(memory_size * sizeof(unsigned char));
	mmapped_array2 = (unsigned char *) malloc(memory_size * sizeof(unsigned char));
	temp1 = (unsigned char*) malloc(3 * KILOBYTE * sizeof(unsigned char)); //temp1 is exactly the size of L3 cache.
	
	for (unsigned int i=0; i < REPEAT_COUNT; i++) {
	    //Allocate memory for arrays using mmap
	    for (unsigned int j = 0; j < memory_size; j++)
	    	mmapped_array1[j] = (unsigned char) rand() % 255;
	    // Read measurement.
	    // use loop unrolling to copy integers from array1 to array2
	    // We assume arrays are contiguous chunks of memory
	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    memcpy(temp1, &mmapped_array1[0], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[3 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[6 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[9 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[12 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[15 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[18 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[21 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[24 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[27 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[30 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[33 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[36 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[39 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[42 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[45 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[48 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[51 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[54 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[57 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[60 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[63 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[66 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[69 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[72 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[75 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[78 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[81 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[84 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[87 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[90 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[93 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[96 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[99 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[102 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[105 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[108 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[111 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[114 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[117 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[120 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[123 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[126 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[129 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[132 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[135 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[138 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[141 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[144 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[147 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[150 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[153 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[156 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[159 * KILOBYTE], 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec = (162 * BILLION) / time_taken;
	    cout << "read bandwith: " << mb_per_sec  << "kb/secs;"<< endl;
	    fprintf(file,"%lu\n", mb_per_sec);

	    // Write measurement.
	    // use loop unrolling to copy integers from array1 to array2
	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    memcpy(&mmapped_array2[0 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[3 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[6 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[9 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[12 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[15 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[18 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[21 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[24 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[27 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[30 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[33 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[36 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[39 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[42 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[45 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[48 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[51 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[54 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[57 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[60 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[63 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[66 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[69 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[72 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[75 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[78 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[81 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[84 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[87 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[90 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[93 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[96 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[99 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[102 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[105 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[108 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[111 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[114 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[117 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[120 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[123 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[126 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[129 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[132 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[135 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[138 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[141 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[144 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[147 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[150 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[153 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[156 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[159 * KILOBYTE], temp1, 3 * KILOBYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec = (162 * BILLION) / time_taken;
	    cout << "write bandwith: " << mb_per_sec << "kb/secs;"<< endl;
	    fprintf(file,"%lu\n", mb_per_sec);

	}
	free(mmapped_array1);
	free(mmapped_array2);
	free(temp1);
	fclose(file);
}

int main(int argc, char **argv, char **envp) {
	experimentTwo();
	return 0;
}
