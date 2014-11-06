/*
 * cgroups.c
 *
 *  Created on: Feb 4, 2013
 *      Author: jtesta
 */
#include <cstdlib>
#include <ios>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unistd.h>

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


//This file mallocs chunks of memory and signals when no more memory can be accessed (up to some user defined
//limit, of course.
int main(int argc, char **argv, char **envp) {
	using std::cout;
	using std::endl;

	//Start time counter
	time_t tstart, tend;
	tstart = time(0);

	//We are supposed to have an argument specifying the size of the memory to allocate
	//in megabytes.
	int numMegaBytes = 0;
	const int MEGABYTE = 1048576;
	char one_kb_string[1024] = "123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456712345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456788";
	const int ALLOC_SIZE = MEGABYTE/1024;
	double vm, rss;
	long page_size;
	char filename[100];
	char filename2 [100];

	if (argc <= 1) {
		printf(
				"You should have one argument specifying the memory you wish to allocate in megabytes.\n");
		exit(0);
	}

	//Prepare file pointer to write allocation evolution in terms of VM and RAM.
	numMegaBytes = atoi(argv[1]);
	FILE *file;
	sprintf(filename,"csv/allocation_%iMB.csv",numMegaBytes);
	file = fopen(filename,"w");

	printf("You have requested to allocate %i megabytes...\n", numMegaBytes);
	fprintf(file,"Allocated VM RAM\n");
	//Allocate memory per chunks
	char **mallocs = (char**) malloc(numMegaBytes * sizeof(char*));
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
		strcpy(mallocs[i],one_kb_string);
		for (j=0; j<ALLOC_SIZE-1; j++)
			strcat(mallocs[i],one_kb_string);
		
		//Collect data
		process_mem_usage(vm, rss, page_size);
		cout << "Allocated : " << i << "MB; VM: " << vm << "kB; RAM: " << rss << "kB; RAM PageSize:" << page_size << "kB"<< endl;
		fprintf(file,"%i %li %li\n",i,(long)vm,(long)rss);
	}
	if (!reached) {
		printf("Memory successfully allocated!");
		reached = numMegaBytes;
	}

	//Here we measure access time in C for memory at different places in the array.
	FILE *file2;
	sprintf(filename2,"csv/lookup_%iMB.csv",numMegaBytes);
	file2 = fopen(filename2,"w");
	printf("Now performing some memory lookup and measuring access times.\n");
	clock_t t;
	char retrieve[1024];
	fprintf(file2,"MemoryArea Microseconds\n");
	for (i = 0 ; i < numMegaBytes; i++) {
		t = (double)clock();
		memcpy(retrieve,mallocs[i],1024);
		t = (double)clock()-t;
		cout << "Memory Area : " << i << "; Microseconds : " << t << endl;
		fprintf(file2,"%i %li\n",i,(long)t);
	}

	//Free the memory
	printf("Now freeing...\n");
	for (i = 0; i < reached; i++) {
		free(mallocs[i]);
	}
	free(mallocs);

	//close the file
	fclose(file);

	//Print the time taken for the allocation operations
	tend = time(0);
	cout << "The process took " << difftime(tend, tstart) << " second(s)." << endl;

	printf("Good Bye!\n");
}
