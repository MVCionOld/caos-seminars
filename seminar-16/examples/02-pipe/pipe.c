#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd); // fd[0] - in, fd[1] - out (like stdin=0, stdout=1)
    pid_t pid_1;
    pid_t pid_2;

    if ((pid_1 = fork()) == 0) {
        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]); // notice: close file descriptors explicitly
        close(fd[1]); // try to comment out and compare behaviour

        execlp("ps", "ps", "aux", NULL);
        exit(1);
    }
    if ((pid_2 = fork()) == 0) {
        dup2(fd[0], STDIN_FILENO);

        close(fd[0]); // notice: close file descriptors explicitly
        close(fd[1]); // try to comment out and compare behaviour
        // ^^^ tail не завершиться пока открыт файловый дескриптор на запись в pipe (он будет ждать данных, которые он бы смог прочитать)

        execlp("tail", "tail", "-n", "4", NULL);
        exit(1);
    }

    close(fd[0]); // Тут закрыли pipe, потому что он нам больше не нужен (и потому что, если не закроем, то будет ошибка как с программой tail)
    close(fd[1]); // https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/fdup-pipe#%D0%BF%D1%80%D0%BE%D0%B1%D0%BB%D0%B5%D0%BC%D0%B0-dead-lock

    int status;
    waitpid(pid_1, &status, 0);
    waitpid(pid_2, &status, 0);

    return 0;
}