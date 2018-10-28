#include <u.h>
#include <libc.h>

vlong time_begin;
vlong time_end;
double time_diff;

void func1(void)
{

}


int func2(int value1, int value2)
{
	return 0;
}

int func3(int value1, int value2)
{
	int val = value1;
	int val2 = value2;
	return value;
}

void main(int argc, char *argv[]) {
	
	double measurements[64];
	double mean;
	double mean2;
	double mean3;

	double standarddev;

	//measureing empty function
	for(int i=0;i<64;i++)
	{
	   time_begin = nsec();
	   func1(void);
	   time_end = nsec();
	   measurements[i] =  (time_end - time_begin) * 0.000001;
	   mean = mean + measurments[i];
	}
	
	mean = mean/64;

	for(int i=0;i<64;i++)
	{
		standarddev = standarddev + (measurements[i] - mean)*(measurements[i] - mean);
	}
	
	standarddev = standarddev/64;
	standarddev = sqrt(standarddev);  

	print("empty function mean: %f\n", mean);
	print("empty function stddev: %f\n", standarddev);

	//measuring function call with input parameters
	for(int i=0;i<64;i++)
	{
	   time_begin = nsec();
	   func2(1000,1000);
	   time_end = nsec();
	   measurements[i] =  (time_end - time_begin) * 0.000001;
	   mean2 = mean2 + measurments[i];
	}
	
	mean2 = mean2/64;

	for(int i=0;i<64;i++)
	{
		standarddev = standarddev + (measurements[i] - mean2)*(measurements[i] - mean2);
	}
	
	standarddev = standarddev/64;
	standarddev = sqrt(standarddev);
	  

	print("function call with input parameters mean: %f\n", mean);
	print("function call with input parameters stddev: %f\n", standarddev);

	//measuring function call with input+return
	for(int i=0;i<64;i++)
	{
	   time_begin = nsec();
	   func2(1000,1000);
	   time_end = nsec();
	   measurements[i] =  (time_end - time_begin) * 0.000001;
	   mean3 = mean3 + measurments[i];
	}
	
	mean3 = mean3/64;

	for(int i=0;i<64;i++)
	{
		standarddev = standarddev + (measurements[i] - mean3)*(measurements[i] - mean3);
	}
	
	standarddev = standarddev/64;
	standarddev = sqrt(standarddev);

	print("function call with input+return mean: %f\n", mean3);
	print("function call with input+return stddev: %f\n", standarddev);


	double diff=mean2-mean;

	print("difference func and func2: %f\n", diff);

	diff = mean3- mean2;

	print("difference func2 and func2: %f\n", diff);

	exits(nil);
}



	
