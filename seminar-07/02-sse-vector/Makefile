show_ints_sse:
	gcc -std=c99 -m64 -masm=intel -O3 ints.c -S -o ints.S && \
	cat ints.S | ../asm_filter_useless && \
	rm -f ints.S

show_doubles_sse:
	gcc -std=c99 -m64 -masm=intel -O3 doubles.c -S -o doubles.S
	cat doubles.S | ../asm_filter_useless && \
	rm -f doubles.S

show_doubles_avx:
	gcc -std=c99 -mavx -m64 -masm=intel -O3 doubles.c -S -o doubles.S
	cat doubles.S | ../asm_filter_useless && \
	rm -f doubles.S