#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef union {
  int fds[2];
  struct {
    int out;
    int in;
  } pipe;
} pipe_t;

void close_pipe(pipe_t p_pipe) {
    close(p_pipe.pipe.in);
    close(p_pipe.pipe.out);
}

enum {
  BUFFER_SIZE = 1024
};

int main() {
    pipe_t p_pipe;
    pipe(p_pipe.fds);
    pid_t pid;

    if ((pid = fork()) == 0) {
        dup2(p_pipe.pipe.out, STDIN_FILENO);
        close_pipe(p_pipe);
        char buffer[16];
        read(STDIN_FILENO, buffer, sizeof(buffer));
        return 0;
    }

    dup2(p_pipe.pipe.in, STDOUT_FILENO);
    close_pipe(p_pipe);

    char* buffer = (char*) calloc(BUFFER_SIZE, 1);
    for (int batch = 0; batch < 128; batch++) {
        write(STDOUT_FILENO, buffer, BUFFER_SIZE);
    }
    free(buffer);

    int status;
    waitpid(pid, &status, 0);

    return 0;
}