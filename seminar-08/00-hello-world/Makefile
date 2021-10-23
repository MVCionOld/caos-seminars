run:
	gcc -o main.exe main.c && ./main.exe && rm -f main.exe
strace:
	gcc -o main.exe main.c && strace ./main.exe 2>strace.out \
		&& rm -f main.exe && echo "----" && cat strace.out && rm -f strace.out