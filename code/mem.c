#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 64
#define ARRAY_SIZE 7500000
#define ACCESS_COUNT 1000


double comparison(void) {

    uvlong time_tot = 0.0;
    uvlong time_begin, time_end;

	ulong i;
	// start trying to read time
    for (i = 0; i < 16384; i++) 
    {
        cycles(&time_begin);
        if(0==0)
		{
		}
        cycles(&time_end);
        time_tot = time_tot + (time_end - time_begin);
	}

    return time_tot / (16384.0);
}

double calc_mean(vlong trials[TRIALS_SIZE]) 
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

double calc_stddev(vlong trials[TRIALS_SIZE], double mean) 
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

    uvlong time_tot = 0.0;
    uvlong time_begin, time_end;

    trials_comp = comparison();
    
    double mean_comp = calc_mean(trials_comp);
    double stddev_comp = calc_stddev(trials_comp, mean_comp);
    
    print("Measuring only comparison in cycles\n");
	print("mean: %f\t stddev: %f\n", mean_comp, stddev_comp);

    int iarray[ARRAY_SIZE];
    double timing[ARRAY_SIZE/ACCES_COUNT];

    for(long int m=1;m < ARRAY_SIZE/ACCESS_COUNT;m=++)
    {	time_tot = 0.0;
	    for(int i = 0; i < ACCESS_COUNT ;i++)
		{
			cycles(&time_begin);
			if(iarray[i*m]==0)
			{
			}
			cycles(&time_end);
			time_tot = time_tot + (time_end - time_begin) - mean_comp;
	    }
	    timing[m] = time_tot/ACCESS_COUNT;
	    
    }
    
	print("mean: %f\t stddev: %f\n", mean_12, stddev_12);
	for(long int m=1; m < ARRAY_SIZE/ACCESS_COUNT;m=m+100)
	{
		print("avrg. time: %f\n", timing[m]);
	}
	

	exits(nil);
}
