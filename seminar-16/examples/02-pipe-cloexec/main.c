#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef union {
  int fds[2];
  struct {
    int in;
    int out;
  } pipe;
} pipe_t;

int main() {
    pipe_t p_pipe;
    pipe2(p_pipe.fds, O_CLOEXEC); // O_CLOEXEC - created file descriptors will be closed on exec call

    pid_t pid_1;
    pid_t pid_2;

    if ((pid_1 = fork()) == 0) {
        dup2(p_pipe.pipe.out, STDOUT_FILENO); // dup2 doesn't copy O_CLOEXEC attribute
        //dup3(p_pipe.pipe.out, STDOUT_FILENO, O_CLOEXEC); // can compare with this

        execlp("ps", "ps", "aux", NULL);
        exit(1);
    }
    if ((pid_2 = fork()) == 0) {
        // no close calls here
        dup2(p_pipe.pipe.in, STDIN_FILENO);

        execlp("tail", "tail", "-n", "4", NULL);
        exit(1);
    }

    close(p_pipe.pipe.in);
    close(p_pipe.pipe.out);

    int status;
    waitpid(pid_1, &status, 0);
    waitpid(pid_2, &status, 0);

    return 0;
}