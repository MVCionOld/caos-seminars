#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int32_t sum(int32_t n, int32_t* x);

int main() {

    {
        int32_t x[] = {100, 2, 200, 3};
        assert(sum(sizeof(x) / sizeof(int32_t), x) == 100 - 2 + 200 - 3);
    }

    {
        int32_t x[] = {100, 2, 200};
        assert(sum(sizeof(x) / sizeof(int32_t), x) == 100 - 2 + 200);
    }

    {
        int32_t x[] = {100};
        assert(sum(sizeof(x) / sizeof(int32_t), x) == 100);
    }

    fprintf(stderr, "All tests are passed!\n");

    return 0;
}