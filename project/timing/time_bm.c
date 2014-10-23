#include <linux/hardirq.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/preempt.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define ITERATIONS 10

void inline GetElapsedTime(uint64_t *times) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;

    for (i = 0; i < ITERATIONS; i++) {

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
