#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void filewrite(char *fileName, int blocksize, int blockcount) {
    int fd = open(fileName, O_WRONLY | O_CREAT, 0666);
    int iterations = 0;
    unsigned int buf[blocksize];

    while (iterations < blockcount) {
        int byteswritten = write(fd, buf, blocksize);
        iterations++;
    }

    close(fd);
}

void fileread(char *fileName, int blocksize, int blockcount) {
    clock_t start, end;
    double cpu_time_used;
    unsigned int XOR = 0;
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
        
        for (int i = 0; i < bytes_read / sizeof(unsigned int); i++) {
            XOR ^= buf[i];
        }

        iterations++;
        if(iterations == blockcount + 1) {
            break;
        }
    } while (bytes_read != 0);
    close(fd);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Bytes Read = %ld\nTime taken: %f\nSpeed = %f MiB/s\n", totalbytes, cpu_time_used, totalbytes / (1024 * 1024 * cpu_time_used));
    printf("%08x\n", XOR);
}

int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    char *mode = argv[2];
    int blocksize, blockcount;

    if(argc > 3) {
        blocksize = atoi(argv[3]);
    } else {
        blocksize = 100000;
    }
    if(argc > 4) {
        blockcount = atoi(argv[4]);
    } else {
        blockcount = -1;
    }

    if (strcmp(mode, "-r") == 0) {
        fileread(fileName, blocksize, blockcount);
    } else if (strcmp(mode, "-w") == 0) {
        if(blockcount == -1) {
            printf("Unknown file size to write. Aborting.\n");
        } else {
            filewrite(fileName, blocksize, blockcount);
        }
    }
    return 0;
}