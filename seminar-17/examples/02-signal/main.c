#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/**
 *
 * Don't try this at home school or anywhere! Don't try this at home school or anywhere!
 * Don't try this at home school or anywhere! Don't try this at home school or anywhere!
 *
 *       GGGGG     OOOOO   V     V   NN   N    OOOOO   K   K     OOOOO    DDDDD
 *      G     G   O     O  V     V   NN   N   O     O  K  K     O     O   D    D
 *      G         O     O   V   V    N N  N   O     O  K K      O     O   D     D
 *      G  GGGG   O     O   V   V    N  N N   O     O  K  K     O     O   D     D
 *      G     G   O     O    V V     N   NN   O     O  K   K    O     O   D    D
 *       GGGGG     OOOOO      V      N    N    OOOOO   K    K    OOOOO    DDDDD
 *
 * Don't try this at home school or anywhere! Don't try this at home school or anywhere!
 * Don't try this at home school or anywhere! Don't try this at home school or anywhere!
 *
 */

void handler_sigusr1(int signum) {
    printf("Reached SIGUSR1 handler, signum=%d\n", signum);
}

void handler_sigusr2(int signum) {
    printf("Reached SIGUSR2 handler, signum=%d\n", signum);
}

int main() {
    signal(SIGUSR1, handler_sigusr1);
    signal(SIGUSR2, handler_sigusr2);

    while (true) {
        pause();
    }

    return 0;
}