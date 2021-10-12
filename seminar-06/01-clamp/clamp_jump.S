        .intel_syntax noprefix
        .text
        .globl  clamp
clamp:
                            // x -> edi
                            // a -> esi
                            // b -> edx
        mov eax, esi        // eax <- a
        cmp edi, esi        // x ? a
        jl .clamp_return    // if (x < a) then return a
        mov eax, edx        // eax <- b
        cmp edi, edx        // x ? b
        jg .clamp_return    // if (x > b) then return b
        mov eax, edi        // eax <- x
.clamp_return:
        ret