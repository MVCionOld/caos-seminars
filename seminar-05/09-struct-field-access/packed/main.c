#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Operands {
  int64_t lhs;
  int64_t rhs;
} __attribute__((packed)) operands_t;


extern int64_t sum_operands(operands_t* operands);


int main() {

    {
        assert(
            sum_operands(&(operands_t){.lhs = 0, .rhs = 0}) == 0
        );
    }

    {
        assert(
            sum_operands(&(operands_t){.lhs = 2, .rhs = 2}) == 4
        );
    }

    {
        assert(
            sum_operands(&(operands_t){.lhs = INT32_MAX, .rhs = INT32_MIN}) == -1
        );
    }

    puts("All tests are passed!");
}