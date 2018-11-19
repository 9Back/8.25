#include <u.h>
#include <libc.h>


#define ARRAY_SIZE 800000
#define TRIALS_SIZE 250

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

    uvlong time_tot = 0.0;
    uvlong time_begin, time_end;
    
    double timings[TRIALS_SIZE];
    
    double trials_comp[TRIALS_SIZE];

	for(int i=0;i< TRIALS_SIZE;i++)
	{
		trials_comp[i] = comparison();
	}
    
    double mean_comp = calc_mean(trials_comp);
    double stddev_comp = calc_stddev(trials_comp, mean_comp);
    
    print("Measuring only comparison in cycles\n");
	print("mean: %f\t stddev: %f\n", mean_comp, stddev_comp);
    
   
    int *p = (int*) malloc(ARRAY_SIZE*sizeof(int));
    
    //priming cache
    for (int j=0; j < 1000; j++)
    {
	    for(int i = ARRAY_SIZE -1; i >= 0 ;i--)
	    {
			p[i] = 0;
	    }
    }
    
    for(int i=0;i< TRIALS_SIZE;i++)
    {
	    cycles(&time_begin);
	    memset((void*) p,0,4*ARRAY_SIZE-1);
	    cycles(&time_end);
	    time_tot = (time_end - time_begin);
	    timings[i] = (double) time_tot;
	}
    
    double mean = calc_mean(timings);
    double stddev = calc_stddev(timings,mean);
    
    print("Measuring setting(writing) memory\n");
	print("mean: %f\t stddev: %f\n", mean, stddev);
	print("array size %d\n", ARRAY_SIZE);
	
	for(int i=0;i< TRIALS_SIZE;i++)
    {
	    cycles(&time_begin);
	    memchr((void*) p,1,4*ARRAY_SIZE);
	    cycles(&time_end);
	    time_tot = (time_end - time_begin);
	    timings[i] = (double) time_tot - ARRAY_SIZE*mean_comp;
	}
	
	mean = calc_mean(timings);
    stddev = calc_stddev(timings,mean);
    
    print("Measuring comparing memory\n");
	print("mean: %f\t stddev: %f\n", mean, stddev);
	print("array size %d\n", ARRAY_SIZE);
	
	print("cycles per bytes %f\n", mean/(4*ARRAY_SIZE));
	
	exits(nil);
}
