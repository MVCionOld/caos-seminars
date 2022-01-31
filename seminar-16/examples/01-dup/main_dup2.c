#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    printf("Copy of 'Hello world!'");
    return 0;
}