#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

enum {
  BATCH_SIZE = 1024
};

int main() {
    pipe_t p_pipe;
    pipe(p_pipe.fds); // fd[0] - in, fd[1] - out (like stdin=0, stdout=1)

    int batch_num = 1;
    bool not_fail = true;

    while (not_fail) {
        int buffer_size = BATCH_SIZE * batch_num;
        char* buffer = calloc(buffer_size, 1);

        int written = write(p_pipe.pipe.out, buffer, buffer_size);
        if (buffer_size != written) {
            printf("Fail with %dKb!\n", batch_num);
            not_fail = false;
        }
        if (!not_fail) {
            free(buffer);
            break;
        }

        int read_ = read(p_pipe.pipe.in, buffer, buffer_size);
        if (buffer_size != read_) {
            printf("Fail with %dKb!\n", batch_num);
            not_fail = false;
        }
        if (not_fail) {
            printf("Success with %dKb!\n", batch_num);
            batch_num++;
        }

        free(buffer);
    }

    close_pipe(p_pipe);
    return 0;
}