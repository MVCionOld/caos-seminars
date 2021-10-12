        .intel_syntax noprefix
        .text
        .globl  clamp
clamp:
                            // x -> edi
                            // a -> esi
                            // b -> edx
        mov   eax, edi      // eax <- x
        cmp   eax, esi      // x ? a
        cmovl eax, esi      // if (x < a) then eax <- a
        cmp   eax, edx      // x ? b
        cmovg eax, edx      // if (x > b) then eax <- b
        ret
