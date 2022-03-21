run_dummy:
	gcc -std=c11 dummy.c -lpthread -o dummy.exe #-fsanitize=thread
	./dummy.exe

run_atomic:
	gcc -std=c11 atomic.c -lpthread -o atomic.exe -fsanitize=thread
	time ./atomic.exe

run_spinlock_diy:
	gcc -std=c11 spinlock_diy.c -lpthread -o spinlock_diy.exe -fsanitize=thread
	time ./spinlock_diy.exe

run_spinlock:
	gcc -std=gnu11 spinlock.c -lpthread -o spinlock.exe -fsanitize=thread
	time ./spinlock.exe

clear:
	rm -f dummy.exe atomic.exe spinlock_diy.exe spinlock.exen