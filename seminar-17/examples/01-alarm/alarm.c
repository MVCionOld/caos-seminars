#include <unistd.h>
#include <stdio.h>

int main() {
    alarm(5);
    sleep(10);
    printf("Unreachable code?");
    return 0;
}