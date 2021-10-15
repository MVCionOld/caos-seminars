#include <xmmintrin.h>

void add_mem_double(double* restrict a_, double* restrict b_) {
    double *a = __builtin_assume_aligned(a_, 16);
    double *b = __builtin_assume_aligned(b_, 16);
    for (int i = 0; i < 2; ++i) {
        a[i] += b[i];
    }
}

void add_mem_double_intr(double* a, double* b) {
    *(__m128d*)a = _mm_add_pd(*(__m128d*)a, *(__m128d*)b);
}

void add_mem_float(float* restrict a_, float* restrict b_) {
    float *a = __builtin_assume_aligned(a_, 32);
    float *b = __builtin_assume_aligned(b_, 32);
    for (int i = 0; i < 8; ++i) {
        a[i] += b[i];
    }
}