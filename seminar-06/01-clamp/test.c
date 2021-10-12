#include <stdint.h>
#include <stdio.h>
#include <assert.h>


int32_t clamp(int32_t x, int32_t a, int32_t b);

int main() {
    assert(clamp(1, 10, 20) == 10);
    assert(clamp(100, 10, 20) == 20);
    assert(clamp(15, 10, 20) == 15);
    fprintf(stderr, "All tests are passed!\n");
    return 0;
}