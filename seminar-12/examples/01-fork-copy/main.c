#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    printf("Hello, World!");

    pid_t pid;
    if (0 < (pid = fork())) {

        int status;
        pid_t wp_res = waitpid(pid, &status, 0);
        if (wp_res == -1) {
            perror("waitpid");
            exit(-1);
        }

        if (WIFEXITED(status)) {
            return (int) WEXITSTATUS(status); // если дочерний процесс завершился, прерываем родительский с тем же кодом
        } else {
            return -1;
        }

    } else if (0 == pid) {

        return 0;

    } else {
        perror("fork");
    }
}