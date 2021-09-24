#include <assert.h>
#include <stdint.h>
#include <stdio.h>

extern uint64_t asm_for_loop_inc(uint32_t N, uint32_t x0);

uint64_t c_for_loop_inc(uint32_t N, uint32_t x0) {
    for (uint32_t i = 0; i < N; ++i) {
        ++x0;
    }
    return x0;
}


int main() {
    assert(c_for_loop_inc(0u, 0u) == asm_for_loop_inc(0u, 0u));

    assert(c_for_loop_inc(42u, 0u) == asm_for_loop_inc(42u, 0u));

    assert(c_for_loop_inc(0u, 42u) == asm_for_loop_inc(0u, 42u));

    assert(c_for_loop_inc(37u, 41u) == asm_for_loop_inc(37u, 41u));

    puts("All tests are passed!");
}