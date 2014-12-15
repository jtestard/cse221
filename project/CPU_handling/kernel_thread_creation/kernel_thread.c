#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/preempt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h> 
#include <linux/timer.h>
#define ITERATIONS 3

uint64_t average_time;
void inline GetElapsedTime(void);

// Function run by the kernel thread
int thread_fn(void* data) {
	struct timespec ts_start, ts_end,test_of_time;
	ts_start = *((struct timespec*)data);
	getnstimeofday(&ts_end);
	test_of_time = timespec_sub(ts_end,ts_start);
    average_time = test_of_time.tv_nsec;
    printk(KERN_CRIT "%llu\n", average_time);
    return 0;
}

void inline GetElapsedTime(void) {
    int i;
	void* data = NULL;
    for (i = 0; i < ITERATIONS; i++) {
	    // Data required for thread creation.
		static struct task_struct *thread;
	    char our_thread[8]="thread";
		struct timespec ts_start;
		getnstimeofday(&ts_start);
		data = &ts_start;
		printk(KERN_CRIT "Thread create\n");
        thread = kthread_create(thread_fn,data,our_thread);
        if(thread) {
            wake_up_process(thread);
        }
 		kthread_stop(thread);
    }
    return;
}

static int __init timebm_start(void)
{
    printk(KERN_CRIT "Time Benchmark module started\n");
    GetElapsedTime();
    return 0;
}

static void __exit timebm_end(void)
{
    printk(KERN_CRIT "Time Benchmark module done\n");
}

module_init(timebm_start);
module_exit(timebm_end);
