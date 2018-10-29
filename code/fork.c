#include <u.h>
#include <libc.h>

#define LENGTH 20000

void (main int argc, char *argv[]) {

	double measurements[LENGTH];
	vlong time_begin=0;
	vlong time_end=0;
	double time_diff=0.0;
	double mean=0.0;
	double stddev=0.0;

	for(int i=0; i<LENGTH;i++)
	{
		/*RFPROC    If set a new process is created; otherwise changes
                            affect the current process.*/

		/*RFCNAMEG  If set, the new process starts with a clean name
                    space. A new name space must be built from a mount
                    of an open file descriptor.  Is mutually exclusive
                    with RFNAMEG.*/

		/*RFCENVG   If set, the new process starts with an empty envi-
                    ronment.  Is mutually exclusive with RFENVG.*/
          								
		time_begin = nsec();
		rfork(RFPROC | RFCNAMEG | RFCENVG);
		time_end = nsec();
	  	measurements[i] =  (time_end - time_begin) * 0.000001;
	  	mean = mean + measurements[i];
	}
		
	mean = mean/double(LENGTH);
	for(int i=0;i<LENGTH;i++)
	{
		stddev = stddev + (measurements[i] - mean)*(measurements[i] - mean);
	}
	
	stddev = stddev/LENGTH;
	stddev = sqrt(stddev);  
	
	print("fork without any shared stuff with parent mu: %f\n", mean);
	print("fork without any shared stuff with parent sigma: %f\n", stddev);
	
	
	stddev = 0.0;
	mean = 0.0;
	
	for(int i=0; i<LENGTH;i++)
	{          					
	
				
		time_begin = nsec();
		
		// Fork is just a call of rfork(RFFDG|RFREND|RFPROC)
		
		/* RFFDG     If set, the invoker's file descriptor table (see
                    intro(2)) is copied; otherwise the two processes
                    share a single table.*/
                    
        /* RFREND    If set, the process will be unable to
                    rendezvous(2) with any of its ancestors; its chil-
                    dren will, however, be able to rendezvous with it.
                    In effect, RFREND makes the process the first in a
                    group of processes that share a space for
                    rendezvous tags.*/
                    
        /* If RFPROC is set, the value returned in the parent process
          is the process id of the child process; the value returned
          in the child is zero.  Without RFPROC, the return value is
          zero.  Process ids range from 1 to the maximum integer (int)
          value.  Rfork will sleep, if necessary, until required pro-
          cess resources are available. */
        
		fork();
		time_end = nsec();
	  	measurements[i] =  (time_end - time_begin) * 0.000001;
	  	mean = mean + measurements[i];
	}
	
	mean = mean/double(LENGTH);
	for(int i=0;i<LENGTH;i++)
	{
		stddev = stddev + (measurements[i] - mean)*(measurements[i] - mean);
	}
	
	stddev = stddev/LENGTH;
	stddev = sqrt(stddev);  
	
	print("standard fork mu: %f\n", mean);
	print("standard fork sigma: %f\n", stddev);
	
	exits(nil);
}
