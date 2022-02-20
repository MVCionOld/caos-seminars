run_client_main:
	gcc -o client_main.exe -std=c11 client.c
	./client_main.exe localhost 8080
run_client_getaddrinfo_main:
	gcc -o client_getaddrinfo_main.exe -std=c11 -DDEBUG client_getaddrinfo.c
	./client_getaddrinfo_main.exe localhost 8080
run_server_main:
	gcc -o run_server_main.exe -std=c11 -DDEBUG server.c
	./run_server_main.exe 0.0.0.0 8080
clear:
	rm -f run_server_main.exe client_main.exe client_getaddrinfo_main.exe