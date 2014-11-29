#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#define ITERATIONS 10000
#define TIMING_OVERHEAD 40
#define STACK_SIZE 65536

int pipefd [2];
char buff;

int compare(const void *, const void *);
void GetElapsedTime(uint64_t *);
int writeToPipe(void *);

int main(void)
{
    int i;
    uint64_t *times;
    int results[ITERATIONS];

    times = malloc(ITERATIONS*sizeof(uint64_t*));

    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }

    printf("[*] Starting kthread context switch experiment...\n");

    GetElapsedTime(times);
    printf("DEBUG");

    for (i = 0; i < ITERATIONS; i++) {
        results[i] = times[i] - TIMING_OVERHEAD;
        printf("Iteration %d: kthread Context Switch = %" PRIu64 " cycles\n", i, times[i]);
    }

    qsort (results, ITERATIONS, sizeof(int), compare);
    for (i = 0; i < ITERATIONS; i++) {
        printf("Iteration %d: kthread Context Switch = %d cycles\n", i, results[i]);
    }
    printf("[*] The median kthread context switch overhead across all iterations is: %d cycles\n", results[ITERATIONS/2]);
    
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

    char *stack;
    char *stackTop;
    pid_t pid;

    // Allocate child stack
   stack = malloc(STACK_SIZE);
   if (stack == NULL) {
       perror("[!] Failed stack allocation for child");
       exit(EXIT_FAILURE);
   }
   stackTop = stack + STACK_SIZE;

    // Create pipe here since we do not need to 
    // include its setup time in our measurements
    
    if (pipe(pipefd) == -1) {
        perror("[!] failed pipe");
        exit(EXIT_FAILURE);
    }

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

        pid = clone(writeToPipe, stackTop, CLONE_VM, NULL);

        if (pid == -1) {
            perror("[!] Failed to clone thread");
            exit(EXIT_FAILURE);
        }

        read(pipefd[0], &buff, 1);

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

int writeToPipe(void * arg) {
    write(pipefd[1], "a", 1);
    exit(EXIT_SUCCESS);
}
