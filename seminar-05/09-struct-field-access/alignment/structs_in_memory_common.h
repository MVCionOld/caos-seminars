#ifndef MIPT_ATP_CAOS_SEMINARS_STRUCTS_IN_MEMORY_COMMON_H
#define MIPT_ATP_CAOS_SEMINARS_STRUCTS_IN_MEMORY_COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define print_int(x) printf(#x " = %d\n", (int)x)

#define print_info(x) printf("%10s: size = %d, alignment = %d\n", #x, sizeof(x), _Alignof(x))

#define print_offset(type, field) {\
    type o; \
    printf("  %10s: shift of ." #field " is %d\n", #type, (int)((void*)&o.field - (void*)&o)); \
}

#endif //MIPT_ATP_CAOS_SEMINARS_STRUCTS_IN_MEMORY_COMMON_H
