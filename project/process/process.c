#include <linux/hardirq.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/preempt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>

#define ITERATIONS 10
#define STACK_SIZE 4096

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
int process_fn(void* data) {
    unsigned int n = 30;
    printk(KERN_INFO "Kernal Thread Fibonacci n=%u", n);
    fibonacci(n);
    printk(KERN_INFO "Kernal Thread Fibonacci done");
    return 0;
}

void inline GetElapsedTime(uint64_t *times) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

    for (i = 0; i < ITERATIONS; i++) {
	    // Data required for thread creation.
	    void * child_stack = kmalloc(STACK_SIZE,__GFP_NOWARN);
		
		//CLONE_NEWPID
        preempt_disable();
        raw_local_irq_save(flags);
    
        asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

        /*********************************************
         * Code to be benchmarked goes here
         *********************************************/
		fork (&process_fn, child_stack+STACK_SIZE, CLONE_NEWPID, NULL);

        asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

        raw_local_irq_restore(flags);
        preempt_enable();

        start = (((uint64_t) cycles_high << 32) | cycles_low);
        end = (((uint64_t) cycles_high1 << 32) | cycles_low1);

        times[i] = end - start;
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
        printk(KERN_CRIT "The elapsed time is: %llu\n", times[i]);

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
