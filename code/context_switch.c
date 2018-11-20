#include <u.h>
#include <libc.h>

#define TRIALS 1000

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

void main( int argc, char *argv[]) {

	double measurements[TRIALS];
	uvlong time_begin=0;
	uvlong time_end=0;
	double time_diff=0.0;
	double stddev=0.0;
	double pipe_overhead=0.0;

	//measuring pipes
	int fd[2];
	if(pipe(fd)==-1) 
	{ 
		exits(nil); 
	}

	char data = 'X';
	write(fd[1],&data,1);
	close(fd[1]);

	cycles(&time_begin);
	char data2;	
	read(fd[0],&data2,1);
	close(fd[0]);
	cycles(&time_end);

	pipe_overhead = (time_end - time_begin);


	for(int i=0;i<TRIALS;i++)
	{
		int fd[2];

		if(pipe(fd)==-1) 
		{ 
			exits(nil); 
		} 
		int p = fork();

		//parent process
		if(p > 0)
		{
			char data = 'X';
			write(fd[1],&data,1);

			cycles(&time_begin);
			wait();
			cycles(&time_end);

			close(fd[1]);

			measurements[i] = (time_end - time_begin);
			measurements[i] = measurements[i] - pipe_overhead;
			measurements[i] = measurements[i]/2.0;

		}
		else
		{
			char data2;	
			read(fd[0],&data2,1);
			close(fd[0]);
			exits(nil);
		}
	}

	double mean = calc_mean(measurements);
	print("context switch mu (nsec): %f\n", mean / 2.5);
	double std_dev = calc_stddev(measurements, mean);
	print("context switch stddev (nsec): %f\n", std_dev / 2.5);

	double last_min = 1000000000.0;
	for(int i=0;i<TRIALS;i++)
	{
		if(measurements[i] < last_min)
		{
			last_min = measurements[i];
		}
	}
	print("minimum context switch (nsec): %f\n", last_min / 2.5);

	exits(nil);
}
