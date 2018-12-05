#include <u.h>
#include <libc.h>
#include <stdio.h>

// TODO change this MAX_SIZE to something greater than 1GB (30)
#define MAX_SIZE 28 // file sizes up to 2^MAX_SIZE bytes
#define MAX_SIZE 28
#define MAX_FILENAME_SIZE 64

void main(void) {
    char* data = malloc(1 << MAX_SIZE);
    int fd = open("/dev/random", OREAD);
	for (int i = 0; i < MAX_SIZE; i++) {
        // get 2^i random bytes
		read(fd, data, (1 << i));

        char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
        snprintf(filename, MAX_FILENAME_SIZE, "%d.bin", (1 << i));
        
        int prep_fd = create(filename, OWRITE | OTRUNC, 420);
        write(prep_fd, data, (1 << i));
        close(prep_fd); 

        free(filename);
	}
    close(fd);
    free(data);
	exits(nil);
}

