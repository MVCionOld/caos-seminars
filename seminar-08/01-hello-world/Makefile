run:
	gcc -o main.exe main.c && ./main.exe && rm -f main.exe
strace:
	gcc -o main.exe main.c && strace ./main.exe 2>strace.out \
		&& rm -f main.exe && echo "----" && cat strace.out && rm -f strace.out
ldd:
	gcc -m64 -masm=intel -O3 main.c -o main.exe && \
    ls -la main.exe && echo "----\n" && \
    ldd main.exe && rm -f main.exe