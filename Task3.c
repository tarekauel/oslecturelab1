#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int my_value = 42;

int parent = 1;

void check(int error, char* command) {
    if (error < 0) {
        fprintf(stderr, "Error %d (parent: %d) at %s, errno: %d", error, parent, command, errno);
        exit(error);
    }
}

int main() {

    int fd[2];
    pid_t pid;

    char* string = malloc(100);

    check(pipe(fd), "pipe");

    pid = fork();
    check(pid, "fork");
    usleep(150);

    if (pid == 0) {
        parent = 0;
        close(fd[1]);
        my_value = 18951;
        fprintf(stderr, "I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(2000);
        read(fd[0], string, sizeof(*string));
        fprintf(stderr, "!: %s", string);
    } else {
        close(fd[0]);
        sprintf(string, "Hi, I am your parent. My PID=%d and my_value=%d\n", getpid(), my_value);
        write(fd[1], string, sizeof(*string));
        wait(0);
        fprintf(stderr, "I waited for my child\n");
    }

    return 0;
}