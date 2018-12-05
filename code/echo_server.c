#include <u.h>
#include <libc.h>

int net_test(void)
{
    int dfd, acfd, lcfd;
    char adir[40], ldir[40];
    int n;
    char buf[256];

    acfd = announce("tcp!*!7", adir);
    //if(acfd < 0)
   //     return −1;
    for(;;){
        /* listen for a call */
        lcfd = listen(adir, ldir);
       // if(lcfd < 0)
        //    return −1;
        /* fork a process to echo */
        switch(fork()){
        //case −1:
        //    perror("forking");
        //    close(lcfd);
        //    break;
        case 0:
            /* accept the call and open the data file */
            dfd = accept(lcfd, ldir);
            //if(dfd < 0)
             //   return −1;
            /* echo until EOF */
            while((n = read(dfd, buf, sizeof(buf))) > 0)
                write(dfd, buf, n);
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
