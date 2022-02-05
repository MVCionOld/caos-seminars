#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static const char sigusr1_fmt[] = "Reached SIGUSR1 handler\n";
static const char sigusr2_fmt[] = "Reached SIGUSR2 handler\n";

void handler_sigusr1(int signum) {
    // use async-signal-safety functions
    // man 7 signal-safety
    // https://man7.org/linux/man-pages/man7/signal-safety.7.html
    write(STDOUT_FILENO, sigusr1_fmt, sizeof(sigusr1_fmt) - 1);
}

void handler_sigusr2(int signum) {
    write(STDOUT_FILENO, sigusr2_fmt, sizeof(sigusr1_fmt) - 1);
}

int main() {

    sigaction(
        SIGUSR1,
        /*
          лаконичный способ использования структур,
          но не совместим со С++,
          доступен с 11го стандарта
        */
        &(struct sigaction){
          .sa_handler = handler_sigusr1, // можно писать SIG_IGN или SIG_DFL
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

    while (true) {
        pause();
    }

    return 0;
}