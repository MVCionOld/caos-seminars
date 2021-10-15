show_math:
	gcc -m32 -masm=intel -mfpmath=387 -O3 math.c -S -o math.S && \
	cat math.S | ../asm_filter_useless && \
	rm -f math.S

show_exp:
	gcc -std=c99 -m32 -masm=intel -mfpmath=387 -O3 exp.c -S -o exp.S && \
	cat exp.S && rm -f exp.S

run_c_exp:
	gcc -std=c99 -g3 -m32 -masm=intel main.c exp.c -o main && ./main && rm -f main

run_asm_exp:
	gcc -g3 -m32 -masm=intel main.c asm_exp.S -o main && ./main && rm -f main