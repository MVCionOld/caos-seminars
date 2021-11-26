sum.o:
	gcc -c sum.c -o sum.o
libsum.so: sum.o
	gcc -shared -fPIC sum.o -o libsum.so
main.exe: libsum.so
	gcc main.c -o main.exe -ldl
run: main.exe
	./main.exe
clean:
	rm -rf main.exe sum.o libsum.so main.exe.dSYM