#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t last_signal = 0;

static void handler(int signum) {
    last_signal = signum;
}

int main() {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    int signals[] = {SIGUSR1, SIGINT, 0};
    for (int* signal = signals; *signal; ++signal) {
        sigaction(
            *signal,
            &(struct sigaction){
                .sa_handler=handler,
                .sa_flags=SA_RESTART,
                .sa_mask=mask
            },
            NULL
        );
    }
    sigemptyset(&mask);

    int parent_pid = getpid();
    int child_pid = fork();

    if (child_pid == 0) {
        while (true) {
            sigsuspend(&mask);

            if (last_signal) {
                if (last_signal == SIGUSR1) {
                    printf("Child process: Pong\n");
                    fflush(stdout);
                    kill(parent_pid, SIGUSR1);
                } else {
                    printf("Child process finish\n");
                    fflush(stdout);
                    return 0;
                }
                last_signal = 0;
            }
        }
    } else if (child_pid > 0) {
        for (int i = 0; i < 3; ++i) {
            printf("Parent process: Ping\n"); fflush(stdout);
            kill(child_pid, SIGUSR1);

            while (true) {
                sigsuspend(&mask);
                if (last_signal) {
                    last_signal = 0;
                    break;
                }
            }
        }

        printf("Parent process: Request child finish\n");
        fflush(stdout);
        kill(child_pid, SIGINT);

        int status;
        waitpid(child_pid, &status, 0);
    } else {
        exit(1);
    }

    return 0;
}
