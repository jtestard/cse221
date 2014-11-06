#ifndef MEMORY_UTILS
#define MEMORY_UTILS

#include <cstdlib>
#include <ios>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unistd.h>
/*
Records difference in time between two timespec structures.
*/
struct timespec diff(struct timespec start, struct timespec end);


/*
Records memory usage use for the profiling.
@param vm_usage:
	how much virtual memory is being used by the process.
@param resident_set:
	how much physical memory is being used by the process.
@param page_size_kb:
	the page size used by the architecture.
*/
void process_mem_usage(double& vm_usage, double& resident_set, long& page_size_kb);

/*
Writes megabytes of memory as strings. Also records outputs the memory usage and latency during that process.
Writing is done per chunks of one megabyte.

@param numMegabytes:
	The number of megabytes to allocate.
@param mallocs:
	The array containing the allocated memory.
@param file:
	The file to which profiling information is to be written.
@returns
	The number of megabytes actually allocated.
*/
unsigned int write_megabyte(char **mallocs, unsigned int numMegaBytes, FILE* file);


/*
Reads megabytes of memory as strings. Also records outputs the memory usage and latency during that process.
Reading is done per chunks of one kilobyte.

@param numKilobytes:
	The number of kilobytes to read.
@param mallocs:
	The string array from which to read.
@param file:
	The file to which profiling information is to be written.
@param random:
	If true, location of the memory being read is random, making caching for the OS harder.
	If false, location of the memory being read is sequential. *NOT IMPLEMENTED*
@returns
	The number of megabytes actually read.
*/
unsigned int read_kilobytes(char **mallocs, unsigned int numKiloBytes, FILE* file, bool random);

const unsigned int MEGABYTE = 1048576;
const unsigned int KILOBYTE = 1024;
const unsigned int ALLOC_SIZE = MEGABYTE/1024;
const char ONE_KB_STRING[1024] = "123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456712345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456788";
#endif
