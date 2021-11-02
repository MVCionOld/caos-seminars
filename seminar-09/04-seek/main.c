#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

/*
 * Берем второй символ в файле, преобразовываем к цифре и инкрементим по модулю 10
 */

int main(int argc, char *argv[]) {
    assert(argc >= 2);

    // O_RDWR - открытие файла на чтение и запись одновременно
    int fd = open(argv[1],
                  O_RDWR | O_CREAT,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH // 0664
    );
    if (-1 == fd) {
        goto terminate;
    }

    // Перемещаемся на конец файла, получаем позицию конца файла - это размер файла
    int size = lseek(fd, 0, SEEK_END);
    printf("File size: %d\n", size);

    // если размер меньше 2, то дописываем цифры
    if (size < 2) {
        const char s[] = "10";
        lseek(fd, 0, SEEK_SET);
        write(fd, s, sizeof(s) - 1);
        printf("Written bytes: %d\n", (int)sizeof(s) - 1);
        size = lseek(fd, 0, SEEK_END);
        printf("File size: %d\n", size);
    }

    // читаем символ со 2й позиции
    lseek(fd, 1, SEEK_SET);
    char c;
    if (0 > (read(fd, &c, 1))) {
        perror("Cannot read 2nd byte");
        goto cleanup;
    }
    c = !isdigit(c) ? '0' : ((c - '0') + 1) % 10 + '0';

    // записываем символ в 2ю позицию
    lseek(fd, 1, SEEK_SET);
    write(fd, &c, 1);

cleanup:
    close(fd);
terminate:
    return 0;
}