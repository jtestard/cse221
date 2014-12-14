#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/preempt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h> 
#include <linux/timer.h>
#define ITERATIONS 100

// Slow fibonacci function
unsigned int fibonacci (unsigned int n) {
	if ( n == 0)
		return 0;
	else if ( n == 1)
		return 1;
	else
		return fibonacci(n-1) + fibonacci(n-2);
}

// Function run by the kernel thread
int thread_fn(void* data) {
    unsigned int n = 30;
    fibonacci(n);
    return 0;
}

void inline GetElapsedTime(uint64_t *times) {
    int i;
	struct timespec ts_start,ts_end,test_of_time;

    for (i = 0; i < ITERATIONS; i++) {
	    // Data required for thread creation.
		static struct task_struct *thread;
	    char  our_thread[8]="thread";
	    void * data = NULL;

		//Not disabling preemption in order to make meaningful comparison.
		//Read time
		getnstimeofday(&ts_start);

        thread = kthread_create(thread_fn,data,our_thread);
        if((thread)) {
            wake_up_process(thread);
        }
 		kthread_stop(thread);

		//Read time
		getnstimeofday(&ts_end);
        
		test_of_time = timespec_sub(ts_end,ts_start);

        times[i] = test_of_time.tv_nsec;
    }
    
    return;
}

static int __init timebm_start(void)
{
    int i;
    uint64_t *times;
	uint64_t average_time = 0;

    times = kmalloc(ITERATIONS*sizeof(uint64_t*), GFP_KERNEL);

    if (!times) {
        printk(KERN_ERR "Failed to allocate times memory\n");
        return 0;
    }

    printk(KERN_CRIT "Loading Timing Benchmark module...\n");

    GetElapsedTime(times);

    for (i = 0; i < ITERATIONS; i++) {
        average_time += times[i];
    }

    average_time /= ITERATIONS;
    
    printk(KERN_CRIT "The average elapsed time is: %llu\n", average_time);
    
    return 0;
}

static void __exit timebm_end(void)
{
    printk(KERN_CRIT "Time Benchmark module removed\n");
}

module_init(timebm_start);
module_exit(timebm_end);
