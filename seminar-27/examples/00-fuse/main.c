#define _GNU_SOURCE
#define FUSE_USE_VERSION 31
#include <errno.h>
#include <fuse.h>
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char* filename;
  char* filecontent;
  char* log;
} options_t;

options_t options;

void print_cwd();

/**
 * Cписок всех колбэков:
 * http://libfuse.github.io/doxygen/structfuse__operations.html
 */
/**
 * Самый важный колбэк.
 * Вызывается первым при любом другом колбэке.
 * Заполняет структуру stbuf.
 */
int getattr_callback(const char* path,
                     struct stat* stbuf,
                     struct fuse_file_info* fi);
int readdir_callback(const char* path, void* buf,
                     fuse_fill_dir_t filler,
                     off_t offset,
                     struct fuse_file_info* fi,
                     enum fuse_readdir_flags flags);
/**
 * Вызывается после успешной обработки open.
 */
int read_callback(const char* path,
                  char* buf,
                  size_t size,
                  off_t offset,
                  struct fuse_file_info* fi);

/**
 * Структура с колбэками.
 */
struct fuse_operations
fuse_defined_operations = {
    .getattr = getattr_callback,
    .read    = read_callback,
    .readdir = readdir_callback,
};

/**
 * https://libfuse.github.io/doxygen/structfuse__opt.html
 */
struct fuse_opt
opt_specs[] = {
    (struct fuse_opt){
        .templ = "--file-name=%s",
        .offset = offsetof(options_t, filename),
        .value = 1
    },
    (struct fuse_opt){
        .templ = "--file-content=%s",
        .offset = offsetof(options_t, filecontent),
        .value = 1
    },
    (struct fuse_opt){
        .templ = "--log=%s",
        .offset = offsetof(options_t, log),
        .value = 1
    },
    FUSE_OPT_END  /* Sentinel */
};

int main(int argc, char* argv[]) {
    /**
     * Для выделения, специфичных для конкретной файловой системы,
     * опций используется модифицируемый список опций fuse_args,
     * который инициализируется макросом FUSE_ARGS_INIT(argc, argv)
     *
     * P.S.: флаг "-f" - синхронный режим
     *      (программа работает, пока `fusermount -u 00-fuse` не будет сделан)
     */
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    /*
    * Заполняемые поля должны быть инициализированы нулями.
    *
    * Заполнит структуру `options_t option` и удалит соответствующие
    * аргументы из `fuse_args args`.
    */
    if (fuse_opt_parse(&args, &options, opt_specs, NULL) == -1) {
        return 1;
    }

    print_cwd();

    int ret = fuse_main(
        args.argc,
        args.argv,
        &fuse_defined_operations,
        NULL
    );
    fuse_opt_free_args(&args);

    fprintf(stderr, "Fuse start result code: %d\n", ret);
    return ret;
}

/**
 * Implementations:
 */
void print_cwd() {
    if (options.log) {
        FILE* f = fopen(options.log, "at");
        char buffer[PATH_MAX];
        getcwd(buffer, sizeof(buffer));
        fprintf(f, "Current working dir: %s\n", buffer);
        fclose(f);
    }
}

int getattr_callback(const char* path,
                     struct stat* stbuf,
                     struct fuse_file_info* fi) {
    if (strcmp(path, "/") == 0) {
        /**
         * st_mode(тип файла, а также права доступа)
         * st_nlink(количество ссылок на файл)
         * > количество ссылок у папки = 2 + n
         *      где n - количество подпапок
         */
        *stbuf = (struct stat) {
            .st_nlink = 2,
            .st_mode = S_IFDIR | 0755
        };
        return 0;
    }

    if (path[0] == '/' && strcmp(path + 1, options.filename) == 0) {
        *stbuf = (struct stat) {
            .st_nlink = 2,
            .st_mode = S_IFREG | 0777,
            .st_size = (__off_t)strlen(options.filecontent)
        };
        return 0;
    }
    /**
     * при ошибке, вместо errno возвращаем (-errno)
     */
    return -ENOENT;
}

int readdir_callback(const char* path, void* buf,
                     fuse_fill_dir_t filler,
                     off_t offset,
                     struct fuse_file_info* fi,
                     enum fuse_readdir_flags flags) {
    /**
     * filler(void* buf,
     *        const char* name,
     *        const struct stat* stbuf,
     *        off_t off,
     *        enum fuse_fill_dir_flags flags)
     * заполняет информацию о файле и вставляет её в buf
     */
    filler(buf, ".", NULL, 0, (enum fuse_fill_dir_flags)0);
    filler(buf, "..", NULL, 0, (enum fuse_fill_dir_flags)0);
    filler(buf, options.filename, NULL, 0, (enum fuse_fill_dir_flags)0);
    return 0;
}

int read_callback(const char* path,
                  char* buf,
                  size_t size,
                  off_t offset,
                  struct fuse_file_info* fi) {
    // "/":
    if (strcmp(path, "/") == 0) {
        return -EISDIR;
    }

    print_cwd();

    // "/my_file":
    if (path[0] == '/' && strcmp(path + 1, options.filename) == 0) {
        size_t len = strlen(options.filecontent);
        if (offset >= len) {
            return 0;
        }
        size = (offset + size <= len) ? size : (len - offset);
        memcpy(buf, options.filecontent + offset, size);
        return size;
    }

    return -EIO;
}
