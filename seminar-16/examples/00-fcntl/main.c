#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    /*
     * хотим сделать копию-дескриптор >= 1,
     * поэтому тут нужно закрыть дескриптор 1,
     * чтобы он стал доступен
     */
    close(STDOUT_FILENO);
    int fd_copy = fcntl(fd, F_DUPFD, STDOUT_FILENO);
    assert(fd_copy == STDOUT_FILENO);

    close(fd);
    printf("Copy of 'Hello world!'");

    return 0;
}