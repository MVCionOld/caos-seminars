#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        perror("Input path is not defined");
        goto terminate;
    }

    struct stat s;

    if (0 > lstat(argv[1], &s)) {
        perror("Cannot get stat on stdin");
        goto terminate;
    }
//    Cм. структуру struct stat на своей системе, в Linux:
//    printf("update time: %s", asctime(gmtime(&s.st_mtim.tv_sec)));
//    printf("access time: %s", asctime(gmtime(&s.st_atim.tv_sec)));

//    В BSD:
    printf("update time: %s", asctime(gmtime(&s.st_mtimespec.tv_sec)));
    printf("access time: %s", asctime(gmtime(&s.st_atimespec.tv_sec)));

//    printf("Last status change:       %s", ctime(&s.st_ctime));
//    printf("Last file access:         %s", ctime(&s.st_atime));
//    printf("Last file modification:   %s", ctime(&s.st_mtime));

terminate:
    return 0;
}