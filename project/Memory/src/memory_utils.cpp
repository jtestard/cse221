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
		if (file != NULL) {
		    process_mem_usage(vm, rss, page_size);
		    cout << "Allocated : " << i+1 << "MB; VM: " << vm << "kB; RAM: " << rss << "kB;";
		    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
		    fprintf(file,"%i %li %li %lu\n",i+1,(long)vm,(long)rss, time_taken);
		} else {
			printf(".");
			fflush(stdout);
		}
	}
	cout << endl;
	if (!reached) {
		printf("Memory successfully allocated!");
		reached = numMegaBytes;
	}
	return reached;
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

void mmap_array(int* &mmapped_array, unsigned int size, char* filename) {
    int fd;
    int result;

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
	    perror("Error opening file for writing");
	    exit(EXIT_FAILURE);
    }
	//Strectches file size to size of int array.
    result = lseek(fd, size-1, SEEK_SET);
    if (result == -1) {
	    close(fd);
	    perror("Error calling lseek() to 'stretch' the file");
	    exit(EXIT_FAILURE);
    }
    //Necessary after using lseek.
    result = write(fd, "", 1);
    if (result != 1) {
	    close(fd);
	    perror("Error writing last byte of the file");
	    exit(EXIT_FAILURE);
    }

    mmapped_array = (int*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmapped_array == MAP_FAILED) {
	    close(fd);
	    perror("Error mmapping the file");
	    exit(EXIT_FAILURE);
    }
    
    close(fd);
}

void free_mmapped_array(int* mmapped_array, unsigned int size) {
    /* Don't forget to free the mmapped memory
     */
    if (munmap(mmapped_array, size) == -1) {
		perror("Error un-mmapping the file");
		exit(EXIT_FAILURE);
    }
}

//DEPRECATED
void make_list(node **root_ptr, unsigned int size, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	bool profile = false;
	unsigned int i;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);	
	node* last;
	node* root = (node*) malloc(sizeof(node));
	strcpy(root->data, ONE_KB_STRING);
	node* prev = root;

	for (i=1; i < size; i++) {
		node *current = (node*) malloc(sizeof(node));
		strcpy(current->data, ONE_KB_STRING);
		prev->next = current;
		if (i == size-1) {
			last = current;
		}
		prev = current;
	}
	//Make the function wrap around
	last->next = root;
	clock_gettime(CLOCK_REALTIME,&ts_end);
	*root_ptr = root;

	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	
	//Collect data
	if (profile) {
	    process_mem_usage(vm, rss, page_size);
	    cout << "List size: " << size << "; VM: " << vm << "kB; RAM: " << rss << "kB;";
	    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	    fprintf(file,"%u %li %li %lu\n", size,(long)vm,(long)rss, time_taken);
	} else {
		cout << "List size: " << size << "; Latency: " << time_taken << " nanoseconds" << endl;
	}
}

//DEPRECATED
void read_list_wrap(node **root_ptr, unsigned int iter, FILE* file) {
	using std::cout;
	using std::endl;
	double vm, rss;
	long page_size;
	struct timespec ts_start,ts_end,test_of_time;
	long unsigned time_taken;
	bool profile = true;
	unsigned int i;
	
	clock_gettime(CLOCK_REALTIME,&ts_start);	
	node* p = *root_ptr;
	for (i=1; i < iter; i++) {
		p = p->next;
	}
	clock_gettime(CLOCK_REALTIME,&ts_end);
	test_of_time = diff(ts_start,ts_end);
	time_taken = test_of_time.tv_nsec;
	
	//Collect data
	if (profile) {
	    process_mem_usage(vm, rss, page_size);
	    cout << "Number of iterations: " << iter << "; VM: " << vm << "kB; RAM: " << rss << "kB;";
	    cout << " RAM PageSize:" << page_size << "kB; Latency: " << time_taken << " nanoseconds;"<< endl;
	    fprintf(file,"%u %li %li %lu\n", iter,(long)vm,(long)rss, time_taken);
	} else {
		cout << "Number of iterations: " << iter << "; Latency: " << time_taken << " nanoseconds" << endl;
	}

}

//DEPRECATED
void free_list(node **root_ptr) {
	node* temp1 = (*root_ptr)->next;
	while(temp1!=(*root_ptr)) 
	{   
	    (*root_ptr)->next = temp1->next;
		temp1->next = NULL;
		free(temp1);
		temp1 = (*root_ptr)->next;
	}
	free((*root_ptr));
}

