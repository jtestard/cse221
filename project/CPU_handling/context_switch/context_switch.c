#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#define ITERATIONS 1000
#define TIMING_OVERHEAD 40

int compare(const void *, const void *);
void GetElapsedTime(uint64_t *);
void Loop(void);

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

    printf("[*] Starting context switch experiment...\n");

    GetElapsedTime(times);

    for (i = 0; i < ITERATIONS; i++) {
        results[i] = times[i] - TIMING_OVERHEAD;
        printf("Iteration %d: Context Switch = %" PRIu64 " cycles\n", i, times[i]);
    }

    qsort (results, ITERATIONS, sizeof(int), compare);

    printf("[*] The median context switch overhead across all iterations is: %d cycles\n", results[ITERATIONS/2]);
    
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

    pid_t pid;

    // Create pipe here since we do not need to consider
    // how long it takes to setup
    int pipefd [2];
    char buff;
    
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

    

        /*********************************************
         * Code to be benchmarked goes here
         *********************************************/

        if ((pid = fork()) == -1) {
            perror("[!] failed fork");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
    //        close(pipefd[1]);
         asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );
           read(pipefd[0], &buff, 1);
       //     close(pipefd[0]);
        } else {
     //       close(pipefd[0]);
            write(pipefd[1], "a", 1);
      //      close(pipefd[1]);
            exit(EXIT_SUCCESS);
        }


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
