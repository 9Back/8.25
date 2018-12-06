#include <u.h>
#include <libc.h>

int net_test(void)
{
    int dfd, acfd, lcfd;
    char adir[40], ldir[40];
    int n;
    char buf[256];
    int tmp;

    acfd = announce("tcp!*!9876", adir);
    if(acfd < 0)
    {
        tmp = -1;
        return tmp;
    }
    for(;;){
        /* listen for a call */
        lcfd = listen(adir, ldir);
        if(lcfd < 0)
        {
           tmp = -1;
           return tmp;
        }
        /* fork a process to echo */
        switch(fork()){
        //case −1:
        //    perror("forking");
        //    close(lcfd);
        //    break;
        case 0:
            /* accept the call and open the data file */
            dfd = accept(lcfd, ldir);
            if(dfd < 0)
            {
               tmp = -1;
               return tmp;
            }

            /* echo until EOF */
            while((n = read(dfd, buf, sizeof(buf))) > 0)
            {
            }

            exits(0);
        default:
            close(lcfd);
            break;
        }
    }
}

void main(int argc, char *argv[])
{
    net_test();
    exits(nil);
}
