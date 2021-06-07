#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <unistd.h>

#define identifier "client3"
#define directory "dir3"

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

    sleep(15);

    char ping_message[2048];

    fd = open("dir3/client3_RD",O_RDONLY);

    read(fd,ping_message,2048);

    short* a = (short *)ping_message;

    short b = *a;

    char pong_message[2048];

    pong_message[0] = 0;

    pong_message[1] = 6;

    close(fd);
    

    if(b == 5){
        
        printf("stage 1 passed\n");
        fd = open("dir3/client3_WR",O_WRONLY);
        write(fd,pong_message,2048);
        close(fd);


    }

    
    

    return 0;


}
