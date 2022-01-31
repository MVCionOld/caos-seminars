#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef union {
  int fds[2];
  struct {
    int in;
    int out;
  } pipe;
} pipe_t;

void close_pipe(pipe_t p_pipe) {
    close(p_pipe.pipe.in);
    close(p_pipe.pipe.out);
}

int main() {
    pipe_t p_pipe;
    pipe(p_pipe.fds); // fd[0] - in, fd[1] - out (like stdin=0, stdout=1)
    pid_t pid_1;
    pid_t pid_2;

    if ((pid_1 = fork()) == 0) {
        dup2(p_pipe.pipe.out, STDOUT_FILENO);
        close_pipe(p_pipe);

        execlp("ps", "ps", "aux", NULL);
        exit(1);
    }

    if ((pid_2 = fork()) == 0) {
        dup2(p_pipe.pipe.in, STDIN_FILENO);
        close_pipe(p_pipe);

        execlp("tail", "tail", "-n", "4", NULL);
        exit(1);
    }

    close_pipe(p_pipe);

    int status;
    waitpid(pid_1, &status, 0);
    waitpid(pid_2, &status, 0);

    return 0;
}