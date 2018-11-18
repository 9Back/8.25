#include <u.h>
#include <libc.h>

#define LENGTH 20000

uvlong time_begin=0;
uvlong time_end=0;
double time_diff=0;

void func1(void)
{

}


int func2(int value1, int value2)
{
	return 0;
}

int func3(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8 )
{
	int val = value1;
	int val2 = value2;
	return val;
}

void main(int argc, char *argv[]) {
	
	for(int i=0;i<LENGTH;i++)
	{
	    func1();
	    func2(1,1);
	    func3(1,2,3,4,5,6,7,8);
	}
	
	double measurements[LENGTH];
	double mean=0;
	double mean2=0;
	double mean3=0;

	double standarddev=0;

	//measureing empty function
	for(int i=0;i<LENGTH;i++)
	{
       cycles(&time_begin);
	   func1();
       cycles(&time_end);
	   measurements[i] =  (time_end - time_begin);
	   mean = mean + measurements[i];
	}
	
	mean = mean/LENGTH;

	for(int i=0;i<LENGTH;i++)
	{
		standarddev = standarddev + (measurements[i] - mean)*(measurements[i] - mean);
	}
	
	standarddev = standarddev/LENGTH;
	standarddev = sqrt(standarddev);  

	print("empty function mean (cycles): %f\n", mean);
	print("empty function stddev (cycles): %f\n", standarddev);

	standarddev = 0.0;

	//measuring function call with input parameters
	for(int i=0;i<LENGTH;i++)
	{
       cycles(&time_begin);
	   func2(1000,1000);
       cycles(&time_end);
	   measurements[i] =  (time_end - time_begin);
	   mean2 = mean2 + measurements[i];
	}
	
	mean2 = mean2/LENGTH;

	for(int i=0;i<LENGTH;i++)
	{
		standarddev = standarddev + (measurements[i] - mean2)*(measurements[i] - mean2);
	}
	
	standarddev = standarddev/LENGTH;
	standarddev = sqrt(standarddev);
	  

	print("function call with input parameters mean (cycles): %f\n", mean2);
	print("function call with input parameters stddev (cycles): %f\n", standarddev);

	standarddev=0.0;

	//measuring function call with input+return
	for(int i=0;i<LENGTH;i++)
	{
       cycles(&time_begin);
	   func3(1000,1000,3,4,5,6,7,8);
       cycles(&time_end);
	   measurements[i] =  (time_end - time_begin);
	   mean3 = mean3 + measurements[i];
	}
	
	mean3 = mean3/LENGTH;

	for(int i=0;i<LENGTH;i++)
	{
	    standarddev = standarddev + (measurements[i] - mean3)*(measurements[i] - mean3);
	}
	
	standarddev = standarddev/LENGTH;
	standarddev = sqrt(standarddev);

	print("function call with input+return mean (cycles): %f\n", mean3);
	print("function call with input+return stddev (cycles): %f\n", standarddev);

	double diff=mean2-mean;

	print("difference func and func2 (cycles): %f\n", diff);

	diff = mean3- mean2;

	print("difference func2 and func2 (cycles): %f\n", diff);

	exits(nil);
}	
