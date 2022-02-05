#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

static const char message[] = "Got Ctrl+C\n";

static void handler(int signum) {
    write(STDOUT_FILENO, message, sizeof(message)-1);
}

int main() {
    sigaction(
        SIGINT,
        &(struct sigaction){
            .sa_handler=handler,
            .sa_flags=SA_RESTART
        },
        NULL
    );

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    while (true) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        sleep(10);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
    }

    return 0;
}