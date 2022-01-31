#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    close(STDOUT_FILENO);
    assert(STDOUT_FILENO == dup(fd));
    close(fd);
    printf("Copy of 'Hello world!'");
    return 0;
}