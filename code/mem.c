#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64


volatile int d = 0.0


double memory_access(void) {
    char err_str[4];
    uvlong time_tot = 0.0;
    uvlong time_begin, time_end;

	ulong i;
	// start trying to read time
    for (i = 0; i < 16384; i++) {
        cycles(&time_begin);
        if(d==0)
	{
	}
        cycles(&time_end);
        time_tot = time_tot + (time_end - time_begin);
	}

    return time_tot / (16384.0);
}

double comparison(void) {
    char err_str[4];
    vlong time_tot = 0.0;
    vlong time_begin, time_end;

	ulong i;
	// start trying to read time
    for (i = 0; i < 16384; i++) {
        cycles(&time_begin);
        if(0==0)
	{
	}
        cycles(&time_end);
        time_tot = time_tot + (time_end - time_begin);
	}

    return time_tot / (16384.0);
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
    vlong trials_mem[TRIALS_SIZE];
    vlong trials_comp[TRIALS_SIZE];

    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials_mem[i] = memory_access();
         trials_comp[i] = comparison();
	} 

    double mean_1 = calc_mean(trials_mem);
    double stddev_1 = calc_stddev(trials_mem, mean_1);

    double mean_2 = calc_mean(trials_comp);
    double stddev_2 = calc_stddev(trials_comp, mean_2);

	print("Measuring loading from ram + comparison in cycles\n");
	print("mean: %f\t stddev: %f\n", mean_1, stddev_1);

	print("Measuring only comparison in cycles\n");
	print("mean: %f\t stddev: %f\n", mean_2, stddev_2);

   double mean_12 = mean_1 - mean_2;

   double stddev_12 = sqrt(stddev_1*stddev_1 + stddev_2*stddev_2);

	print("Only loading from ram in cycles\n");
	print("mean: %f\t stddev: %f\n", mean_12, stddev_12);


	exits(nil);
}
