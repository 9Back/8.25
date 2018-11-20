#include <u.h>
#include <libc.h>

#define TRIALS 20000

uvlong time_begin=0;
uvlong time_end=0;
double time_diff=0;

void func0(void)
{
}
void func1(int a)
{
}
void func2(int a, int b)
{
}
void func3(int a, int b, int c)
{
}
void func4(int a, int b, int c, int d)
{
}
void func5(int a, int b, int c, int d, int e)
{
}
void func6(int a, int b, int c, int d, int e, int f)
{
}
void func7(int a, int b, int c, int d, int e, int f, int g)
{
}
void func8(int a, int b, int c, int d, int e, int f, int g, int h)
{
}

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
	double mean=0;
	double stddev=0;

	print("Args, Mean (nsec), StdDev (nsec)\n", mean, stddev);

	//measuring empty function
	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func0();
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 0, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func1(1);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 1, mean, stddev);


	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func2(1, 2);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 2, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func3(1, 2, 3);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 3, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func4(1, 2, 3, 4);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 4, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func5(1, 2, 3, 4, 5);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 5, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func6(1, 2, 3, 4 ,5, 6);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 6, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func7(1, 2, 3, 4, 5, 6, 7);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 7, mean, stddev);

	for(int i=0;i<TRIALS;i++)
	{
		cycles(&time_begin);
		func8(1, 2, 3, 4, 5, 6, 7, 8);
		cycles(&time_end);
		measurements[i] = (time_end - time_begin);
	}
	
	mean = calc_mean(measurements) / 2.5;
	stddev = calc_stddev(measurements, mean) / 2.5;
	print("%d, %f, %f\n", 8, mean, stddev);

	exits(nil);
}	
