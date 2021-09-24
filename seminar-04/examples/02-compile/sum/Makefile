asm:
	gcc -S -o sum.s sum.c && cat sum.s && rm -f sum.s
asm_unwind_tables:
	gcc -S -Os -fno-asynchronous-unwind-tables -o sum.s sum.c && cat sum.s && rm -f sum.s
asm_fverbose:
	gcc -S -Os -fverbose-asm -o sum.s sum.c && cat sum.s && rm -f sum.s
objdump:
	gcc -c -Os -fno-asynchronous-unwind-tables sum.c -o sum.o && \
	objdump -D sum.o | grep '<sum>' -A 3