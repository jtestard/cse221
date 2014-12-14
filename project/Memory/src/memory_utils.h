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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <limits.h>

const unsigned int MEGABYTE = 1048576;
const unsigned int KILOBYTE = 1024;
const unsigned int ALLOC_SIZE = MEGABYTE/1024;
const unsigned int REPEAT_COUNT = 1;
/*
Records difference in time between two timespec structures.
*/
struct timespec diff(struct timespec start, struct timespec end);

/* Used for back to back load
*/
typedef struct _node {
	char data[KILOBYTE];
	struct _node *next;
} node;


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
unsigned int write_megabyte(char ***mallocs_ptr, unsigned int numMegaBytes, FILE* file);


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
unsigned int read_kilobytes(char ***mallocs_ptr, unsigned int numKiloBytes, FILE* file, bool random);

/*
Reads all the memory in the mallocs array. If the array is bigger than 
memory size, then pagefaults will occur.
*/
void read_all_memory(char ***mallocs_ptr, unsigned int numMegaBytes, FILE* file);

/**
Creates list for experiment 1.
*/
void make_list(node **root_ptr, unsigned int size, FILE* file);

void free_list(node **root_ptr);

void read_list_wrap(node **root_ptr, unsigned int iter, FILE* file);

void free_mmapped_array(int* mmapped_array, unsigned int size);

void mmap_array(int* &mmapped_array, unsigned int size, char* filename);

void read_array(int * &array, unsigned long size, unsigned long iterations, unsigned int stride_size, FILE* file);

void make_array(int* &array, unsigned long size, FILE* file);

const char ONE_KB_STRING[1024] = "123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456712345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456788";
#endif
