libsumcpp.so:
	g++ -std=c++11 -Wall -shared -fPIC libsumcpp.cpp -o libsumcpp.so
objdump: libsumcpp.so
	objdump -t libsumcpp.so | grep um
main.exe: libsumcpp.so
	gcc -std=c11 -Wall -o main.exe main.c -ldl
run: main.exe
	./main.exe
clear:
	rm -rf main.exe libsumcpp.so main.exe.dSYM