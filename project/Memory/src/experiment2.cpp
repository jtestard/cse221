#include "memory_utils.h"

#define REPEAT_COUNT 5
#define BILLION 1000000000LL

void exp_2() {
	using std::cout;
	using std::endl;
	FILE *file;
	char filename[] = "experimentTwo.csv";
	unsigned char* big_array1;
	unsigned char* big_array2;
	unsigned int memory_size = 162 * MEGABYTE; // L3 cache is 3MB. Array is size is 54 times size of L3 cache.
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken, mb_per_sec_r, mb_per_sec_w;
	unsigned char* smallArray;
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem trying to open file : %s... experiment aborted.\n", filename);
		exit(1);
	}
	printf("Starting experiment two...\n");
	srand(time(NULL));
	big_array1 = (unsigned char *) malloc(memory_size * sizeof(unsigned char));
	big_array2 = (unsigned char *) malloc(memory_size * sizeof(unsigned char));
	smallArray = (unsigned char*) malloc(3 * MEGABYTE * sizeof(unsigned char)); //small array is exactly the size of L3 cache.
	
	// Populate the first big array.
	for (unsigned int j = 0; j < memory_size; j++)
		big_array1[j] = (unsigned char) rand() % 255;
	
	for (unsigned int i=0; i < REPEAT_COUNT; i++) {
	    
		// We assume arrays are contiguous chunks of memory
	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    #define ONE_READ memcpy(smallArray, &big_array1[(rand()%54)*3*MEGABYTE], 3 * MEGABYTE);
		#define FIVE_R ONE_READ ONE_READ ONE_READ ONE_READ ONE_READ
		#define TWENTY_R FIVE_R FIVE_R FIVE_R FIVE_R
		#define HUNDRED_R TWENTY_R TWENTY_R TWENTY_R TWENTY_R TWENTY_R
		#define THOUSAND_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R HUNDRED_R
		THOUSAND_R
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec_r = (3000 * BILLION) / time_taken;
	    cout << "read bandwith: " << mb_per_sec_r  << "mb/secs; ";

	    clock_gettime(CLOCK_REALTIME,&ts_start);
	    #define ONE_WRITE memcpy(&big_array2[(rand()%54)*3*MEGABYTE], smallArray, 3 * MEGABYTE);
		#define FIVE_W ONE_WRITE ONE_WRITE ONE_WRITE ONE_WRITE ONE_WRITE
		#define TWENTY_W FIVE_W FIVE_W FIVE_W FIVE_W
		#define HUNDRED_W TWENTY_W TWENTY_W TWENTY_W TWENTY_W TWENTY_W
		#define THOUSAND_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W HUNDRED_W
		THOUSAND_W
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
		mb_per_sec_w = (3000 * BILLION) / time_taken;
	    cout << "write bandwith: " << mb_per_sec_w << "mb/secs;"<< endl;
	    fprintf(file,"%lu %lu\n", mb_per_sec_r, mb_per_sec_w);

	}
	free(big_array1);
	free(big_array2);
	free(smallArray);
	fclose(file);

}

int main(int argc, char **argv, char **envp) {
	exp_2();
	return 0;
}
