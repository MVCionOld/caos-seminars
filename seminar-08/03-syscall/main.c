#include <unistd.h>

int main() {
    int w = write(1, "Hello, World!", 13);
    return 0;
}