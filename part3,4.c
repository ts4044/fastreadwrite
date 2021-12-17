#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

double fileread(char *fileName, int blocksize, int blockcount) {
    clock_t start, end;
    double cpu_time_used;
    int iterations = 0;

    start = clock();
    unsigned int *buf = malloc(blocksize);
    int fd = open(fileName, O_RDONLY);
    
    readahead(fd, 0, 0);

    size_t totalbytes = 0;
    size_t bytes_read;

    do {        
        bytes_read = read(fd, buf, blocksize);
        totalbytes += bytes_read;
        iterations++;
        if(iterations == blockcount + 1) {
            break;
        }
    } while (bytes_read != 0);
    close(fd);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return (double)totalbytes / (1024 * 1024 * cpu_time_used);
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    int blocksize, blockcount;

    blocksize = atoi(argv[2]);
    blockcount = atoi(argv[3]);
    
    printf("Uncached : %f MiB/s\n", fileread(fileName, blocksize, blockcount));

    double output = 0;
    for (int i = 0; i < 4; i++) {
        output += fileread(fileName, blocksize, blockcount);
    }
    printf("Cached : %f MiB/s\n", output/4);
    return 0;
}