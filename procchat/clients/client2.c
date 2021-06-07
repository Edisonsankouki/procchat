#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <unistd.h>

#define identifier "client2"
#define directory "dir2"

int main(){
    char total[2048];
    total[0] = 0;
    total[1] = 0;

    strcpy(total+2,identifier);
    strcpy(total+258,directory);

    int fd;
    fd = open("gevent",O_WRONLY);
    
    write(fd,total,2048);
    close(fd);

    return 0;



}
