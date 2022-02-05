#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    signal(SIGALRM, SIG_IGN);
    alarm(5);
    sleep(10);
    printf("Unreachable code?");
    return 0;
}