#include <assert.h>

int main() {
    volatile int i = 0;
    for (; i < 5; ++i);
    assert(2 + 2 == i);
    return 0;
}