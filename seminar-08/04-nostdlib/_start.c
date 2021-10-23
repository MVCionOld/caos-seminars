int my_exit(int code);
__asm__(R"(
my_exit:
    mov rax, 231
    syscall
)");

void _start() {
    my_exit(42);
}