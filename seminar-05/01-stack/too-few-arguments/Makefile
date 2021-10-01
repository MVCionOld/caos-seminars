test_naive:
	gcc -std=c99 -o naive.out main.c asm_naive.s && ./naive.out && rm -f naive.out 2>/dev/null

test_iterative:
	gcc -std=c99 -o iterative.out main.c asm_iterative.s && ./iterative.out && rm -f iterative.out 2>/dev/null

debug_iterative:
	gcc -ggdb -std=c99 -o iterative.out main.c asm_iterative.s && gdb iterative.out
