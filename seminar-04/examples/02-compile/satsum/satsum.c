unsigned int satsum(unsigned int x, unsigned int y) {
    unsigned int z;
    if (__builtin_uadd_overflow(x, y, &z)) {
        return ~0u;
    }
    return z;
}