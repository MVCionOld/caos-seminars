#ifndef MIPT_ATP_CAOS_SEMINARS_STAND_H
#define MIPT_ATP_CAOS_SEMINARS_STAND_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define EXTRA_INFO // включение более подробного вывода

#if defined(EXTRA_INFO)
#define IS_VLINE_POINT(i) (i == 63 || i == 52)
#define DESCRIBE(d) describe_double(d)

typedef union {
  double double_val;
  struct {
    uint64_t mantissa_val: 52;
    uint64_t exp_val: 11;
    uint64_t sign_val: 1;
  };
} double_parser_t;

void describe_double (double x) {
    double_parser_t parser = {.double_val = x};
    printf("  (-1)^%d * 2^(%d) * 0x1.%013llx",
           (int) parser.sign_val, parser.exp_val - 1023, (long long unsigned int) parser.mantissa_val);
}

#else
#define IS_VLINE_POINT(i) 0
#define DESCRIBE(d) (void)(d)
#endif

uint64_t bits_of_double (double d) {
    uint64_t result;
    memcpy(&result, &d, sizeof(result));
    return result;
}

void print_doubles (double *dds) {
    char line_1[70] = {0}, line_2[70] = {0}, hline[70] = {0};
    int j = 0;
    for (int i = 63; i >= 0; --i) {
        line_1[j] = (i % 10 == 0) ? ('0' + (i / 10)) : ' ';
        line_2[j] = '0' + (i % 10);
        hline[j] = '-';
        ++j;
        if (IS_VLINE_POINT(i)) {
            line_1[j] = line_2[j] = '|';
            hline[j] = '-';
            ++j;
        }
    }
    printf("Bit numbers: %s\n", line_1);
    printf("             %s  (-1)^S * 2^(E-B) * (1+M/(2^Mbits))\n", line_2);
    printf("             %s\n", hline);
    for (double *d = dds; *d; ++d) {
        printf("%10.4lf   ", *d);
        uint64_t m = bits_of_double(*d);
        for (int i = 63; i >= 0; --i) {
            printf("%d", (int) ((m >> i) & 1));
            if (IS_VLINE_POINT(i)) {
                printf("|");
            }
        }
        DESCRIBE(*d);
        printf("\n");
    }
}

#endif //MIPT_ATP_CAOS_SEMINARS_STAND_H
