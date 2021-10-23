#include <sys/syscall.h>


// Универсальная функция для совершения системных вызовов (до 5 аргументов системного вызова)
int syscall(int code, ...);
__asm__(R"(
syscall:
    /* Function arguments: rdi, rsi, rdx, rcx, r8, r9 */
    /* Syscall arguments: rax (syscall num), rdi, rsi, rdx, r10, r8, r9.*/
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov r10, r8
    mov r8, r9
    syscall
    ret
)");

void my_exit(int code) {
    syscall(SYS_exit, code);
}

int write(int fd, const void* data, int size) {
    return syscall(SYS_write, fd, data, size);
}


void int_to_s(unsigned int i, char* s, int* len) {
    int clen = 0;
    for (int ic = i; ic; ic /= 10, ++clen);
    clen = clen ?: 1;
    s[clen] = '\0';
    for (int j = 0; j < clen; ++j, i /= 10) {
        s[clen - j - 1] = '0' + i % 10;
    }
    *len = clen;
}

unsigned int s_to_int(char* s) {
    unsigned int res = 0;
    while ('0' <= *s && *s <= '9') {
        res *= 10;
        res += *s - '0';
        ++s;
    }
    return res;
}

int print_int(int fd, unsigned int i) {
    char s[20];
    int len;
    int_to_s(i, s, &len);
    return write(fd, s, len);
}

int print_s(int fd, const char* s) {
    int len = 0;
    while (s[len]) ++len;
    return write(fd, s, len);
}




const char hello_s[] = "Hello world from function 'write'!\n";
const int hello_s_size = sizeof(hello_s);

// Забавно, но перед вызовом функции start стек не был выровнен по 16 :)
// Вернее был, но видимо не положили адрес возврата (так как не нужен), а сишный компилятор его ожидает...
__asm__(R"(
.globl _start
_start:
    sub rsp, 8
    jmp main
)");


void main() {
    const char hello_s_2[] = "Hello world from 'syscall'!\n";
    write(1, hello_s, sizeof(hello_s) - 1);
    syscall(SYS_write, 1, hello_s_2, sizeof(hello_s_2) - 1);
    print_s(1, "Look at this value: "); print_int(1, 10050042);
    print_s(1, "\n");
    print_s(1, "Look at this value: "); print_int(1, s_to_int("123456"));
    print_s(1, "\n");

    my_exit(0);
}