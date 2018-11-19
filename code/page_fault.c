#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 4
#define PAGE_SIZE 4096

// 9 bytes * 1GB = 9GB, greater than 8GB ram
struct {
    uchar things[9];
} thing;

double time_fault() {

    ulong size = (1 << 30) * sizeof(struct thing);
    uchar *mem = (uchar*)malloc(size));
	print("sizeof ulong: %d\n", sizeof(ulong));

    int fd = open("file.bin", OREAD);
    read(fd, mem, size);
    close(fd);

    uchar data;
    uchar i = ((ulong)(mem) + PAGE_SIZE) % PAGE_SIZE;
    uchar *aligned_i = (ulong*)i;

    uvlong time_total, time_begin, time_end;
    ulong iterations = 0;
    for (; aligned_i < (uchar)(mem) + size; aligned_i + PAGE_SIZE) {
        cycles(&time_begin);
        data = *aligned_i;
        cycles(&time_end);
        ++iterations;
        time_total += (time_end - time_begin);
    }

    double mean = (double)(time_total) / (double)(iterations);
    free(mem);
    
    return mean;
}

double calc_mean(vlong trials[TRIALS_SIZE]) {
	double mean = 0.0;

	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
		mean += trials[i];
    }
    mean = mean / TRIALS_SIZE;

    return mean;
}

double calc_stddev(vlong trials[TRIALS_SIZE], double mean) {
	double stddev = 0.0;

	ulong i;
	for (i = 0; i < TRIALS_SIZE; i++) {
		double diff = trials[i] - mean;	
		stddev = stddev + (diff * diff);
	}
    stddev = stddev / TRIALS_SIZE;
    stddev = sqrt(stddev);

    return stddev;
}

void main(int argc, char *argv[]) {
	// Contains trial timings in nanoseconds
    vlong trials[TRIALS_SIZE];
    
	ulong i;
    for (i = 0; i < TRIALS_SIZE; i++) {
         trials[i] = time_fault();
	} 

    double mean = calc_mean(trials);
    double stddev = calc_stddev(trials, mean);

	print("(cycles) mean: %f\t stddev: %f\n", mean, stddev);

	exits(nil);
}
