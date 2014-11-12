#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define ITERATIONS 10
#define LOOPS 10

// Determined from previous experiment
#define TIMING_OVERHEAD 40

int compare(const void *, const void *);
void GetElapsedTime(uint64_t *);
void Loop(void);

int main(void)
{
    int i;
    uint64_t *times;
    double results[ITERATIONS];
    double loop_time;

    times = malloc(ITERATIONS*sizeof(uint64_t*));

    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }

    printf("[*] Loading Timing Benchmark module...\n");

    GetElapsedTime(times);

    for (i = 0; i < ITERATIONS; i++) {
        loop_time = (times[i] - TIMING_OVERHEAD) / (double) LOOPS;
        results[i] = loop_time;

        printf("Iteration %d: The total overhead for %d loops is: %" PRIu64 "\n", i, LOOPS, times[i]);
        printf("    The average loop overhead is: %f\n", loop_time);
    }

    qsort (results, ITERATIONS, sizeof(uint64_t), compare);
    printf("[*] The median loop overhead across all iterations is: %f\n", results[ITERATIONS/2]);
    
    return 0;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

void inline GetElapsedTime(uint64_t *times) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;

    asm volatile (
                 "CPUID\n\t"
                 "RDTSC\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                 );


    asm volatile (
                 "RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                 );

    for (i = 0; i < ITERATIONS; i++) {

    
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
                "mov %0, %%eax\n\t"
                "begin:\n\t"
                "test %%eax, %%eax\n\t"
                "jz end\n\t"
                "dec %%eax\n\t"
                "jmp begin\n\t"
                "end:\n\t"
                ::"r"(LOOPS):"%rax", "%rdx"
                );

        asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

        start = (((uint64_t) cycles_high << 32) | cycles_low);
        end = (((uint64_t) cycles_high1 << 32) | cycles_low1);

        times[i] = end - start;
    }
    
    return;
}
