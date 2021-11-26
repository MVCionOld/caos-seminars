build_sum:
	gcc -g -fPIC sum.c -c -o sum.o

objdump: build_sum
	objdump -d -F sum.o | grep sum -A 42

build: build_sum
	gcc -g main.c -o main

run: build
	./main

clean:
	rm -rf main sum.o main.dSYM