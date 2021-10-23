syscall32:
	gcc -m32 -masm=intel -static -flto -O3 main.c -o main32.exe && \
	objdump -M intel -d main32.exe | grep "<__libc_write>:" -A 15 | head -n 100 > syscall32.out && \
	cat syscall32.out && rm -f syscall32.out

syscall64:
	gcc -m64 -masm=intel -static -flto -O3 main.c -o main64.exe
	objdump -M intel -d main64.exe | grep "<__libc_write>:" -A 8 | head -n 100 > syscall64.out && \
	cat syscall64.out && rm -f syscall64.out
