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
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigset_t mask_without_usrs;
    sigfillset(&mask_without_usrs);
    sigdelset(&mask_without_usrs, SIGUSR1);
    sigdelset(&mask_without_usrs, SIGUSR2);

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

    // вот теперь хорошо
    while (true) {
        sigsuspend(&mask_without_usrs);
        if (sigusr1_flg) {
            sigusr1_flg = 0;
            printf("Reached SIGUSR1 handler, signum=%d\n", SIGUSR1);
        }
        if (sigusr2_flg) {
            sigusr2_flg = 0;
            printf("Reached SIGUSR2 handler, signum=%d\n", SIGUSR2);
        }
    }

    return 0;
}