#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int64_t f(int32_t A, int32_t B, int32_t C, int32_t x);
__asm__ (R"(
   .text
   .global f
f:
    smaddl    x0, w0, w3, x1
    smaddl    x0, w0, w3, x2
    ret
)");

int main() {
    int32_t x = 2;
    int32_t A = 1;
    int32_t B = 4;
    int32_t C = 7;
    printf("x = %"PRId32"\n", x);
    printf("%"PRId32"*x*x + %"PRId32"*x + %"PRId32" = %"PRId64"\n", A, B, C, f(A, B, C, x));
}