#include "memory_utils.h"

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

unsigned int write_megabyte(char ***mallocs_ptr, unsigned int numMegaBytes, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	fprintf(file, "writing megabytes...\n"); //Added for debugging. TODO: remove

	*mallocs_ptr = (char**) malloc(sizeof(char*) * numMegaBytes);
	unsigned int i,j,reached = 0;
	for (i = 0; i < numMegaBytes; i++) {
		void* p = malloc(sizeof(char) * MEGABYTE);
		if (!p) {
			printf(
					"No more memory available! Memory allocated up to now : %i megabytes.\n",
					i);
			reached = i;
			break;
		}
		(*mallocs_ptr)[i] = (char*) p;

		//Now fill in the malloc with some data, which forces the OS
		//to have this data in memory.
		clock_gettime(CLOCK_REALTIME,&ts_start);	
		
		strcpy((*mallocs_ptr)[i],ONE_KB_STRING);
		for (j=0; j<ALLOC_SIZE-1; j++)
			strcat((*mallocs_ptr)[i],ONE_KB_STRING);
		
		clock_gettime(CLOCK_REALTIME,&ts_end);	
		test_of_time = diff(ts_start,ts_end);
		time_taken = test_of_time.tv_nsec;
		
		//Collect data
		process_mem_usage(vm, rss, page_size);
		cout << "Allocated : " << i+1 << "MB; VM: " << vm << "kB; RAM: " << rss << "kB;";
		cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
		fprintf(file,"%i %li %li %lu\n",i+1,(long)vm,(long)rss, time_taken);
	}
	if (!reached) {
		printf("Memory successfully allocated!");
		reached = numMegaBytes;
	}
	return reached;
}

unsigned int read_kilobytes(char ***mallocs_ptr, unsigned int numKiloBytes, FILE* file, bool random) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	char* retrieve;
	unsigned int i,j;
	unsigned int mb_offset = numKiloBytes / KILOBYTE; //Number of megabytes requested.
	unsigned int kb_offset = numKiloBytes % KILOBYTE; //Should always be 0 in our case, but it could be non zero in other cases.
	
	void* p = malloc(KILOBYTE * sizeof(char));
	if (!p) {
		printf("Not enough memory available!");
		return 0;
	}
	retrieve = (char*) p;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);	
	
	for (i = 0; i < mb_offset; i++) {
		for (j = 0; j < kb_offset; j++) {
			memcpy(retrieve,(*mallocs_ptr)[i]+j,KILOBYTE);
		}
	}
	
	clock_gettime(CLOCK_REALTIME,&ts_end);	

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	
	//Collect data
	process_mem_usage(vm, rss, page_size);
	cout << "Read : " << numKiloBytes << "kB; VM: " << vm << "kB; RAM: " << rss << "kB;";
	cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	fprintf(file,"%u %li %li %lu\n", numKiloBytes,(long)vm,(long)rss, time_taken);
	
	//Free retrieve pointer
	free(retrieve);
	return numKiloBytes;
}

void read_all_memory(char ***mallocs_ptr, unsigned int numMegaBytes, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	char* retrieve[1024];
	unsigned int i;
	for (i = 0 ; i < numMegaBytes; i++) {
		clock_gettime(CLOCK_REALTIME,&ts_start);	
		memcpy(retrieve,(*mallocs_ptr)[i],1024); // Copy part of the string at mallocs[i]
	    clock_gettime(CLOCK_REALTIME,&ts_end);	
	    test_of_time = diff(ts_start,ts_end);
	    time_taken = test_of_time.tv_nsec;
	    
		//Collect data
	    process_mem_usage(vm, rss, page_size);
	    cout << "Read : " << numMegaBytes << "kB; VM: " << vm << "kB; RAM: " << rss << "kB;";
	    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	    fprintf(file,"%u %li %li %lu\n", numMegaBytes,(long)vm,(long)rss, time_taken);
	}
}
