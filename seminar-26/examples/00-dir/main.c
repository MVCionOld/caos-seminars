#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Пример программы, которая считает размер директории,
 * учитывая древовидную структуру с помощью POSIX API
 */

extern uint64_t
calc_tree_size (const char *dirname);

int
main (int argc, char **argv) {
    printf("%"PRIu64"\n", calc_tree_size(argv[1]));
    return 0;
}

extern void
calc_total_size (const char* dirname, uint64_t* total_size) {
    DIR *dir = opendir(dirname);
    chdir(dirname);

    struct stat *file_stat = (struct stat *) malloc(sizeof(struct stat));
    uint64_t dir_files_size = 0LL;
    struct dirent *entry = NULL;

    while ((entry = readdir(dir))) {
        if (lstat(entry->d_name, file_stat) != -1) {
            if (S_ISREG(file_stat->st_mode)) {
                dir_files_size += file_stat->st_size;
            } else if (S_ISDIR(file_stat->st_mode)) {
                if (!(entry->d_name[0] == '.' &&
                      (entry->d_name[1] == '.' || entry->d_name[1] == '\0'))) {
                    calc_total_size(entry->d_name, &dir_files_size);
                    chdir("..");
                }
            }
        }
    }

    (*total_size) += dir_files_size;
    free(file_stat);
    closedir(dir);
}

extern uint64_t
calc_tree_size (const char *dirname) {
    uint64_t total_size = 0LL;
    calc_total_size(dirname, &total_size);
    return total_size;
}