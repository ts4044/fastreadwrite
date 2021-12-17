#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void testsize(char *fileName, int blocksize)
{
    unsigned int *buf = malloc(blocksize);
    clock_t start, end;
    long blockcount = 0;
    double diff = 0;

    int fd = open(fileName, O_RDONLY);

    start = clock();
    while (diff < 7) {
        int bytes = pread(fd, buf, blocksize, 0);
        blockcount++;
        diff = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
    printf("%d\t%ld\t%ld\t%f\n", blocksize, blockcount, blocksize * blockcount, diff);
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    int blocksize;

    if(argc > 2) {
        blocksize = atoi(argv[2]);
    } else {
        blocksize = 100000;
    }
    
    testsize(fileName, blocksize);

    return 0;
}