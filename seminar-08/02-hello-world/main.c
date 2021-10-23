#include <unistd.h>

int main() {
    int w = write(1, "Hello world!\n", 13);
    return 0;
}