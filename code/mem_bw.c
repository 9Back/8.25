#include <u.h>
#include <libc.h>

#define ARRAY_SIZE 5000000
#define TRIALS_SIZE 250

double calc_mean(double trials[TRIALS_SIZE]) 
{
	double mean = 0.0;

	ulong i;
	for (i = 0; i < TRIALS_SIZE; i++) 
	{
		mean += trials[i];
	}
	mean = mean / TRIALS_SIZE;

	return mean;
}

double calc_stddev(double trials[TRIALS_SIZE], double mean) 
{
	double stddev = 0.0;

	ulong i;
	for (i = 0; i < TRIALS_SIZE; i++) 
	{
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
	stddev = stddev / TRIALS_SIZE;
	stddev = sqrt(stddev);

	return stddev;
}

void main(int argc, char *argv[]) {

	double time_tot = 0.0;
	uvlong time_begin, time_end;

	double loop_time = 0;
	for (int i = 0; i < TRIALS_SIZE; i++) {
		cycles(&time_begin);
		for (int j = 0; j < ARRAY_SIZE; j++) {
		}
		cycles(&time_end);
		loop_time += time_end - time_begin;
	}
	loop_time /= TRIALS_SIZE;

	double timings[TRIALS_SIZE];
	double trials_comp[TRIALS_SIZE];
	int *p = (int*) malloc(ARRAY_SIZE*sizeof(int));

	for(int i=0;i< TRIALS_SIZE;i++)
	{
		cycles(&time_begin);
		memset((void*) p,0,4*ARRAY_SIZE-1);
		cycles(&time_end);
		timings[i] = (double) (time_end - time_begin);
	}

	double mean = calc_mean(timings);
	double std_dev = calc_stddev(timings, mean);
	double cycles_per_byte = mean / (ARRAY_SIZE * 4);
	double gb_per_second = ((1 / cycles_per_byte) * 2500000000) / (1024 * 1024 * 1024);

	print("Measuring setting(writing) memory\n");
	print("Mean Cycles: %f\t Std Dev: %f\n", mean, std_dev);
	print("Mean Gb_per_second: %f\t\n", gb_per_second);
	print("array size %d\n", ARRAY_SIZE);

	time_tot = 0;
	for(int i=0;i<TRIALS_SIZE;i++)
	{
		cycles(&time_begin);
		for (int j = 0; j < ARRAY_SIZE; j++) {
			p[j];
		}
		cycles(&time_end);
		timings[i] = (double) (time_end - time_begin);
	}

	mean = calc_mean(timings) - loop_time;
	std_dev = calc_stddev(timings, mean);
	cycles_per_byte = mean / (ARRAY_SIZE * 4);
	gb_per_second = ((1 / cycles_per_byte) * 2500000000) / (1024 * 1024 * 1024);

	print("Measuring setting(reading) memory\n");
	print("Mean Cycles: %f\t Std Dev: %f\n", mean, std_dev);
	print("Mean Gb_per_second: %f\t\n", gb_per_second);
	print("array size %d\n", ARRAY_SIZE);

	exits(nil);
}
