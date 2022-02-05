#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t sigusr1_flg = 0;
static volatile sig_atomic_t sigusr2_flg = 0;

void handler_sigusr1(int signum) {
    sigusr1_flg = 1;
}

void handler_sigusr2(int signum) {
    sigusr2_flg = 1;
}

int main() {

    sigaction(
        SIGUSR1,
        &(struct sigaction){
          .sa_handler = handler_sigusr1,
          .sa_flags = SA_RESTART
        },
        NULL
    );

    sigaction(
        SIGUSR2,
        &(struct sigaction){
            .sa_handler = handler_sigusr2,
            .sa_flags = SA_RESTART
        },
        NULL
    );

    // Более лаконичный подход, но не без изъяна.
    // Что здесь не так?
    while (true) {
        if (sigusr1_flg) {
            sigusr1_flg = 0;
            printf("Reached SIGUSR1 handler, signum=%d\n", SIGUSR1);
        }
        if (sigusr2_flg) {
            sigusr2_flg = 0;
            printf("Reached SIGUSR2 handler, signum=%d\n", SIGUSR2);
        }
        pause();
    }

    return 0;
}