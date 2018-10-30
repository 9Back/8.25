#include <u.h>
#include <libc.h>

vlong time_begin=0;
vlong time_end=0;
double time_diff=0;

#define LENGTH 1000

void main( int argc, char *argv[]) {

	double measurements[LENGTH];
	vlong time_begin=0;
	vlong time_end=0;
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
	
	time_begin = nsec();
	char data2;	
    read(fd[0],&data2,1);
    close(fd[0]);
    time_end = nsec();
    
    pipe_overhead = (time_end - time_begin);
    
	
	for(int i=0;i<LENGTH;i++)
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
		 	
		 	time_begin = nsec();
		 	wait();
		 	time_end = nsec();
		 	
		 	close(fd[1]);
		 	
		 	measurements[i] = (time_end - time_begin);
		 	measurements[i] = measurements[i] - pipe_overhead;
		 	measurements[i] = measurements[i]/2.0;
		 	
		 	print("context switch: %f\n", measurements[i]);
		 }
		 else
		 {
		 	char data2;	
		 	read(fd[0],&data2,1);
		 	close(fd[0]);
		 	exits(nil);
		 }
	 }
	 
	 double mean=0.0;
	 
	 for(int i=0;i<LENGTH;i++)
	 {
	 	mean = mean + measurements[i];
	 }
	 
	 mean = mean/LENGTH;
	 
	 print("context switch mu: %f\n", mean);
	 
	 double last_min = 1000000000.0;
	 for(int i=0;i<LENGTH;i++)
	 {
	 
	 	if(measurements[i] < last_min)
	 	{
	 		last_min = measurements[i];
	 	}
	 }
	 
	 print("minimum context switch: %f\n", last_min);
	 exits(nil);
}
