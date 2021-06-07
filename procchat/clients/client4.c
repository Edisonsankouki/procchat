#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <unistd.h>

#define identifier "client4"
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


    char say_content[2048];

    say_content[0] = 0;
    say_content[1] = 1;

    char* content = "test message 1\n";

    strcpy(say_content+2,content);

    int fw;

    fw = open("dir1/client4_WR",O_WRONLY);

    write(fd,say_content,2048);
    printf("test say passed\n");
    close(fw);


    return 0;



}
