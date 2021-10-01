#include <assert.h>
#include <stdint.h>
#include <stdio.h>

extern uint64_t asm_fibonacci(uint16_t N);

uint64_t c_fibonacci(uint16_t N) {
    uint64_t prev = 0, curr = 1;
    for (uint16_t i = 1; i < N; ++i) {
        uint64_t prev_copy = prev;
        prev = curr;
        curr = prev_copy + curr;
    }
    return curr;
}


int main() {

    for (uint16_t i = 1; i < 94; ++i) {
        assert(c_fibonacci(i) == asm_fibonacci(i));
    }

    puts("All tests are passed!");
}