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

double do_trial() {
    char *dir_name = "/n/pkg/386";
    // Assume the directory is already mounted.
 
    Dir *buf_dir;
    int fd_dir = open(dir_name, OREAD);

    long dir_size = dirreadall(fd_dir, &buf_dir);
    vlong total_length = 0;
    for (long i = 0; i < dir_size; i++) {
        Dir* cur_dir = buf_dir[i];
        print("%s\t%lld\n", cur_dir->name, cur_dir->length);

        total_length += cur_dir->length;
        free(cur_dir);
    }
    free(buf_dir);

	uvlong time_s = 0;
	uvlong time_e = 0;

	char* data = malloc(READ_STRIDE * sizeof(char));

    double tot_cycles = 0.0;
    /*
    for (int i = 0; i < INNER_TRIALS; i++) {
        cycles(&time_s);
        fread(data, sizeof(char), actual_read_size, fp);
        cycles(&time_e);
        tot_cycles += time_e - time_s; 
    }
    */

    free(data);

    return (double)(tot_cycles) / INNER_TRIALS;
}


void main(void) {
	double timings[TRIALS];

    // intentionally read a different file size, to make sure all trials
    // experience the file cache time, not the L1/memory cache time
    for (int j = 0; j < TRIALS; j++) {
        timings[j] = do_trial(j);
    }

    print("file size (bytes)\tmean (cycles)\tstddev (cycles)\n");
    double mean = calc_mean(timings);
    double std_dev = calc_stddev(timings, mean);

    print("%d\t%f\t%f\n", 0, mean, std_dev);
	exits(nil);
}

