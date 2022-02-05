int main() {
    char str[128];
    volatile int i = 0;
    for (; i < 2048; ++i);
    return str[i];
}