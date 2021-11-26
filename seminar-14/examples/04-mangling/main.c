#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  CLASS_OBJECT_SIZE = 128
};

int main() {

    void *lib_handle = dlopen("./libsumcpp.so", RTLD_NOW);
    if (!lib_handle) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        abort();
    }

    int (*sum_c)(int, int) = dlsym(lib_handle, "sum_c");
    int (*sum)(int, int) = dlsym(lib_handle, "_Z7sum_cppii");
    float (*sum_f)(float, float) = dlsym(lib_handle, "_Z9sum_cpp_fff");

#define p(stmt, fmt) printf(#stmt " = " fmt "\n", stmt);

    {
        p(sum_c(1, 1), "%d");
        p(sum_c(40, 5000), "%d");

        p(sum(1, 1), "%d");
        p(sum(40, 5000), "%d");

        p(sum_f(1, 1), "%0.2f");
        p(sum_f(4.0, 500.1), "%0.2f");
    }

    char* obj[CLASS_OBJECT_SIZE];
    void (*constructor)(void*, int) = dlsym(lib_handle, "_ZN7TSummerC1Ei");
    int (*sumA)(void*, int) = dlsym(lib_handle, "_ZN7TSummer4SumAEi");

    p((constructor(obj, 10), sumA(obj, 1)), "%d");
    // operator, -- выполняет все команды и берет возвращаемое значение последнего
    p((constructor(obj, 4000), sumA(obj, 20)), "%d");

    dlclose(lib_handle);
    return 0;
}