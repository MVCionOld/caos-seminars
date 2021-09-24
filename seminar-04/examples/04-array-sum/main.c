#include <assert.h>
#include <stdio.h>

// Реализуйте на языке ассемблера armv8 (AArch64) функцию с сигнатурой:
// int sum(int x0, size_t N, int *X)
// Функция должна вычислять значение x0+∑xi, где 0≤i<N
// Обратите внимание на то, что тип int имеет размер 32 бит, а тип size_t - 64 бит.
// Ссылка на комнату: https://interview.cups.online/live-coding/?room=7545986e-1d79-4cb4-9c38-d47158a42498

extern int sum(int x0, size_t N, int *X);

int main() {

    {
        int X[] = {1, 2, 3, 4, 5};
        int x0 = 12;
        assert(sum(x0, (size_t)5, X) == 27);
    }

    {
        int X[] = {-1, 1, 0, -200, 400, 21, 21};
        int x0 = -200;
        assert(sum(x0, (size_t)5, X) == 42);
    }

    puts("All tests are passed!");
}