#include "structs_in_memory_common.h"

int main() {

    typedef struct { // максимальное выравнивание у инта, значит выравнивание структуры 4
      char c;      // 0 байт
      int i;       // 4-7 байты
      char c2;     // 8 байт
    } Obj1_t;        // 9-11 - padding байты, чтобы размер делился на выравнивание
    print_info(Obj1_t);
    print_offset(Obj1_t, c);
    print_offset(Obj1_t, i);
    print_offset(Obj1_t, c2);

    typedef struct { // тут все правила про выравнивание не применимы, так как указан аттрибут упаковки
      char c;
      int i;
      char c2;
    } __attribute__((packed)) Obj2_t;
    print_info(Obj2_t);
    print_offset(Obj2_t, c);
    print_offset(Obj2_t, i);
    print_offset(Obj2_t, c2);

    typedef struct {  // максимальное выравнивание члена - 8, так что и у всей структуры такое же
      char c8;      // 0 байт
      uint64_t u64; // 8-15 байты
    } Obj3_t;         // всего 16 байт, выравнивание 8
    print_info(Obj3_t);
    print_offset(Obj3_t, c8);
    print_offset(Obj3_t, u64);

    typedef struct {
      char c8;
      char c8_1;
      char c8_2;
    } Obj4_t;
    print_info(Obj4_t);
    print_offset(Obj4_t, c8);
    print_offset(Obj4_t, c8_1);
    print_offset(Obj4_t, c8_2);

    typedef struct {     // тут пример двух структур равного размера, но с разным выравниванием
      long long a;
    } ObjS8A8;
    print_info(ObjS8A8);
    typedef struct {
      int a;
      int b;
    } ObjS8A4;
    print_info(ObjS8A4);

    typedef struct {    // и вот тут разное выравнивание ObjS8A8 и ObjS8A4 себя покажет
      ObjS8A8 o;
      char c;
    } Obj5_t;
    print_info(Obj5_t); // обратите внимание на разницу с Obj6_t!

    typedef struct {
      ObjS8A4 o;
      char c;
    } Obj6_t;
    print_info(Obj6_t);

    typedef union {
      unsigned long long u;
      int i[3];
    } Obj7_t;
    print_info(Obj7_t); // то же самое, что и с Obj5_t

    return 0;
}