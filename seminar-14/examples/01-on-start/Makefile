utils.o:
	gcc -c utils.c -o utils.o
libutils.so: utils.o
	gcc -shared -fPIC utils.o -o libutils.so
main.exe: libutils.so
	gcc -Wall -g main.c -L. -lutils -Wl,-rpath -Wl,'$ORIGIN/.' -o main.exe
run: main.exe
	./main.exe
clear:
	rm -rf main.exe libutils.so utils.o main.exe.dSYM
