#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64

double time_loop(void) {
    double time_total = 0.0;

    double work = 1.1;
	ulong i;

    vlong time_end, time_begin;
    uvlong time_begin, time_end

    // Calculate loop overhead 16384 times.
    // Loop 16385 times to discard the first.
    for (i = 0; i < 16385; i++) {
        cycles(&time_end);
        if (i != 0) {
            double delta = time_end - time_begin;
            time_total = time_total + delta;
        }
        work = (1.1 * work + 0.7) / work;
        cycles(&time_begin);
	}

    return time_total / 16384.0;
}

double calc_mean(vlong trials[TRIALS_SIZE]) {
	double mean = 0.0;

	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
		mean += trials[i];
    }
    mean = mean / TRIALS_SIZE;

    return mean;
}

double calc_stddev(vlong trials[TRIALS_SIZE], double mean) {
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
	// Contains trial timings in nanoseconds
    vlong trials[TRIALS_SIZE];
    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials[i] = time_loop();
	} 

    double mean = calc_mean(trials);
    double stddev = calc_stddev(trials, mean);

	print("(cycles) mean: %f\t stddev: %f\n", mean, stddev);

	exits(nil);
}
