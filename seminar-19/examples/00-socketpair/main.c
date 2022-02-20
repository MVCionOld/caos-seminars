#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef enum {
  DECREMENTER,
  INCREMENTER
} WorkerType;

typedef enum {
  DEC = -5,
  INC = 2
} WorkerAction;

void parent_wait(pid_t pid) {
    int status;
    pid_t waitpid_status = waitpid(pid, &status, 0);
    if (waitpid_status == -1) {
        fprintf(stderr, "waitpid error on pid: %d\n", pid);
        fflush(stderr);
    }
    if (WIFEXITED(status)) {
        printf("Child exited with code %d\n", WEXITSTATUS(status));
        fflush(stdout);
    } else if (WIFSIGNALED(status)) {
        printf("Child exited terminated with signal %d\n", WTERMSIG(status));
        fflush(stdout);
    }
}

void worker_exit(int fd) {
    shutdown(fd, SHUT_RDWR);  // уведомляем вторую сторону
    close(fd);
    printf("Process(pid=%d) is exiting...\n", getpid());
    fflush(stdout);
    exit(0);
}

int worker_routine(int socket_vector[], WorkerType worker_type, int seed) {
    signal(SIGPIPE, SIG_IGN);

    int action;
    int fd;
    if (worker_type == DECREMENTER) {
        fd = socket_vector[1];
        action = DEC;
        close(socket_vector[0]);
    } else {
        fd = socket_vector[0];
        action = INC;
        close(socket_vector[1]);
        write(fd, &seed, sizeof(seed));
    }

    while (true) {
        int payload;
        read(fd, &payload, sizeof(payload));
        printf("Process(pid=%d) received: %d\n", getpid(), payload);
        fflush(stdout);
        if (payload < 0) {
            worker_exit(fd);
        }

        payload += action;
        write(fd, &payload, sizeof(payload));
        if (payload < 0) {
            worker_exit(fd);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    int seed = (int) strtol(argv[1], NULL, 10);

    int socket_vector[2];
    int create_socketpair = socketpair(
        /* domain = */ AF_UNIX, // сокет внутри системы, адрес в данном случае - адрес файла сокета в файловой системе
        /* type = */ SOCK_STREAM, // для Unix-сокетов можно использовать также SOCK_DGRAM
        /* protocol = */ 0,
        /* socket_vector = */ socket_vector
    );
    if (create_socketpair != 0) {
        fprintf(stderr, "socketpair error");
        fflush(stderr);
        goto exit;
    }

    pid_t incrementer;
    if ((incrementer = fork()) == 0) {
        worker_routine(socket_vector, INCREMENTER, seed);
    } else if (incrementer < 0) {
        fprintf(stderr, "fork error on incrementer");
        fflush(stderr);
        goto close_socketpair;
    }

    pid_t decrementer;
    if ((decrementer = fork()) == 0) {
        worker_routine(socket_vector, DECREMENTER, seed);
    } else if (decrementer < 0) {
        fprintf(stderr, "fork error on decrementer\n");
        fflush(stderr);
        goto wait_incrementer;
    }

    parent_wait(decrementer);
wait_incrementer:
    parent_wait(incrementer);
close_socketpair:
    close(socket_vector[0]);
    close(socket_vector[1]);
exit:
    return 0;
}
