#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <unistd.h>

#define identifier "client5"
#define directory "dir1"

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

    
    char read_content[2048];

    
    int fw;

    fw = open("dir1/client5_RD",O_RDONLY);

    read(fw,read_content,2048);

    printf("test receive passed\n");

    close(fw);


    return 0;



}
