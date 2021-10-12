// Task:
// Write function `// Write function `int32_t sum(int32_t n, int32_t* x);`
// that calculates: $\sum_{i=0}^{n - 1} (-1)^i \cdot x[i]$

    .intel_syntax noprefix
    .text
    .globl sum
sum:
                    // n -> edi
                    // x -> rsi
    mov eax, 0
    cmp edi, 0      // set flags for next jmp instruction
.start_loop:
    jle .return_eax
    add eax, DWORD PTR [rsi]
    add rsi, 4
    dec edi         // and write compare with 0 flags

    jle .return_eax
    sub eax, DWORD PTR [rsi]
    add rsi, 4
    dec edi         // and write compare with 0 flags

    jmp .start_loop
.return_eax:
    ret