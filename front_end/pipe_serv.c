#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>


void channel_game(char ** pipes) {

    //mkfifo(pipes[0], 0666);
    //mkfifo(pipes[1], 0666);
    //int write_pipe = 0;
    int write_pipe = open(pipes[0], O_WRONLY);
    //write(write_pipe, "1\n", 2);
    int read_pipe = open(pipes[1], O_RDONLY);
    while( 1 ) {
        //write_pipe = open(pipes[0], O_WRONLY);
        char buff[256];
        int bytes = read(read_pipe, buff, 256);
        if(bytes <= 0)
            break;
        buff[bytes-1] = '\0';

        printf("Child Read %d bytes:\n%s", bytes, buff);
        strcat(buff, " acknowledged\n!");
        printf("Wrote: %s\n", buff);
        write(write_pipe, buff, strlen(buff));
        close(write_pipe);
    }
    close(write_pipe);
    close(read_pipe);
    remove(pipes[0]);
    remove(pipes[1]);
    exit(0);
}
int main(int argc, char ** argv) {
    remove("setup");
    /*
    int n = 0;
    char s1[10];
    char s2[10];
    char num[4];
    int ret = 1;
    while( ret != -1) {
        strcpy(s1, "cpipe_");
        strcpy(s2, "spipe_");
        memset(num, 0, 4);
        sprintf(num, "%d", n);
        strcat(s1, num);
        strcat(s2, num);

        remove(s1);
        ret = remove(s2);
        n++;
    }
   */ 
    mkfifo("setup", 0666);

    int setup_fd = open("setup", O_RDONLY);

    while(1) {
        char buff[256];
        int bytes = read(setup_fd, buff, 256);
        if(bytes <= 0)
            break;
        buff[bytes-1] = '\0';
        printf("Main: Read %d bytes:\n%s\n", bytes, buff);
        int pid = fork();
        //printf("%d\n", pid);
        if(pid == 0) {
            char ** new_pipes = malloc(2 * sizeof(char *));
            new_pipes[0] = malloc(16);
            new_pipes[1] = malloc(16);
            strcpy(new_pipes[0], "spipe_");
            strcat(new_pipes[0], buff);

            strcpy(new_pipes[1], "cpipe_");
            strcat(new_pipes[1], buff);

            //printf("%s\n%s\n", new_pipes[0], new_pipes[1]);
            channel_game(new_pipes);
        }
    }
}
