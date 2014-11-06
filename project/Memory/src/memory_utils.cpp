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
struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

/*
Records memory usage use for the profiling.
@param vm_usage:
	how much virtual memory is being used by the process.
@param resident_set:
	how much physical memory is being used by the process.
@param page_size_kb:
	the page size used by the architecture.
*/
void process_mem_usage(double& vm_usage, double& resident_set, long& page_size_kb) {
	using std::ios_base;
	using std::ifstream;
	using std::string;

	vm_usage = 0.0;
	resident_set = 0.0;

	// 'file' stat seems to give the most reliable results
	//
	ifstream stat_stream("/proc/self/stat", ios_base::in);

	// dummy vars for leading entries in stat that we don't care about
	//
	string pid, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string utime, stime, cutime, cstime, priority, nice;
	string O, itrealvalue, starttime;

	// the two fields we want
	//
	unsigned long vsize;
	long rss;

	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
			>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime
			>> stime >> cutime >> cstime >> priority >> nice >> O >> itrealvalue
			>> starttime >> vsize >> rss; // don't care about the rest

	stat_stream.close();

	page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
	vm_usage = vsize / 1024.0;
	resident_set = rss * page_size_kb;
}

const unsigned int MEGABYTE = 1048576;
const unsigned int KILOBYTE = 1024;
const unsigned int ALLOC_SIZE = MEGABYTE/1024;
const char ONE_KB_STRING[1024] = "123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456712345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456788";

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
unsigned int write_megabyte(char **mallocs, unsigned int numMegaBytes, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;

	mallocs = (char**) malloc(numMegaBytes * sizeof(char*));
	int i,j,reached = 0;
	for (i = 0; i < numMegaBytes; i++) {
		void* p = malloc(MEGABYTE * sizeof(char));
		if (!p) {
			printf(
					"No more memory available! Memory allocated up to now : %i megabytes.\n",
					i);
			reached = i;
			break;
		}
		mallocs[i] = (char*) p;

		//Now fill in the malloc with some data, which forces the OS
		//to have this data in memory.
		clock_gettime(CLOCK_REALTIME,&ts_start);	
		
		strcpy(mallocs[i],ONE_KB_STRING);
		for (j=0; j<ALLOC_SIZE-1; j++)
			strcat(mallocs[i],ONE_KB_STRING);
		
		clock_gettime(CLOCK_REALTIME,&ts_end);	
		test_of_time = diff(ts_start,ts_end);
		time_taken = test_of_time.tv_nsec;
		
		//Collect data
		process_mem_usage(vm, rss, page_size);
		cout << "Allocated : " << i << "MB; VM: " << vm << "kB; RAM: " << rss << "kB;";
		cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
		fprintf(file,"%i %li %li %lu\n",i,(long)vm,(long)rss, time_taken);
	}
	if (!reached) {
		printf("Memory successfully allocated!");
		reached = numMegaBytes;
	}
	return reached;
}

/*
Reads megabytes of memory as strings. Also records outputs the memory usage and latency during that process.
Reading is done per chunks of one megabyte.

@param numMegabytes:
	The number of megabytes to read.
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
unsigned int read_megabytes(char **mallocs, char* retrieve, unsigned int numMegaBytes, FILE* file, bool random) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	
	void* p = malloc(MEGABYTE * sizeof(char));
	if (!p) {
		printf("Not enough memory available!");
		return 0;
	}
	retrieve = (char*) p;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);	
	
	for (int i = 0; i < numMegaBytes; i++) {
		memcpy(retrieve,mallocs[i],MEGABYTE);
	}
	
	clock_gettime(CLOCK_REALTIME,&ts_end);	

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	
	//Collect data
	process_mem_usage(vm, rss, page_size);
	cout << "Read : " << numMegaBytes << "MB; VM: " << vm << "kB; RAM: " << rss << "kB;";
	cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	fprintf(file,"%u %li %li %lu\n", numMegaBytes,(long)vm,(long)rss, time_taken);
	return numMegaBytes;
}
