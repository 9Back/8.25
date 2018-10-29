#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64

double time_nsec(void) {
    vlong time_begin = nsec();

	ulong i;
	// start trying to read time
    for (i = 0; i < 16384; i++) {
		nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();

        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
        nsec();
	}

    vlong time_end = nsec();
    return (time_end - time_begin) / (16384.0 * 16.0);
}

void main(int argc, char *argv[]) {
	// Contains trial timings in nanoseconds
    vlong trials[TRIALS_SIZE];
    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials[i] = time_nsec();
	} 
	
	double mean;
    for (i = 0; i < TRIALS_SIZE; i++) {
		mean += trials[i];
		print("Trial: %d, time: %d\n", i, trials[i]);
	}
    mean = mean / TRIALS_SIZE;

	double stddev;
	for (i = 0; i < TRIALS_SIZE; i++) {
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
    stddev = stddev / TRIALS_SIZE;
    stddev = sqrt(stddev);

	print("mean: %f\t stddev: %f\n", mean, stddev);

	exits(nil);
}
