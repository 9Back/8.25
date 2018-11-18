#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64

double time_cycles(void) {
    uvlong time_begin, time_end;

    uvlong tmp;
	ulong i;

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

double calc_mean(double trials[TRIALS_SIZE]) {
	double mean = 0.0;

	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
		mean += trials[i];
    }
    mean = mean / TRIALS_SIZE;

    return mean;
}

double calc_stddev(double trials[TRIALS_SIZE], double mean) {
	double stddev = 0.0;

    ulong i;
	for (i = 0; i < TRIALS_SIZE; i++) {
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
    stddev = stddev / TRIALS_SIZE;
    stddev = sqrt(stddev);

    return stddev;
}

void main(int argc, char *argv[]) {
	// Contains trial timings in num cycles
    double trials[TRIALS_SIZE];
    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials[i] = time_cycles();
	} 

    double mean = calc_mean(trials);
    double stddev = calc_stddev(trials, mean);

	print("cycles: mean: %f\t stddev: %f\n", mean, stddev);

	exits(nil);
}
