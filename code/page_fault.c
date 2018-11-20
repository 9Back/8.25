#include <u.h>
#include <libc.h>

#define TRIALS 1000000

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

void main(void) {
	uvlong time_s = 0;
	uvlong time_e = 0;
	double timings[TRIALS];
	int stride = ((1 << 10) * 4) + 1;
	char * gig1 = malloc(4 * (1 << 10));
	for (int i = 0; i < TRIALS; i++) {
		int fd = open("/dev/random", OREAD);
		cycles(&time_s);
		read(fd, gig1, 1);
		cycles(&time_e);
		close(fd);
		timings[i] = (double) (time_e - time_s);
	}
	double mean = calc_mean(timings);
	double std_dev = calc_stddev(timings, mean);
	print("Mean (nsec): %f, StdDev (nsec): %f\n", mean / 2.5, std_dev / 2.5);
	free(gig1);
	exits(nil);
}

