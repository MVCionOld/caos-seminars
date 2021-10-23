    .intel_syntax noprefix
    .text

my_exit:
    mov rax, 231
    syscall

.globl _start
_start:
    mov  rdi, 0
    call my_exit