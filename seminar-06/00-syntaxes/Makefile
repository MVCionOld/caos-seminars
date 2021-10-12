att:
	gcc -m64 -masm=att -O3 sum.c -S -o att_example.S \
	&& cat att_example.S && rm -f att_example.S

intel:
	gcc -m64 -masm=intel -O3 sum.c -S -o intel_example.S \
	&& cat intel_example.S && rm -f intel_example.S