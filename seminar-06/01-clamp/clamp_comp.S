        .intel_syntax noprefix
        .text
        .globl  clamp
clamp:
                            // x -> edi
                            // a -> esi
                            // b -> edx
        cmp     edi, esi    // x ? a
        mov     eax, esi    // eax <- a
        jl      .L2         // if (x < a) then return a
        cmp     edi, edx    // x ? b
        mov     eax, edx    // eax <- b
        cmovle  eax, edi    // if (x <= b) then eax <- x
.L2:
        ret
