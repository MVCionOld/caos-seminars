#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

long get_page_size() {
    static long page_size = 0;   // avoid multiple system calls
    return page_size = page_size ?: sysconf(_SC_PAGE_SIZE);
}

int upper_round_to_page_size(int sz) {
    return (sz + get_page_size() - 1) / get_page_size() * get_page_size();
}

void produce(int64_t* buff, long len) {
    buff[0] = buff[1] = 1;
    for (long i = 2; i <= len; ++i) {
        buff[i] = buff[i - 1] + buff[i - 2];
    }
}

int main(int argc, char** argv) {
    int exit_code = 0;

    fprintf(stderr, "page size = %ld\n", get_page_size());

    if (argc < 2) {
        perror("Filename is not defined");
        exit_code = 1;
        goto ret;
    }
    if (argc < 3) {
        perror("Fibonacci sequence length is not defined");
        exit_code = 2;
        goto ret;
    }

    // про возможности конвертации char'ового массива в число
    // https://stackoverflow.com/questions/3420629/what-is-the-difference-between-sscanf-or-atoi-to-convert-a-string-to-an-integer
    long fib_last_inc = strtol(argv[2], NULL, 10);
    if (errno == EINVAL || errno == ERANGE) {
        perror("Cannot convert fibonacci sequence length to long type");
        exit_code = 3;
        goto ret;
    }

    int fd;
    if (0 > (fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0644))) {
        exit_code = 4;
        perror("Cannot open file");
        goto ret;
    }

    int outfile_sz = upper_round_to_page_size((fib_last_inc + 1) * sizeof(int64_t));
    fprintf(stderr, "desired file size = %d\n", outfile_sz);
    if (0 != ftruncate(fd, outfile_sz)) {
        exit_code = 5;
        perror("Cannot truncate file");
        goto close;
    }

    void* fibs = mmap(
        /* void *addr   = */ NULL,
        /* size_t len   = */ outfile_sz,
        /* int prot     = */ PROT_READ | PROT_WRITE,
        /* int flags    = */ MAP_SHARED,
        /* int fd       = */ fd,
        /* off_t offset = */ 0
    );
    if (fibs == MAP_FAILED) {
        exit_code = 6;
        perror("Cannot mmap file");
        goto close;
    }

    memset(fibs, 0, outfile_sz);
    produce((int64_t*) fibs, fib_last_inc);

    struct stat stat_;
    if (0 > fstat(fd, &stat_)) {
        exit_code = 7;
        perror("Cannot extract file's stat");
        goto munmap;
    }
    fprintf(stderr, "actual file size = %lld\n", stat_.st_size);

munmap:
    if (0 != munmap(fibs, outfile_sz)) {
        exit_code = 8;
        perror("Error during memory unmapping");
        goto close;
    }
close:
    if (0 != close(fd)) {  // never forget to close the file
        perror("Cannot close file");
    }
ret:
    return exit_code;
}