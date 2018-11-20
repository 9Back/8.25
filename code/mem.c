#include <u.h>
#include <libc.h>

#define TRIALS 100

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
	uvlong res = 0;
	uvlong total = 0;
	double timings[TRIALS];
	char * mything = malloc(1600 * (1 << 20));

	print("Stride, Mean, StdDev\n");
	for (int stride = 1; stride < ((1 << 20) * 16); stride = ((stride * 1.01) + 1)) {
		for (int i = 0; i < TRIALS; i++) {
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
