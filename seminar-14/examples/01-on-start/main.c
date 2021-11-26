#include <assert.h>
#include <stdio.h>
#include <math.h>

int sum_ints(int a, int b);
float sum_floats(float a, float b);

int main() {
    {
        assert(sum_ints(1, 1) == 2);
        assert(sum_ints(40, 5000) == 5040);
    }

    {
        assert(fabs(sum_floats(1.0, 1.0) - 2.0) < 0.0001);
        assert(fabs(sum_floats(4.0, 500.1) - 504.1) < 0.0001);
    }

    puts("4 tests completed successfully");
    return 0;
}