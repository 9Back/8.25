#include <u.h>
#include <libc.h>
#include <stdio.h>

/**
 * Before running this program, generate the files necessary using
 * 8c prep_file_cache.c
 * 8l prep_file_cache.8
 * ./8.out
 *
 * This will generate files of sizes in powers of two up to MAX_SIZE, as
 * defined in prep_file_cache.c. Therefore MAX_SIZE should match in both of
 * these files.
 */

// TODO change this MAX_SIZE to something greater than 1GB (30)
#define MAX_SIZE 28 // file sizes up to 2^MAX_SIZE bytes
#define MAX_FILENAME_SIZE 64
#define TRIALS 32
#define INNER_TRIALS 8
#define READ_STRIDE (1 << 14)   // read READ_STRIDE bytes at a time

double calc_mean(double * trials) 
{
	double mean = 0.0;

	int i;
	for (i = 0; i < TRIALS; i++) 
	{
		mean += trials[i];
	}
	mean = mean / TRIALS;

	return mean;
}

double calc_stddev(double * trials, double mean) 
{
	double stddev = 0.0;

	int i;
	for (i = 0; i < TRIALS; i++) 
	{
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
	stddev = stddev / TRIALS;
	stddev = sqrt(stddev);

	return stddev;
}

double do_size_trial(int size) {
    ulong num_bytes = (1 << size);
    char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
    snprintf(filename, MAX_FILENAME_SIZE, "%d.bin", num_bytes);

	uvlong time_s = 0;
	uvlong time_e = 0;

    int fd = open(filename, OREAD);

    // read in blocks of 4 pages at a time.
    int read_size = READ_STRIDE;
    int actual_read_size = num_bytes < read_size ? num_bytes : read_size;

	char* data = malloc(actual_read_size * sizeof(char));

    double tot_cycles = 0.0;
    for (int i = 0; i < INNER_TRIALS; i++) {
        for (int j = 0; j < (num_bytes / actual_read_size); j++) {
            cycles(&time_s);
            read(fd, data, actual_read_size);
            cycles(&time_e);
            tot_cycles += time_e - time_s; 
        }
        // reset back to the beginning of the file 
        seek(fd, 0, 0);
    }
    
    free(data);
    close(fd);
    free(filename);
    return (double)(tot_cycles) / (INNER_TRIALS * (num_bytes / actual_read_size));
}

void main(void) {
	double timings[MAX_SIZE][TRIALS];

    // intentionally read a different file size, to make sure all trials
    // experience the file cache time, not the L1/memory cache time
    for (int j = 0; j < TRIALS; j++) {
        for (int i = 0; i < MAX_SIZE; i++) {
            timings[i][j] = do_size_trial(i);
        }
    }

    print("file size (bytes)\tmean (cycles)\tstddev (cycles)\n");
    double means[MAX_SIZE];
    double std_devs[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        means[i] = calc_mean(timings[i]);
        std_devs[i] = calc_stddev(timings[i], means[i]);
        print("%d\t%f\t%f\n", (1 << i), means[i], std_devs[i]);
    }
	exits(nil);
}

