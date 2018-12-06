#include <u.h>
#include <libc.h>
#include <stdio.h>

#define INNER_TRIALS 4
#define TRIALS 128

#define SIZE 20  // file sizes of 2^SIZE bytes
// Number of concurrent procs doing accesses to different files 
#define NUM_PROCS 64    
#define MAX_FILENAME_SIZE 64
#define READ_SIZE (1 << 14) // have the contention processes read 4 blocks at a time

// TODO assume 4K block size
#define BLOCK_SIZE (1 << 12)

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

int do_contention(int proc_num) {
    int pid = fork();
    if (!pid) {
        // in child, continuously read from file until receive interrupt
        char* data = malloc(READ_SIZE * sizeof(char));

        char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
        snprintf(filename, MAX_FILENAME_SIZE, "%d.contention.bin", proc_num);

        FILE* fp = fopen(filename, "r");
        setvbuf(fp, 0, _IONBF, 0);
        int iterations = 0;
        for (;; iterations++) {
            if (iterations >= ((1 << SIZE) / READ_SIZE)) {
                // reset back to the beginning of the file when we read over
                fseek(fp, 0, 0);
                iterations = 0;
            }
            fread(data, sizeof(char), READ_SIZE, fp);
        }
        fclose(fp);
        free(filename);
        free(data);

        return 0;
    } else {
        return pid;
    }
}

double do_trial(void) {
    int rand_file = rand() % NUM_PROCS;
    char* filename = malloc(MAX_FILENAME_SIZE * sizeof(char));
    snprintf(filename, MAX_FILENAME_SIZE, "%d.contention.bin", rand_file);

    FILE* fp = fopen(filename, "r");

	uvlong time_s = 0;
	uvlong time_e = 0;

    // buffer and bufsize params are not needed with _IONBF.
    // try to set the file in no buffering mode
    setvbuf(fp, 0, _IONBF, 0);

    int read_size = BLOCK_SIZE;
	char* data = malloc(read_size * sizeof(char));
    int num_steps = (1 << SIZE) / read_size;

    double tot_cycles = 0.0;
    for (int i = 0; i < INNER_TRIALS; i++) {
        for (int j = 0; j < num_steps; j++) {
            cycles(&time_s);
            fread(data, sizeof(char), read_size, fp);
            cycles(&time_e);
            tot_cycles += time_e - time_s; 
        }
        // reset back to the beginning of the file to read sequentially thru
        fseek(fp, 0, 0);
    }
    
    free(data);
    fclose(fp);
    free(filename);

    return (double)(tot_cycles) / (INNER_TRIALS * num_steps);
}

void main(int argc, char *argv[]) {
    int pids[NUM_PROCS];
	double timings[NUM_PROCS][TRIALS];

    for (int i = 0; i < NUM_PROCS; i++) {
        for (int j = 0; j < TRIALS; j++) { 
            timings[i][j] = do_trial();
        }
        // spawn off yet another process
        int pid = do_contention(i);
        pids[i] = pid;
    }

    // stop all child processes from doing the reads
    postnote(PNGROUP, getpid(), "interrupt");

    print("num procs\tmean (cycles)\tstddev (cycles)\n");
    double means[NUM_PROCS];
    double std_devs[NUM_PROCS];
    for (int i = 0; i < NUM_PROCS; i++) {
        means[i] = calc_mean(timings[i]);
        std_devs[i] = calc_stddev(timings[i], means[i]);
        print("%d\t%f\t%f\n", i, means[i], std_devs[i]);
    }

	exits(nil);

}
