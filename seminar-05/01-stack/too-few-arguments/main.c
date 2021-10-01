#include <assert.h>
#include <stdint.h>
#include <stdio.h>

extern int64_t few_arguments_function_sum(
    int64_t i0, int64_t i1, int64_t i2, int64_t i3,
    int64_t i4, int64_t i5, int64_t i6, int64_t i7,
    int64_t i8, int64_t i9, int64_t i10, int64_t i11 // <--- where are they stored?
);


int main() {
    {
        assert(few_arguments_function_sum(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) == 0);
    }

    {
        assert(few_arguments_function_sum(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11) == 66);
    }

    {
        assert(few_arguments_function_sum(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42) == 42);
    }

    puts("All tests are passed!");
}