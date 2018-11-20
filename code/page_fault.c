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
	double total = 0.0;
	int stride = ((1 << 10) * 4) + 1;
	char * gig1 = malloc((1 << 30));
	char * gig2 = malloc((1 << 30));
	char * gig3 = malloc((1 << 30));
	char * gig4 = malloc((1 << 30));
	char * gig5 = malloc((1 << 30));
	char * gig6 = malloc((1 << 30));
	char * gig7 = malloc((1 << 30));
	char * gig8 = malloc((1 << 30));
	char * gig9 = malloc((1 << 30));
	free(gig1);
	free(gig2);
	free(gig3);
	free(gig4);
	free(gig5);
	free(gig6);
	free(gig7);
	free(gig8);
	free(gig9);
	exits(nil);
}

