#include "custom_lib.h"
unsigned long file_size;
char* filename = NULL;

void done()
{
	time_t end;
	FILE *file;

	time(&end);

	if (end < start + getTimeout()) {
		printf(".");
		alarm(1);
		return;
	}
	
	file = fopen(filename, "a");

	if (count) {
		double seeks_per_sec = count / (double) getTimeout();
		double access_time_per_block = getTimeout() / (double)count;
		double read_bandwidth = getblocksize() * seeks_per_sec / (double) getMegabyte();
	  printf(".\nResults: %.0f accesses/second, %.2f ms access time per block (%.2fMB/s)\n",
		 seeks_per_sec, access_time_per_block * 1000.0, read_bandwidth);
		fprintf(file, "%lu %.2f %.5f %.2f\n", 
				file_size * getblocksize() / getMegabyte(),
				seeks_per_sec,
				access_time_per_block * 1000.0,
				read_bandwidth);
	}
	fclose(file);
	exit(EXIT_SUCCESS);
}

// Does random seeking through the disk.
int main(int argc, char **argv)
{
	int fd, retval;
	unsigned long numblocks, BLCKS_PER_MB;
	off64_t offset;
	BLCKS_PER_MB = getMegabyte() / getblocksize();

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc != 5) {
		printf("Usage: random_access <raw disk device> <fake_file_size_in_mb> <measurement_filename>\n");
		exit(EXIT_SUCCESS);
	}
	// Open file with O_DIRECT, which will bypass file cache.
    fd = open(argv[1], O_RDONLY | O_DIRECT );
    /*fd = open(argv[1], O_RDONLY);*/
	handle("open", fd < 0);
	file_size = atoi(argv[2]) * BLCKS_PER_MB; // WARNING : will fail silently in case of misuse.
	filename = argv[3];
	setTimeout(atoi(argv[4])); // will fail silently

	// Get block size from device driver.
	// Removing guarantee.
//	retval = ioctl(fd, BLKGETSIZE, &numblocks);
//	handle("ioctl", retval == -1);
//	if (file_size > numblocks) 
//		file_size = numblocks;
	numblocks = file_size; 

	// Create buffer and handle alignment (required by O_DIRECT).
	char *buffer;
	unsigned long alignment = getblocksize();
	posix_memalign((void*) &buffer, alignment*2, getblocksize());

	printf("Random access : %s [%luMB] [%lu blocks], wait %d seconds\n",
	       argv[1], numblocks / BLCKS_PER_MB, numblocks, getTimeout());

	// Perform experiment
	time(&start);
	srand(start);
	signal(SIGALRM, &done);
	alarm(1);

	for (;;) {
		offset = (off64_t) numblocks * random() / RAND_MAX;
		retval = lseek64(fd, getblocksize() * offset, SEEK_SET);
		handle("lseek64", retval == (off64_t) -1);
		retval = read(fd, buffer, getblocksize());
		handle("read", retval < 0);
		count++;
	}
	/* notreached */
}
