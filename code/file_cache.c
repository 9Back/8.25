#include <u.h>
#include <libc.h>
#include <stdio.h>

#define MAX_SIZE 28
#define MAX_FILENAME_SIZE 64
#define TRIALS 32
#define INNER_TRIALS 8

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
	char* data = malloc(num_bytes * sizeof(char));

    double tot_cycles;
    for (int i = 0; i < INNER_TRIALS; i++) {
		cycles(&time_s);
		read(fd, data, num_bytes);
		cycles(&time_e);

        tot_cycles += time_e - time_s; 
    }

    free(data);
    free(filename);
    return (double)(tot_cycles) / INNER_TRIALS;
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
        print("%i\t%f\t%f\n", (1 << i), means[i], std_devs[i]);
    }
	exits(nil);
}

