#include <u.h>
#include <libc.h>

vlong time_begin=0;
vlong time_end=0;
double time_diff=0;

#define LENGTH 100

void main( int argc, char *argv[]) {

	double measurements[LENGTH];
	vlong time_begin=0;
	vlong time_end=0;
	double time_diff=0.0;
	double mean=0.0;
	double stddev=0.0;

	int fd1[2];

	if(pipe(fd1)==-1) 
    { 
        	fprintf(stderr, "Pipe Failed" ); 
        	exits(nil); 
    } 
	 int p = fork();
	 
	 //parent process
	 if(p > 0)
	 {
	 
	 	char data = "X";
	 	write(fd1[1],data,1);
	 	
	 	time_begin = nsec();
	 	wait(0);
	 	time_end = nsec();
	 	
	 	measurement[0] = (time_end - time_begin);
	 	
	 	print("empty function mean: %f\n", measurement[0]);
	 	exits(nil);

	 }
	 else
	 {
	 	char data2;
	 	
	 	read(fd1[0],data2,1);

	 	exits(nil);
	 }
}
