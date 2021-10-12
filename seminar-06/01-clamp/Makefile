show_asm:
	gcc -m64 -masm=intel -O2 clamp.c -S -o /dev/stdout
test_plain_c:
	gcc -m64 -masm=intel -O2 clamp.c test.c -o clamp_test_plain_c.exe && \
		./clamp_test_plain_c.exe && \
		rm -f clamp_test_plain_c.exe
test_asm_comp:
	gcc -m64 -masm=intel -O2 clamp_comp.S test.c -o clamp_test_asm_comp.exe && \
		./clamp_test_asm_comp.exe && \
		rm -f clamp_test_asm_comp.exe
test_asm_cond_mov:
	gcc -m64 -masm=intel -O2 clamp_cond_mov.S test.c -o clamp_test_asm_cond_mov.exe && \
		./clamp_test_asm_cond_mov.exe && \
		rm -f clamp_test_asm_cond_mov.exe
test_asm_jump:
	gcc -m64 -masm=intel -O2 clamp_jump.S test.c -o clamp_test_asm_jump.exe && \
		./clamp_test_asm_jump.exe && \
		rm -f clamp_test_asm_jump.exe
