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
	unsigned int memory_size = 162 * MEGABYTE; // L3 cache is 3MB. Array is size is 9 times size of L3 cache.
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
	temp1 = (unsigned char*) malloc(3 * MEGABYTE * sizeof(unsigned char)); //temp1 is exactly the size of L3 cache.
	
	for (unsigned int i=0; i < REPEAT_COUNT; i++) {
	    //Allocate memory for arrays using mmap
	    for (unsigned int j = 0; j < memory_size; j++)
	    	mmapped_array1[j] = (unsigned char) rand() % 255;
	    // Read measurement.
	    // use loop unrolling to copy integers from array1 to array2
	    // We assume arrays are contiguous chunks of memory
	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    memcpy(temp1, &mmapped_array1[0], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[3 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[6 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[9 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[12 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[15 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[18 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[21 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[24 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[27 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[30 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[33 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[36 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[39 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[42 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[45 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[48 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[51 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[54 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[57 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[60 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[63 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[66 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[69 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[72 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[75 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[78 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[81 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[84 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[87 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[90 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[93 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[96 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[99 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[102 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[105 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[108 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[111 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[114 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[117 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[120 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[123 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[126 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[129 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[132 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[135 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[138 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[141 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[144 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[147 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[150 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[153 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[156 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(temp1, &mmapped_array1[159 * MEGABYTE], 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec = (162 * BILLION) / time_taken;
	    cout << "read bandwith: " << mb_per_sec  << "mb/secs;"<< endl;
	    fprintf(file,"%lu\n", mb_per_sec);

	    // Write measurement.
	    // use loop unrolling to copy integers from array1 to array2
	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    memcpy(&mmapped_array2[0 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[3 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[6 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[9 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[12 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[15 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[18 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[21 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[24 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[27 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[30 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[33 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[36 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[39 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[42 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[45 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[48 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[51 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[54 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[57 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[60 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[63 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[66 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[69 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[72 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[75 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[78 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[81 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[84 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[87 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[90 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[93 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[96 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[99 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[102 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[105 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[108 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[111 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[114 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[117 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[120 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[123 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[126 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[129 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[132 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[135 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[138 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[141 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[144 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[147 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[150 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[153 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[156 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    memcpy(&mmapped_array2[159 * MEGABYTE], temp1, 3 * MEGABYTE); // cache miss. Prefetching will fetch first 3 MB in L3 cache.
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec = (162 * BILLION) / time_taken;
	    cout << "write bandwith: " << mb_per_sec << "mb/secs;"<< endl;
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
