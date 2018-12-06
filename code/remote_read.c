#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <disk.h>

/**
 * Before running this program, mount the remote file system using:
 * % 9fs 9front
 *
 * This will mount a remote directory into /n/. We will read the *.tgz files
 * from /n/pkg/386, because they seem like nice files to read.
 */

#define TRIALS 4
#define INNER_TRIALS 4
#define READ_STRIDE (1 << 12)   // read READ_STRIDE bytes at a time
#define MAX_FILENAME_SIZE 256

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

double do_trial(void) {
    char *dir_name = "/n/pkg/386";
    // Assume the directory is already mounted.
 
    Dir* buf_dir;
    int fd_dir = open(dir_name, OREAD);
    if (fd_dir < 0) {
        print("open error: %r\n");
    }

    long dir_size = dirreadall(fd_dir, &buf_dir);
    vlong total_length = 0;
    for (long i = 0; i < dir_size; i++) {
        Dir cur_dir = buf_dir[i];
        print("Will read: %s\t%lld\n", cur_dir.name, cur_dir.length);

        total_length += cur_dir.length;
    }
    free(buf_dir);

	uvlong time_s = 0;
	uvlong time_e = 0;

    char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
	char* data = malloc(READ_STRIDE * sizeof(char));

    double tot_cycles = 0.0;
    for (int i = 0; i < INNER_TRIALS; i++) {
        for (long i = 0; i < dir_size; i++) {
            Dir cur_dir = buf_dir[i];
            snprintf(filename, MAX_FILENAME_SIZE, "/n/pkg/386/%s", cur_dir.name);
            int fd = open(filename, OREAD);

            int read_times = cur_dir.length / READ_STRIDE;
            if (cur_dir.length % READ_STRIDE > 0) {
                // take into account filesizes not perfectly fitting
                read_times += 1;
            }
            for (int i = 0; i < read_times; i++) {
                if (i == read_times - 1 && cur_dir.length % READ_STRIDE > 0) {
                    cycles(&time_s);
                    read(fd, data, cur_dir.length % READ_STRIDE); 
                    cycles(&time_e);
                } else {
                    cycles(&time_s);
                    read(fd, data, READ_STRIDE);
                    cycles(&time_e);
                }
                tot_cycles += time_e - time_s; 
            }
            close(fd);
        }
    }

    free(data);

    // cycles per byte
    return ((double)(tot_cycles) / total_length) / INNER_TRIALS;
}


void main(void) {
	double timings[TRIALS];

    // intentionally read a different file size, to make sure all trials
    // experience the file cache time, not the L1/memory cache time
    for (int j = 0; j < TRIALS; j++) {
        timings[j] = do_trial();
    }

    print("mean (cycles/byte)\tstddev (cycles/byte)\n");
    double mean = calc_mean(timings);
    double std_dev = calc_stddev(timings, mean);

    print("%d\t%f\t%f\n", 0, mean, std_dev);
	exits(nil);
}

