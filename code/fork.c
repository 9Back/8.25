#include <u.h>
#include <libc.h>

#define TRIALS 20000

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

void main(int argc, char *argv[]) {

	double measurements[TRIALS];
	uvlong time_begin=0;
	uvlong time_end=0;
	double time_diff=0.0;
	double mean=0.0;
	double stddev=0.0;

	for(int i=0; i<TRIALS;i++)
	{
		/*RFPROC    If set a new process is created; otherwise changes
		  affect the current process.*/

		/*RFCNAMEG  If set, the new process starts with a clean name
		  space. A new name space must be built from a mount
		  of an open file descriptor.  Is mutually exclusive
		  with RFNAMEG.*/

		/*RFCENVG   If set, the new process starts with an empty envi-
		  ronment.  Is mutually exclusive with RFENVG.*/

		cycles(&time_begin);
		int pid = rfork(RFPROC | RFCNAMEG | RFCENVG);
		cycles(&time_end);

		if (pid == 0) {
			exits(nil);
		}

		measurements[i] = (time_end - time_begin);
		mean = mean + measurements[i];
	}

	mean = calc_mean(measurements);
	stddev = calc_stddev(measurements, mean);
	print("fork without any shared stuff with parent mu: %f\n", mean);
	print("fork without any shared stuff with parent sigma: %f\n", stddev);

	for(int i=0; i<TRIALS;i++)
	{          					
		cycles(&time_begin);
		int pid = fork();
		cycles(&time_end);

		if (pid == 0) {
			exits(nil);
		}

		measurements[i] =  (time_end - time_begin);
	}

	mean = calc_mean(measurements);
	stddev = calc_stddev(measurements, mean);
	print("standard fork mu (cycles): %f\n", mean);
	print("standard fork sigma (cycles): %f\n", stddev);

	exits(nil);

}
