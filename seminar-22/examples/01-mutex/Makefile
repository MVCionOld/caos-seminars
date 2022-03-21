run_mutex:
	gcc -std=gnu11 mutex.c -lpthread -o mutex.exe -fsanitize=thread
	time ./mutex.exe

run_recursive_mutex:
	gcc -std=gnu11 recursive_mutex.c -lpthread -o recursive_mutex.exe -fsanitize=thread
	./recursive_mutex.exe

clear:
	rm -f mutex.exe recursive_mutex.exe
