#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char *argv[]) {
    char *fileName = argv[1];
    
    clock_t start, end;
    double cpu_time_used;

    for(int bytes = 1; bytes <= 8; bytes*=2) {
        // lseek()
        start = clock();
        int fd = open(fileName, O_RDONLY);
        for(int i = 0; i < 10000000; i++) {
            lseek(fd, bytes, SEEK_SET);
        }
        close(fd);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("lseek() did %d byte operations with %d bytes per operation in %lf seconds = "
            "%lf Bytes per Second = %lf MiB/s\n",10000000/bytes, bytes, cpu_time_used, 10000000.0/cpu_time_used, 10000000.0/(1024 * 1024 * cpu_time_used));


        // read()
        start = clock();
        fd = open(fileName, O_RDONLY);
        int buf1[bytes];
        for(int i = 0; i < 10000000; i++) {
            read(fd, &buf1, bytes);
        }
        close(fd);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("read() did %d byte operations with %d bytes per operation in in %lf seconds = "
            "%lf Bytes per Second = %lf MiB/s\n",10000000/bytes, bytes, cpu_time_used, 10000000.0/cpu_time_used, 10000000.0/(1024 * 1024 * cpu_time_used));


        // pread()
        start = clock();
        fd = open(fileName, O_RDONLY);
        int buf2[bytes];
        for(int i = 0; i < 10000000; i++) {
            pread(fd, &buf2, bytes, 0);
        }
        close(fd);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("pread() did %d byte operations with %d bytes per operation in in %lf seconds = "
            "%lf Bytes per Second = %lf MiB/s\n",10000000/bytes, bytes, cpu_time_used, 10000000.0/cpu_time_used, 10000000.0/(1024 * 1024 * cpu_time_used));
    }
    return 0;
}