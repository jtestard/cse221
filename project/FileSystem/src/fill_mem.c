#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
    int page_size;
    long avail_pages;
    char * stub;

    if ((page_size = sysconf(_SC_PAGESIZE)) < 0) {
        perror("[!] getpagesize");
        return -1;
    }

    printf("[*] Page size in bytes = %d\n", page_size);

    if ((avail_pages = sysconf(_SC_AVPHYS_PAGES)) < 0) {
        perror("[!] sysconf");
        return -1;
    }

    printf("[*] Total available pages  = %lu\n", avail_pages);

    if (!(stub = malloc(avail_pages * page_size))) {
            perror("[!] malloc");
            return -1;
    }

    memset(stub, '\0', avail_pages * page_size);

    sleep(3600);
    printf("[*] Waking from sleep\n");
    return 0;
}
