#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char to_stdout[] = "Hello to stdout!\n";
    const char to_stderr[] = "Hello to stderr!\n";
    const char to_file[] = "Hello to file!\n";

    assert(argc >= 2);
    const char* file_name = argv[1];

    int bytes_written = write(STDOUT_FILENO, to_stdout, strlen(to_stdout));
    assert(bytes_written == strlen(to_stdout));

    bytes_written = write(STDERR_FILENO, to_stderr, strlen(to_stderr));
    assert(bytes_written == strlen(to_stderr));

    int fd = open(file_name, O_WRONLY | O_CREAT, 0644); // 0644 это важно! Но об этом позже
    // open(file_name, O_WRONLY | O_CREAT) ~ printf("%s")
    bytes_written = write(fd, to_file, strlen(to_file));
    close(fd);

    return 0;
}