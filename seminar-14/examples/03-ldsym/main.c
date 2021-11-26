#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *lib_handle = dlopen("./libsum.so", RTLD_NOW); // also RTLD_LAZY
    if (!lib_handle) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        abort();
    }

    int (*sum)(int, int) = dlsym(lib_handle, "sum");

    printf("sum(1, 1)     = %d\n", sum(1, 1));
    printf("sum(10, 100)  = %d\n", sum(10, 100));
    printf("sum(40, 5000) = %d\n", sum(40, 5000));

    return 0;
}