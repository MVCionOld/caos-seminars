#include <stdint.h>

int32_t clamp (int32_t x, int32_t a, int32_t b) {
    if (x < a) {
        return a;
    } else if (x > b) {
        return b;
    } else {
        return x;
    }
}