#include <u.h>
#include <libc.h>

#define TRIALS 500

double time_cycles(void) {
	uvlong time_begin, time_end;

	uvlong tmp;
	int i;

	cycles(&time_begin);
	// start trying to read time
	for (i = 0; i < 16384; i++) {
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);

		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
		cycles(&tmp);
	}

	cycles(&time_end);
	return (time_end - time_begin) / (16384.0 * 16.0);
}

double calc_mean(double trials[TRIALS]) {
	double mean = 0.0;

	int i;
	for (i = 0; i < TRIALS; i++) {
		mean += trials[i];
	}
	mean = mean / TRIALS;

	return mean;
}

double calc_stddev(double trials[TRIALS], double mean) {
	double stddev = 0.0;

	int i;
	for (i = 0; i < TRIALS; i++) {
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
	stddev = stddev / TRIALS;
	stddev = sqrt(stddev);

	return stddev;
}

void main(int argc, char *argv[]) {
	// Contains trial timings in num cycles
	double trials[TRIALS];

	int i;
	for (i = 0; i < TRIALS; i++) {
		trials[i] = time_cycles();
	} 

	double mean = calc_mean(trials);
	double stddev = calc_stddev(trials, mean);

	print("cycles: mean (nsec): %f\t stddev (nsec): %f\n", mean / 2.5, stddev / 2.5);

	exits(nil);
}
