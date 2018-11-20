#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 100

double calc_mean(double * trials) 
{
	double mean = 0.0;

	int i;
	for (i = 0; i < TRIALS_SIZE; i++) 
	{
		mean += trials[i];
	}
	mean = mean / TRIALS_SIZE;

	return mean;
}

double calc_stddev(double * trials, double mean) 
{
	double stddev = 0.0;

	int i;
	for (i = 0; i < TRIALS_SIZE; i++) 
	{
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
	stddev = stddev / TRIALS_SIZE;
	stddev = sqrt(stddev);

	return stddev;
}

void main(void) {
	uvlong time_s = 0;
	uvlong time_e = 0;
	uvlong res = 0;
	uvlong total = 0;
	double timings[TRIALS_SIZE];
	char * mything = malloc(1600 * (1 << 20));

	print("Stride, Mean, StdDev\n");
	for (int stride = 1; stride < ((1 << 20) * 16); stride = (int) (1.1 * stride + 1)) {
		for (int i = 0; i < TRIALS_SIZE; i++) {
			cycles(&time_s);
			if (mything[i * stride] == 420) {}
			cycles(&time_e);
			res = (double) (time_e - time_s);
			timings[i] = res;
		}
		double mean = calc_mean(timings);
		double std_dev = calc_stddev(timings, mean);
		print("%d, %f, %f\n", stride, mean, std_dev);
	}

	free(mything);
	exits(nil);
}
