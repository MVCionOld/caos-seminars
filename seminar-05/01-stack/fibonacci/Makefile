test:
	gcc -std=c99 -o main.out main.c asm.s && ./main.out && rm -f main.out 2>/dev/null

debug:
	gcc -ggdb -std=c99 -o main.out main.c asm.s && gdb main.out
