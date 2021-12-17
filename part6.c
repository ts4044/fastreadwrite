#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

void readmmap(char *fileName, int blocksize, int blockcount) {
    clock_t starttime, endtime;
    double cpu_time_used;

    starttime = clock();

    int fd = open(fileName, O_RDONLY);
    struct stat filestat;

    fstat(fd, &filestat);
    u_int64_t filesize = filestat.st_size;
    u_int64_t size;
    unsigned int *start;

    if(blockcount != -1) {
        size = (u_int64_t)blocksize * blockcount;
        if (size > filesize) {
            size = filesize;
        }
    } else {
        size = filesize;
    }

    start = (unsigned int *)(mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0));

    unsigned int XOR = 0;
    for (u_int64_t i = 0; i < size / sizeof(unsigned int); i++) {
        XOR ^= start[i];
    }

    endtime = clock();
    cpu_time_used = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;

    printf("Time taken: %f\nSpeed = %f MiB/s\n", cpu_time_used, size / (1024 * 1024 * cpu_time_used));
    printf("Size read = %lu\n", size);
    printf("%08x\n", XOR);
    close(fd);
    munmap(start, size);
}

int main(int argc, char *argv[])
{
    char *fileName = argv[1];
    
    int blocksize, blockcount;
    
    if(argc > 2) {
        blocksize = atoi(argv[2]);
    } else {
        blocksize = 100000;
    }
    if(argc > 3) {
        blockcount = atoi(argv[3]);
    } else {
        blockcount = -1;
    }

    readmmap(fileName, blocksize, blockcount);
    
    return 0;
}