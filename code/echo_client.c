#include <u.h>
#include <libc.h>

void main(int argc, char *argv[])
{
    char buf[256] = {"a"};
    int n;

    int fd = dial("tcp!10.0.2.15!7",0,0,0);

    if(fd == -1)
    {
        print("failed\n");
        exits(nil);
    }

    write(fd,buf,sizeof(buf));
    while((n = read(fd,buf,sizeof(buf)))> 0)
    {
        print("%s\n", buf);
    }

    print("reading from buffer\n");
    exits(nil);
}
