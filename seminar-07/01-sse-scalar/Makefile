show_math_sse:
	gcc -m64 -masm=intel -O3 math.c -S -o math.S && \
	cat math.S | ../asm_filter_useless && \
	rm -f math.S

show_math_avx:
	gcc -mavx -m64 -masm=intel -O3 math.c -S -o math.S
	cat math.S | ../asm_filter_useless && \
	rm -f math.S