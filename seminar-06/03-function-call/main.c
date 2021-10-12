#include <stdio.h>

// int hello(int a, int b) {
//     printf("Hello %d and %d\n", a, b);
//     return a;
// }

int hello(int a, int b);

int main() {
    int a1 = hello(1, 2);
    int a2 = hello(10, 20);
    printf("a1 + a2 = %d\n", a1 + a2);
    return 0;
}
