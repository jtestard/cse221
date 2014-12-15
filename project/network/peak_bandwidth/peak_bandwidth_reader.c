/*
* Server Side code to Measure peak bandwidth for the TCP protocol
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

#define MAXCONNECTIONS 1 /* Maximum Simultaneous Connections Allowed */

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
    }

    qsort (results, ITERATIONS, sizeof(int), compare);

    printf("[**] The median bandwidth is: %.4f bytes per cycles\n", BUFSIZE / (float) results[ITERATIONS/2]);
    printf("[**] The minimum bandwidth is: %.4f bytes per cycles\n", BUFSIZE / (float) results[0]);
    printf("[**] The maximum bandwidth is: %.4f bytes per cycles\n", BUFSIZE / (float) results[ITERATIONS-1]);
    
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
    int buff_size = BUFSIZE;
    int c_sock_fd, s_sock_fd, status, len;
    struct sockaddr_in server_addr, client_addr;
    char buf[INPUTBUFFERSIZE];

    memset(&server_addr, 0, sizeof server_addr);
    memset(&client_addr, 0, sizeof client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(RPORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*********************************************
     * Setup the TCP server.
     *********************************************/

    if ((s_sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("[!] Server: socket");
        return 1;
    }

    setsockopt(s_sock_fd, SOL_SOCKET, SO_RCVBUF, &buff_size, (int)sizeof(buff_size));

    if (bind(s_sock_fd, (struct sockaddr *) &server_addr, 
                                            sizeof(server_addr)) < 0) {
        perror("[!] Server: bind");
        return 1;
    }

    if (listen(s_sock_fd, MAXCONNECTIONS) < 0) {
        perror("[!] Server: listen");
        return 1;
    }

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
        unsigned int clientlen = sizeof(server_addr);
        int received = -1;
        int bytes_read = 0;

        printf("[*] Iteration %d: Waiting on a connection... ", i);
        if ((c_sock_fd = accept(s_sock_fd, (struct sockaddr *) &client_addr, &clientlen)) < 0) {
            perror("[!] Server: accept");
            return 1;
        }
        printf("Client Connected!\n");

        asm volatile (
                     "CPUID\n\t"
                     "RDTSC\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx"
                     );


        /*********************************************
         * Start reading data!
         *********************************************/
        while (1) {
            if ((received = recv(c_sock_fd, buf, INPUTBUFFERSIZE, 0)) < 0) {
                perror("[!] Server: Done reading data");
                return 1;
            } else if (received == 0) {
                printf("    Client finished sending\n");
                break;
            } else {
                if (VERBOSE)
                    printf("    Client recieved %d bytes...\n", received);
                bytes_read += received;
            }
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

        printf("    Total Bytes Read = %d\n", bytes_read);
        printf("    Total Cycles = %d\n", (int) times[i]);
        close(c_sock_fd);
    }
    
    // close the socket
    close(s_sock_fd);

    return 0;
}
