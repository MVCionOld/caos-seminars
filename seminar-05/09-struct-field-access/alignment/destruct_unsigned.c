#include <stdint.h>

typedef struct {
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
}  __attribute__((packed)) complicated_t;


void parse(complicated_t* a, uint8_t* du8, uint16_t* du16, uint32_t* du32, uint64_t* du64) {
    *du8 = a->u8;
    *du16 = a->u16;
    *du32 = a->u32;
    *du64 = a->u64;
}