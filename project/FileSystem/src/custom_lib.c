#include "custom_lib.h"
unsigned long BLOCKSIZE = 4096;
unsigned long MEGABYTE = 1048576;
int TIMEOUT = 5;

void handle(const char *string, int error)
{
	if (error) {
		perror(string);
		exit(EXIT_FAILURE);
	}
}

unsigned long getblocksize() { return BLOCKSIZE; }
unsigned long getMegabyte() { return MEGABYTE; }
int getTimeout() { return TIMEOUT; }

void setTimeout(int t) { TIMEOUT = t;}
