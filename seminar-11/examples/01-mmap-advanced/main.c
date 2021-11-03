#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Этот код не соответствует никаким пределам добра и зла.
 * Написан исключительно в исследовательских целях, никогда
 * так не делайте.
 */

int main() {
    int fd = open("sum.o", O_RDWR);
    struct stat s;
    fstat(fd, &s);

    printf("file size = %d\n", (int)s.st_size);

    void* mapped = mmap(NULL, s.st_size, PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, 0);
    close(fd);

    int (*sum)(int, int) = (void*)((char*)mapped + 0x40); // надо подобрать оффсет с какого начинается функция

    printf("sum(1, 1)     = %d\n", sum(1, 1));
    printf("sum(10, 100)  = %d\n", sum(10, 100));
    printf("sum(40, 5000) = %d\n", sum(40, 5000));

    munmap(mapped, s.st_size);
    return 0;
}