#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <unistd.h>
#include <errno.h>

#define MESSAGE_LENGTH 2048
int alive_num = 1;
int global_parent_id;
void kill_handler(int sig, siginfo_t *info, void *ctx)
{
    kill(info->si_pid, SIGKILL);
}
void report_to_global(int sig)
{
    kill(global_parent_id, SIGUSR1);
}
void client_handler(char *domain_name, char *identifier, int parent_id, char *write_pipe_name, char *read_pipe_name);

int main(int argc, char **argv)
{
    int fd;
    int parent_pid = getpid();
    global_parent_id = parent_pid;
    unlink("gevent");
    mkfifo("gevent", 0777);/*make the main gevent fifo for the client to connect */
    char content[MESSAGE_LENGTH];

    /*siganl handling*/

    struct sigaction dealer;
    dealer.sa_sigaction = kill_handler;
    sigemptyset(&dealer.sa_mask);
    dealer.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &dealer, NULL);
    signal(SIGCHLD, SIG_IGN);
    
    while (1)
    {
        /*read in a message by a client*/
        fd = open("gevent", O_RDONLY);
        
        if (fd == -1)
        {
           continue;
        }

        read(fd, &content, MESSAGE_LENGTH);

        short *num = (short *)content;

        if ((*num) == CONNECT)
        { //Handle conncect
            char *identifier = content + 2;
            char *domain_name = content + 258;
            if (access(domain_name, F_OK) == -1)
            {
                int i = mkdir(domain_name, 0765);
                if (i == -1)
                {
                    printf("error detected when initialize the directory\n");
                    return 1;
                }
            }

            char read_pipe_name[515];
            read_pipe_name[0] = '\0';
            char write_pipe_name[515];
            write_pipe_name[0] = '\0';

            strcat(read_pipe_name, domain_name);
            strcat(read_pipe_name, "/");
            strcat(read_pipe_name, identifier);
            strcat(read_pipe_name, "_RD");

            strcat(write_pipe_name, domain_name);
            strcat(write_pipe_name, "/");
            strcat(write_pipe_name, identifier);
            strcat(write_pipe_name, "_WR");

            mkfifo(read_pipe_name, 0777);
            mkfifo(write_pipe_name, 0777);

            switch (fork())
            {
            case -1:
                printf("fork failed\n");
                break;
            case 0:
                // allocate the handler for the program
                client_handler(domain_name, identifier, parent_pid, write_pipe_name, read_pipe_name);
            }
        }
    }

    return 0;
}

void client_handler(char *domain_name, char *identifier, int parent_id, char *write_pipe_name, char *read_pipe_name)
{
    signal(SIGUSR1, report_to_global);//send SIGUSR1 to global
    int hid = getpid();
    int handler_pid = fork();
    
    switch (handler_pid)
    {
    case 0:
    {   //PING sender and detect zombie
        sleep(15);
        while (1)
        {

            int fd = open(read_pipe_name, O_WRONLY | O_NONBLOCK);
            char new_message[MESSAGE_LENGTH];
            short *num_x = (short *)new_message;
            (*num_x) = PING;
            write(fd, new_message, MESSAGE_LENGTH);
            close(fd);
            alive_num = 0;
            sleep(15);
            if (alive_num != 1)
            {
                unlink(read_pipe_name);
                unlink(write_pipe_name);
                kill(hid, SIGUSR1);
                exit(1);
            }
        }
    }
    case -1:
    {
        /*printf("client handler fork failed\n");*/
        break;
    }
    default:
    {

        while (1)
        {
            //client handler main loop

            int fd = open(write_pipe_name, O_RDONLY);

            if(fd==-1){
                printf("No such pipe\n");
                continue;
            }

            char message[MESSAGE_LENGTH];

            read(fd, message, MESSAGE_LENGTH);

            short *num = (short *)message;

            switch ((*num))
            {
            case SAY:
            {   //handle say
                char *words = message + 2;
                char new_message[MESSAGE_LENGTH];
                short *num = (short *)new_message;
                (*num) = RECEIVE;
                FILE *fp;
                char command[550] = "ls ";
                strcat(command, domain_name);
                strcat(command, "/*_RD -p");
                fp = popen(command, "r");
                while (fp == NULL)
                {
                    fp = popen(command, "r");
                }
                char client_RD[MESSAGE_LENGTH];
                while (fgets(client_RD, MESSAGE_LENGTH, fp) != NULL)
                {
                    client_RD[strlen(client_RD) - 1] = '\0';
                    if (strcmp(read_pipe_name, client_RD) == 0)
                    {
                        continue;
                    }
                    new_message[2] = '\0';
                    strcpy(new_message + 2, identifier);

                    strcpy(new_message + 258, words);
                    int fw = open(client_RD, O_WRONLY);

                    write(fw, new_message, MESSAGE_LENGTH);
                    close(fw);
                }
                fclose(fp);
                close(fd);
                break;
            }

            case SAYCONT:
            {   //handle saycont
                char *words_1 = message + 2;
                char new_message_1[MESSAGE_LENGTH];
                short *num_1 = (short *)new_message_1;
                (*num_1) = RECVCONT;
                new_message_1[2047] = message[2047];
                FILE *fp_1;
                char command_1[550] = "ls ";
                strcat(command_1, domain_name);
                strcat(command_1, "/*_RD -p");
                fp_1 = popen(command_1, "r");
                char client_RD_say_content[MESSAGE_LENGTH];
                while (fgets(client_RD_say_content, MESSAGE_LENGTH, fp_1) != NULL)
                {
                    client_RD_say_content[strlen(client_RD_say_content) - 1] = '\0';
                    if (strcmp(read_pipe_name, client_RD_say_content) == 0)
                    {
                        continue;
                    }
                    strcpy(new_message_1 + 2, identifier);
                    strcpy(new_message_1 + 258, words_1);
                    int fw = open(client_RD_say_content, O_WRONLY);
                    write(fw, new_message_1, MESSAGE_LENGTH);
                    close(fw);
                }
                fclose(fp_1);
                close(fd);
                break;
            }

            case PONG:
            {   //if pong detected, the process is still alive.
                alive_num = 1;
                break;
            }
            case DISCONNECT:
            {   //handle disconnect
                unlink(read_pipe_name);
                unlink(write_pipe_name);
                kill(parent_id, SIGUSR1);
            }
            }
        }
    }
    }
}