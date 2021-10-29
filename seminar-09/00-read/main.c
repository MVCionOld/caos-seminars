#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

enum {
  BUFFER_SIZE = 4096
};

int main(int argc, char *argv[]) {

    int exit_code = 0;

    assert(argc >= 2);
    const char* file_name = argv[1];

    // Read from stdin section
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    if (-1 == (bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE))) {
        perror("Cannot read from stdin");
        return 1;
    }
    printf("From stdin (read bytes: %zd): %s\n", bytes_read, buffer); // buffer not zero-terminated string after `read`!

    // Open file by name section
    int fd;
    if (-1 == (fd = open(file_name, O_RDONLY))) { // O_RDWR also works
        perror("Cannot open file");
        return 1;
    }

    // Read from file section
    if (-1 == (bytes_read = read(fd, buffer, BUFFER_SIZE))) {
        perror("Cannot read from file");
        exit_code = 1;
        goto terminate;
    }
    printf("From file (read bytes: %zd): %s\n", bytes_read, buffer);

terminate:
    close(fd);
    return exit_code;
}