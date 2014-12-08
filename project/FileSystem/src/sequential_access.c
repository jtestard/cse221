#include "custom_lib.h"

unsigned long numblocks;
char *buffer;

void done_sequential()
{
	time_t end;
	FILE *file;
	char filename[] = "sequential_access.csv";

	time(&end);

	if (end < start + getTimeout()) {
		printf(".");
		alarm(1);
		return;
	}

	file = fopen(filename, "a");
	
	if (count) {
		double scans_per_sec = count / (double) getTimeout();
		double access_time_per_block = getTimeout() / (double)(count * numblocks);
		double read_bandwidth = numblocks * getblocksize() * scans_per_sec / (double) getMegabyte();
	  printf(".\nResults: %.0f scans/second, %.2f ms access time per block (%.2fMB/s)\n",
		 scans_per_sec, access_time_per_block * 1000.0, read_bandwidth);
		fprintf(file, "%lu %.2f %.5f %.2f\n", 
				numblocks * getblocksize() / getMegabyte(),
				scans_per_sec,
				access_time_per_block * 1000.0,
				read_bandwidth);
	}
	free(buffer);
	exit(EXIT_SUCCESS);
}


// Does random seeking through the disk.
int main(int argc, char **argv)
{
//	unsigned long getblocksize() = 4096;
	int fd, retval;
	unsigned long file_size, BLCKS_PER_MB, total_size;
	BLCKS_PER_MB = getMegabyte() / getblocksize();

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc != 3) {
		printf("Usage: random_seek <raw disk device> <fake_file_size_in_mb>\n");
		exit(EXIT_SUCCESS);
	}
	// Open file with O_DIRECT, which will bypass file cache.
    fd = open(argv[1], O_RDONLY | O_DIRECT );
	/*fd = open(argv[1], O_RDONLY);*/
	handle("open", fd < 0);
	file_size = atoi(argv[2]) * BLCKS_PER_MB; // WARNING : will fail silently in case of misuse.
	
	// Get block size from device driver.
	retval = ioctl(fd, BLKGETSIZE, &numblocks);
	handle("ioctl", retval == -1);
	// File size cannot grow bigger than of the disk partition (~30GB).
	if (file_size > numblocks) 
		file_size = numblocks;
	numblocks = file_size;
	total_size = numblocks * getblocksize();

	// Create buffer and handle alignment (required by O_DIRECT).
	// Alignment must be a power of 2. We pick the largest power of 2 smaller
	// expected size.
	unsigned long alignment = 1;
	while (alignment < (total_size >> 1))
		alignment <<= 1;
	total_size = alignment;
	// Alignment might be huge
	posix_memalign((void*)&buffer, alignment, alignment);
	// numblocks has to be upated accordingly.
	numblocks = total_size / getblocksize();

	printf("Sequential Access : %s [%luMB] [%lu blocks], wait %d seconds\n",
	       argv[1], numblocks / BLCKS_PER_MB, numblocks, getTimeout());

	// Perform experiment
	time(&start);
	signal(SIGALRM, &done_sequential);
	alarm(1);

	for (;;) {
	    //We read the entire file sequentially. Alignment correct?
		retval = read(fd, buffer, total_size);
		handle("read", retval < 0);
		count++;
	}
	/* notreached */
}
