fail:
	gcc -m64 -masm=intel -nostdlib -O3 fail.c -o fail.exe && \
	strace ./fail.exe
	rm -f fail.exe
empty_main:
	gcc -m64 -masm=intel -O3 empty_main.c -o empty_main.exe
	strace ./empty_main.exe 2>&1 | tail -n 3
	rm -f empty_main.exe
nofail_worse:
	gcc -m64 -masm=intel -nostdlib -O3 nofail_worse.S -o nofail_worse.exe && ./nofail_worse.exe && rm -f nofail_worse.exe
nofail_better:
	gcc -m64 -masm=intel -nostdlib -O3 nofail_better.S -o nofail_better.exe && ./nofail_better.exe && rm -f nofail_better.exe
_start:
	gcc -std=gnu11 -m64 -masm=intel -nostdlib -O3 _start.c -o _start.exe && ./_start.exe && rm -f _start.exe
main:
	gcc -std=gnu11 -g -m64 -masm=intel -nostdlib -O3 main.c -o main.exe
	gcc -std=gnu11 -m64 -masm=intel -nostdlib -fno-asynchronous-unwind-tables -O3 main.c -S -o main.S
	ls -la main.exe
	cat main.S
	objdump -d main.exe
	./main.exe
	rm -f main.exe