#include <u.h>
#include <libc.h>
#include <stdio.h>

#define SIZE 20  // file sizes of 2^SIZE bytes
// Number of concurrent procs doing accesses to different files 
#define NUM_PROCS 64    
#define MAX_FILENAME_SIZE 64

void main(void) {
    int file_size = (1 << SIZE);
    char* data = malloc(file_size);
    int fd = open("/dev/random", OREAD);
	for (int i = 0; i < NUM_PROCS; i++) {
		read(fd, data, file_size);

        char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
        snprintf(filename, MAX_FILENAME_SIZE, "%d.contention.bin", i);
        
        int prep_fd = create(filename, OWRITE | OTRUNC, 420);
        write(prep_fd, data, file_size);
        close(prep_fd); 

        free(filename);
	}
    close(fd);
    free(data);
	exits(nil);
}

