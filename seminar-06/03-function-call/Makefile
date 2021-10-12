show_asm:
	gcc -m64 -masm=intel -Os hello.c -S -o /dev/stdout

run_asm:
	gcc -m64 -masm=intel -O3 main.c hello.S -o hello.exe && ./hello.exe && rm -f hello.exe