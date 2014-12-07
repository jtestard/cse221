#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#define BLOCKSIZE 512
#define TIMEOUT 30

int count;
time_t start;

void done()
{
	time_t end;

	time(&end);

	if (end < start + TIMEOUT) {
		printf(".");
		alarm(1);
		return;
	}

	if (count) {
	  printf(".\nResults: %d seeks/second, %.2f ms random access time\n",
		 count / TIMEOUT, 1000.0 * TIMEOUT / count);
	}
	exit(EXIT_SUCCESS);
}

void handle(const char *string, int error)
{
	if (error) {
		perror(string);
		exit(EXIT_FAILURE);
	}
}

// Does random seeking through the disk.
int main(int argc, char **argv)
{
	char buffer[BLOCKSIZE];
	int fd, retval;
	unsigned long numblocks;
	off64_t offset;

	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Seeker v2.0, 2007-01-15, "
	       "http://www.linuxinsight.com/how_fast_is_your_disk.html\n");

	if (argc != 2) {
		printf("Usage: seeker <raw disk device>\n");
		exit(EXIT_SUCCESS);
	}

	fd = open(argv[1], O_RDONLY);
	handle("open", fd < 0);

	retval = ioctl(fd, BLKGETSIZE, &numblocks);
	handle("ioctl", retval == -1);
	printf("Benchmarking %s [%luMB] [%lu blocks], wait %d seconds",
	       argv[1], numblocks / 2048, numblocks, TIMEOUT);

	time(&start);
	srand(start);
	signal(SIGALRM, &done);
	alarm(1);

	for (;;) {
		offset = (off64_t) numblocks * random() / RAND_MAX;
		retval = lseek64(fd, BLOCKSIZE * offset, SEEK_SET);
		handle("lseek64", retval == (off64_t) -1);
		retval = read(fd, buffer, BLOCKSIZE);
		handle("read", retval < 0);
		count++;
	}
	/* notreached */
}

