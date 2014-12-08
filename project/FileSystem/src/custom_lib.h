#ifndef _CUSTOM_LIB
#define _CUSTOM_LIB

#define _LARGEFILE64_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int count;
time_t start;

void done();
void handle(const char *string, int error);

unsigned long getblocksize();
unsigned long getMegabyte();
int getTimeout();
void setTimeout(int t);

#endif
