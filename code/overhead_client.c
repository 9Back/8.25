#include <u.h>
#include <libc.h>

#define TRIALS 1000

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

void main(int argc, char *argv[])
{
    double measurments[TRIALS];
    double measurments_2[TRIALS];
    uvlong time_begin=0;
    uvlong time_end=0;
    double time_diff = 0.0;

    char buf[256];

    for(int i=0;i<256;i++)
    {
        buf[i] = 'a';
    }

    int n;
    int fd;

    //loopback RTT
    for(int i=0;i<TRIALS;i++)
    {
        //establish connection
        cycles(&time_begin);
        fd = dial("tcp!10.0.2.15!7",0,0,0);
        cycles(&time_end);

        if(fd == -1)
        {
            print("failed\n");
            exits(nil);
        }

        write(fd,buf,sizeof(buf));
        n = read(fd,buf,sizeof(buf));

        measurments[i] = time_end - time_begin;

        cycles(&time_begin);
        close(fd);
        cycles(&time_end);

        measurments_2[i] = time_end - time_begin;

        print("%s\n", buf);

        print("reading from buffer\n");
    }

    double mean = calc_mean(measurments);
    print("est. overhead time (mean) on loopback (nsec): %f\n", mean / 2.5);
    double std_dev = calc_stddev(measurments,mean);
    print("est. overhead time (std_dev) on loopback (nsec): %f\n", std_dev / 2.5);

    double mean_2 = calc_mean(measurments_2);
    print("tear down overhead time (mean) on loopback (nsec): %f\n", mean_2 / 2.5);
    double std_dev_2 = calc_stddev(measurments_2,mean_2);
    print("tear down time (std_dev) on loopback (nsec): %f\n", std_dev_2 / 2.5);

    exits(nil);
}
