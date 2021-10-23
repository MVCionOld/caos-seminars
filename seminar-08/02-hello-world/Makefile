ldd:
	gcc -m64 -masm=intel -static -flto -O3 main.c -o main.exe && \
	ls -la main.exe && \
	ldd main.exe
	rm -f main.exe
objdump:
	gcc -m64 -masm=intel -static -flto -O3 main.c -o main.exe && \
	objdump -M intel -d main.exe | grep "<_start>" -A 10 >_start.out && \
	objdump -M intel -d main.exe | grep "<main>" -A 10 >main.out && \
	objdump -M intel -d main.exe | grep "<__libc_write>:" -A 8 | head -n 100 >__libc_write.out && \
	strace ./main.exe && \
	echo "---- <_start> ----\n" && \
  	cat _start.out && \
	echo "---- <main> ----\n" && \
  	cat main.out && \
	echo "---- <__libc_write> ----\n" && \
  	cat __libc_write.out && \
	rm -f main.exe