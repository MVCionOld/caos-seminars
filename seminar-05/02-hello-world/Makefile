print_asm:
	gcc -S -Os -fno-asynchronous-unwind-tables main.c -o /dev/stdout
run_c:
	gcc -o main.out main.c && ./main.out && rm -f main.out
run_asm:
	gcc -o main.out main.s 2>/dev/null && ./main.out && rm -f main.out
debug_asm:
	gcc -ggdb -std=c99 -o main.out main.s && gdb main.out