#define _GNU_SOURCE
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    sigset_t full_mask;
    sigfillset(&full_mask);
    sigprocmask(SIG_BLOCK, &full_mask, NULL);

    int parent_pid = getpid();

    int child_pid = fork();
    if (child_pid == 0) {
        while (true) {
            siginfo_t info;
            sigwaitinfo(&full_mask, &info); // вместо sigsuspend и обработчика

            int received_signal = info.si_signo;
            int received_value = info.si_value.sival_int;

            if (received_signal == SIGUSR1) {
                printf("Child process: Pong (get %d, send %d)\n", received_value, received_value * 2);
                fflush(stdout);
                sigqueue(
                    parent_pid,
                    SIGRTMAX,
                    (union sigval) {  // вместе с сигналом передаем число
                        .sival_int = received_value * 2
                    }
                );
            } else {
                printf("Child process finish\n");
                fflush(stdout);
                return 0;
            }
        }
    } else {
        int child_response = 100;
        for (int i = 0; i < 3; ++i) {
            printf("Parent process: Ping (got %d, send %d)\n", child_response, child_response + 1);
            fflush(stdout);
            sigqueue(
                child_pid,
                SIGUSR1,
                (union sigval) {
                    .sival_int = child_response + 1
                }
            );

            siginfo_t info;
            sigwaitinfo(&full_mask, &info);
            child_response = info.si_value.sival_int;
        }

        printf("Parent process: Request child finish\n");
        fflush(stdout);
        kill(child_pid, SIGINT);

        int status;
        waitpid(child_pid, &status, 0);
    }
    return 0;
}