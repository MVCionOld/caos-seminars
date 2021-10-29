#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

enum {
  BUFFER_SIZE = 4096
};

int read_retry(int fd, char* data, int size) {
    char* cdata = data;
    while (1) {
        size_t read_bytes = read(fd, cdata, size);
        if (read_bytes == 0) { // если read вернул 0, значит файловый дескриптор закрыт с другого конца
            // или конец файла
            return cdata - data;
        }
        if (read_bytes < 0) { // если возвращено значение < 0, то это ошибка
            if (errno == EAGAIN || errno == EINTR) { // она может быть retryable
                continue;
            } else { // а может быть критичной, и нет смысла пытаться повторить попытку чтения
                return -1;
            }
        }
        // если возвращенное значение > 0, значит успешно прочитано столько байт
        cdata += read_bytes;
        size -= read_bytes;
        if (size == 0) {
            return cdata - data;
        }
    }
}

int main(int argc, char *argv[]) {

    int exit_code = 0;

    assert(argc >= 2);
    const char* file_name = argv[1];

    // Read from stdin section
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    if (-1 == (bytes_read = read_retry(STDIN_FILENO, buffer, BUFFER_SIZE))) {
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
    if (-1 == (bytes_read = read_retry(fd, buffer, BUFFER_SIZE))) {
        perror("Cannot read from file");
        exit_code = 1;
        goto terminate;
    }
    printf("From file (read bytes: %zd): %s\n", bytes_read, buffer);

terminate:
    close(fd);
    return exit_code;
}