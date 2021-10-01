run_asm:
	gcc -o main.out main.s 2>/dev/null && ./main.out && rm -f main.out
debug_asm:
	gcc -ggdb -std=c99 -o main.out main.s && gdb main.out