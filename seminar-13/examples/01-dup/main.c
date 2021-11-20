#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
    pid_t pid;

    if ((pid = fork()) == 0) {
        int fd;
        if (0 > (fd = open("/tmp/out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644))) {
            perror("open");
            exit(-1);
        }
        dup2(fd, 1);
        close(fd);
        chdir("/tmp");
        execlp("bash", "bash", "-c", "pwd && ls -la", NULL);
        perror("exec");
        exit(-1);
    }
    else if (pid < 0) {
        perror("pid");
        exit(-1);
    }

    int status;
    struct rusage resource_usage;
    pid_t w = wait4(pid, &status, 0, &resource_usage); // обязательно нужно дождаться, пока завершится дочерний процесс
    if (w == -1) {
        perror("waitpid");
        exit(-1);
    }
    assert(WIFEXITED(status));
    printf("Child exited with code %d \n"
           "\tUser time %ld sec %ld usec\n"
           "\tSys time %ld sec %ld usec\n",
           WEXITSTATUS(status),
           resource_usage.ru_utime.tv_sec,
           resource_usage.ru_utime.tv_usec,
           resource_usage.ru_stime.tv_sec,
           resource_usage.ru_stime.tv_usec); // выводим код возврата дочернего процесса + еще полезную информацию

    return 0;
}