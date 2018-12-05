#include <u.h>
#include <libc.h>

#define MAX_SIZE 26
#define MAX_FILENAME_SIZE 256

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
	}
    close(fd);
    free(data);
	exits(nil);
}

