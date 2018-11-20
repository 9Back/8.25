#include <u.h>
#include <libc.h>

#define TRIALS_SIZE 4
// TODO change this according to the page size (run sysinfo)
#define PAGE_SIZE 4096
#define NUM_MEMS 32

struct thing {
    // TODO change this if need to allocate lotsa memory...   
    uchar things[1];
};

double time_fault(void) {
    // TODO change this size according to the physical RAM
    ulong size = (1 << 28) * sizeof(struct thing);
    uchar* mems[NUM_MEMS];
    for (int i = 0; i < NUM_MEMS; i++) {
        mems[i] = (uchar*)malloc(size);
    }
	print("sizeof ulong: %d\n", sizeof(ulong));
    print("allocated size %d, allocated %d of them.\n", size, NUM_MEMS);

    // TODO generate huge bin file larger than the size of physical memory 
    int fd = open("file.bin", OREAD);
    for (int i = 0; i < NUM_MEMS; i++) {
        read(fd, mems[i], size);
    }
    close(fd);

    ulong iterations = 0;
    uvlong time_total, time_begin, time_end;
    for (int i = 0; i < NUM_MEMS; i++) {
        uchar data;
        ulong i = (ulong)(mem[i]) + (PAGE_SIZE - ((ulong)(mem[i]) % PAGE_SIZE));
        uchar *aligned_i = (uchar*)i;

        print("aligned address: %d\n", (ulong)(aligned_i));
        print("iterating until: %d\n", (ulong)(mem[i]) + size);

        for (; (ulong)aligned_i < (ulong)(mem[i]) + size; aligned_i = aligned_i + (20 * PAGE_SIZE)) {
            cycles(&time_begin);
            data = *aligned_i;
            cycles(&time_end);
            ++iterations;
            time_total += (time_end - time_begin);
        }
    }

    double mean = (double)(time_total) / (double)(iterations);
    for (int i = 0; i < NUM_MEMS; i++) {
        free(mems[i]);
    }
    
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
