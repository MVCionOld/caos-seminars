#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

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

    printf("is regular: %s      \n", ((s.st_mode & S_IFMT) == S_IFREG) ? "yes" : "no "); // can use predefined mask
    printf("is directory: %s    \n", S_ISDIR(s.st_mode) ? "yes" : "no ");             // or predefined macro
    printf("is symbolic link: %s\n", S_ISLNK(s.st_mode) ? "yes" : "no ");

terminate:
    return 0;
}