run_enough:
	gcc -std=gnu11 main.c -o main.out
	echo "Hello from file!" > file.txt
	(echo "Hello from " 2>/dev/null; sleep 5 2>/dev/null ; echo "stdin!" 2>/dev/null) | ./main.out file.txt
	rm -f file.txt main.out