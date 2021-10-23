#include <sys/syscall.h>

.intel_syntax noprefix
.text

my_exit:
    mov rax, SYS_exit_group
    syscall

.globl _start
_start:
    mov  rdi, 0
    call my_exit