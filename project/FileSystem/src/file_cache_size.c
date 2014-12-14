#include "custom_lib.h"

unsigned long numblocks;
char *buffer;
char *filename = NULL;

void done_sequential()
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
		double MB_per_sec = count / (double) getTimeout();
		double access_time_per_block = getTimeout() / ((double)(count * getMegabyte()) / getblocksize());
        printf(".\n[*] Results: %.0f MB/second, %.2f ms access time per block\n",
                                                             MB_per_sec, access_time_per_block * 1000.0);
		fprintf(file, "%lu %.2f %.5f\n", 
				numblocks * getblocksize() / getMegabyte(),
				MB_per_sec,
				access_time_per_block * 1000.0);
	}
	free(buffer);
	exit(EXIT_SUCCESS);
}


// Does random seeking through the disk.
int main(int argc, char **argv)
{
//	unsigned long getblocksize() = 4096;
	int fd, retval;
	unsigned long BLCKS_PER_MB, total_size, total_read;
	BLCKS_PER_MB = getMegabyte() / getblocksize();

	setvbuf(stdout, NULL, _IONBF, 0);

	if (argc != 5) {
		printf("Usage: file_cache_size  <raw disk device> <fake_file_size_in_mb>\n");
		exit(EXIT_SUCCESS);
	}

    fd = open(argv[1], O_RDONLY);
	handle("open", fd < 0);
	numblocks = atoi(argv[2]) * BLCKS_PER_MB; // WARNING : will fail silently in case of misuse.
	
	filename = argv[3];
	setTimeout(atoi(argv[4])); // will fail silently

	total_size = numblocks * getblocksize();

    buffer = (char *) malloc(getMegabyte());

	printf("File Cache Size: %s [%luMB] [%lu blocks], wait %d seconds",
	       argv[1], numblocks / BLCKS_PER_MB, numblocks, getTimeout());

	// Perform experiment
	time(&start);
	signal(SIGALRM, &done_sequential);
	alarm(1);

	for (;;) {
	    //We read the entire file one MB at a time. Alignment correct?
		retval = read(fd, buffer, getMegabyte());
		handle("read", retval < 0);
        total_read += retval;
        if (total_read >= total_size) {
            retval = lseek64(fd, 0, SEEK_SET);
            handle("lseek64", retval == (off64_t) -1);
            total_read = 0;
            continue;
        } 
		count++;
	}
	/* notreached */
}
