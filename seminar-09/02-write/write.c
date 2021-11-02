#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int write_retry(int fd, char* data, int size) {
    char* cdata = data;
    while (1) {
        size_t written_bytes = write(fd, cdata, size);
        if (written_bytes == 0) { // написали все данные
            return cdata - data;
        }
        if (written_bytes < 0) { // если возвращено значение < 0, то это ошибка
            if (errno == EAGAIN || errno == EINTR) { // она может быть retryable
                continue;
            } else { // а может быть критичной, и нет смысла пытаться повторить попытку чтения
                return -1;
            }
        }
        // если возвращенное значение > 0, значит успешно прочитано столько байт
        cdata += written_bytes;
        size -= written_bytes;
        if (size == 0) {
            return cdata - data;
        }
    }
}

static const char to_stdout[] = "Hello to stdout!\n";
static const char to_stderr[] = "Hello to stderr!\n";
static const char to_file[] = "Hello to file!\n";

int main(int argc, char *argv[]) {
    assert(argc >= 2);
    const char* file_name = argv[1];

    int bytes_written = write_retry(STDOUT_FILENO, (char*) to_stdout, strlen(to_stdout));
    assert(bytes_written == strlen(to_stdout));  // ошибки надо обрабатывать как в предыдущих
                                                 // примерах, а не через assert'ы

    bytes_written = write_retry(STDERR_FILENO, (char*) to_stderr, strlen(to_stderr));
    assert(bytes_written == strlen(to_stderr));

    int fd = open(file_name, O_WRONLY | O_CREAT, 0664); // 0644 это важно! Но об этом позже
                                                        // open(file_name, O_WRONLY | O_CREAT) ~UB~ printf("%s")
    assert(fd > 2);

    bytes_written = write_retry(fd, (char*) to_file, strlen(to_file));
    if (bytes_written < 0) {
        perror("Error write bytes to stdout");
    }

    close(fd);
    return 0;
}