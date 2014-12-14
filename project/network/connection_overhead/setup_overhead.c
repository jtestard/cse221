/*
* Measure connection setup overhead for the TCP protocol
* 
* RUN the following command on remote host for simple echo server:
* ncat -v -l 2000 --keep-open --exec "/bin/cat"
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 1

// Determined from previous experiment
#define TIMING_OVERHEAD 40

int compare(const void *, const void *);
int GetElapsedTime(uint64_t *);
void Loop(void);

int main(void)
{
    int i;
    uint64_t *times;
    int results[ITERATIONS];
    double loop_time;
    int rv;

    times = malloc(ITERATIONS*sizeof(uint64_t*));

    if (!times) {
        printf("[!] Failed to allocate times memory\n");
        return 0;
    }

    printf("[*] Loading Timing Benchmark module...\n");

    rv = GetElapsedTime(times);

    for (i = 0; i < ITERATIONS; i++) {
        loop_time = (times[i] - TIMING_OVERHEAD);
        results[i] = (int) loop_time;

        printf("Iteration %d: Setup Time: %" PRIu64 " cycles\n", i, times[i]);
    }

    qsort (results, ITERATIONS, sizeof(int), compare);

    printf("[*] The median Setup Time is: %d cycles\n", results[ITERATIONS/2]);
    printf("[*] The minimum Setup Time is: %d cycles\n", results[0]);
    printf("[*] The maximum Setup Time is: %d cycles\n", results[ITERATIONS-1]);
    
    return rv;
}

int compare (const void * a, const void * b) 
{
    return ( *(int*)a - *(int*)b );
}

int inline GetElapsedTime(uint64_t *times) {
    unsigned long flags;
    uint64_t start, end;
    int i;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    volatile int variable = 0;

   /**************************************************
    * Initialize structs for a TCP connection
    **************************************************/
    int sock_fd, status, len, bytes_read;
    struct addrinfo remote;
    struct sockaddr_in remote_addr;
    struct in_addr raddr;
    char buf[BUFSIZE];
    char *msg = "A";

    len = strlen(msg);
    memset(&remote, 0, sizeof remote);
    memset(&remote_addr, 0, sizeof remote_addr);

    remote.ai_family = AF_INET;
    remote.ai_socktype = SOCK_STREAM;

    raddr.s_addr = inet_addr(RADDR);

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(RPORT);
    remote_addr.sin_addr = raddr;

   /**************************************************
    * Setup Timing Harness 
    **************************************************/
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
         * Setup the TCP connection.
         *********************************************/

        if ((sock_fd = socket(remote.ai_family, remote.ai_socktype, remote.ai_protocol)) == -1) {
            perror("[!] client: socket");
            return 1;
        }

        if (connect(sock_fd, (struct sockaddr *) &remote_addr, sizeof remote_addr) == -1) {
            close(sock_fd);
            perror("[!] client: connect");
            return 1;
        }

        asm volatile (
                     "RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::"%rax", "%rbx", "%rcx", "%rdx"
                     );

        // close the socket
        close(sock_fd);

        start = (((uint64_t) cycles_high << 32) | cycles_low);
        end = (((uint64_t) cycles_high1 << 32) | cycles_low1);

        times[i] = end - start;
    }
    
    return 0;
}
