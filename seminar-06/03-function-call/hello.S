    .intel_syntax noprefix
    .text
    .globl hello
hello:
                            /* rdi, rsi, rdx, rcx */
    push rdi                /* rsp -= 8 */
    mov  rdx, rsi
    mov  rsi, rdi
    lea  rdi, .format_s[rip]
    call printf
    pop  rax                 /* rsp += 8 */
    ret
.section   .rodata
.format_s:
   .string "Hello %d and %d\n"