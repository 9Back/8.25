#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <disk.h>

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
#define TRIALS 2
#define INNER_TRIALS 2
#define READ_STRIDE (1 << 14)   // read READ_STRIDE bytes at a time

/**
 * Note:
 * In testing the file read time, the assignment directs us to use the raw
 * device interface in order to read directly from the disk. It does not appear
 * that 9front exposes any such interface - checking the source code for `dd`
 * (which we suspect should use some sort of raw I/O interface) - still uses
 * the regular open(2), read(2), etc. interfaces.
 *
 * We try to use setvbuf(fp, 0, _IONBF, 0) to turn off buffering for a given
 * fp.
 */

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

double do_sequential_trial(int size) {
    ulong num_bytes = (1 << size);
    char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
    snprintf(filename, MAX_FILENAME_SIZE, "%d.bin", num_bytes);

    FILE* fp = fopen(filename, "r");

	uvlong time_s = 0;
	uvlong time_e = 0;

    // buffer and bufsize params are not needed with _IONBF.
    // try to set the file in no buffering mode
    setvbuf(fp, 0, _IONBF, 0);

    int read_size = READ_STRIDE;
    int actual_read_size = num_bytes < read_size ? num_bytes : read_size;

	char* data = malloc(actual_read_size * sizeof(char));

    int num_steps = num_bytes / actual_read_size;
    double tot_cycles = 0.0;
    for (int i = 0; i < INNER_TRIALS; i++) {
        for (int j = 0; j < num_steps; j++) {
            cycles(&time_s);
            fread(data, sizeof(char), actual_read_size, fp);
            cycles(&time_e);
            tot_cycles += time_e - time_s; 
        }
        // reset back to the beginning of the file to read sequentially thru
        fseek(fp, 0, 0);
    }
    
    free(data);
    fclose(fp);
    free(filename);

    return (double)(tot_cycles) / (INNER_TRIALS * num_steps);
}

double do_random_trial(int size) {
    ulong num_bytes = (1 << size);
    char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
    snprintf(filename, MAX_FILENAME_SIZE, "%d.bin", num_bytes);

    FILE* fp = fopen(filename, "r");

	uvlong time_s = 0;
	uvlong time_e = 0;

    // buffer and bufsize params are not needed with _IONBF.
    // try to set the file in no buffering mode
    setvbuf(fp, 0, _IONBF, 0);

    // read in blocks of 1 page at a time.
    int read_size = READ_STRIDE;
    int actual_read_size = num_bytes < read_size ? num_bytes : read_size;

	char* data = malloc(actual_read_size * sizeof(char));

    int num_steps = num_bytes / actual_read_size;
    double tot_cycles = 0.0;
    for (int i = 0; i < INNER_TRIALS; i++) {
        for (int j = 0; j < num_steps; j++) {
            cycles(&time_s);
            fread(data, sizeof(char), actual_read_size, fp);
            cycles(&time_e);
            tot_cycles += time_e - time_s; 

            int rand_index = (rand() % num_steps) * actual_read_size;
            fseek(fp, rand_index, 0);
        }
    }
    
    free(data);
    fclose(fp);
    free(filename);

    return (double)(tot_cycles) / (INNER_TRIALS * num_steps);
}

void main(void) {
	double timings_seq[MAX_SIZE][TRIALS];
	double timings_rand[MAX_SIZE][TRIALS];

    // intentionally read a different file size, to make sure all trials
    // experience the file cache time, not the L1/memory cache time
    for (int j = 0; j < TRIALS; j++) {
        for (int i = 0; i < MAX_SIZE; i++) {
            int flip = rand() % 2;
            if (flip) {
                timings_rand[i][j] = do_random_trial(i);
                timings_seq[i][j] = do_sequential_trial(i);
            } else {
                timings_seq[i][j] = do_sequential_trial(i);
                timings_rand[i][j] = do_random_trial(i);
            }
            print("power of 2: %d/%d\n", i, MAX_SIZE);
        }
        print("trial: %d/%d\n", j, TRIALS);
    }

    print("file size (bytes)\tmean (cycles)\tstddev (cycles)\n");
    print("sequential\n");
    double means[MAX_SIZE];
    double std_devs[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++) {
        means[i] = calc_mean(timings_seq[i]);
        std_devs[i] = calc_stddev(timings_seq[i], means[i]);
        print("%d\t%f\t%f\n", (1 << i), means[i], std_devs[i]);
    }
    print("random\n");
    for (int i = 0; i < MAX_SIZE; i++) {
        means[i] = calc_mean(timings_rand[i]);
        std_devs[i] = calc_stddev(timings_rand[i], means[i]);
        print("%d\t%f\t%f\n", (1 << i), means[i], std_devs[i]);
    }
	exits(nil);
}

