#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void interact(int client_socket) {
    int payload = 42;
    int server_answer;

    // Пример взаимодействия через read/write
    write(client_socket, &payload, sizeof(payload));
    read(client_socket, &server_answer, sizeof(server_answer));
    printf("[read/write] Client payload: %d\n", payload);
    printf("[read/write] Server answer: %d\n", server_answer);

    // Пример взаимодействия через send/recv
    send(client_socket, &payload, sizeof(payload), 0);
    recv(client_socket, &server_answer, sizeof(server_answer), 0);
    printf("[send/recv] Client payload: %d\n", payload);
    printf("[send/recv] Server answer: %d\n", server_answer);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        return 1;
    }
    const char* host = argv[1];
    const int port = (int) strtol(argv[2], NULL, 10);

    int client_socket = socket(
        /* domain = */ AF_INET,   // семейство протоколов
        /* type = */ SOCK_STREAM, // указываем, что обрабатываем TCP-соединение
        /* protocol = */ 0
    );
    if (client_socket == -1) {
        return 2;
    }
    struct sockaddr_in addr = (struct sockaddr_in) {
        .sin_family = AF_INET,  // семейство протоколов
        .sin_port = htons(port) // локальный порядок байт в сетевой: Little Endian -> Big Endian
    };
    // `gethostbyname` - устаревшая функция, лучше использовать `getaddrinfo`, не поддерживает ipv6
    struct hostent* hosts = gethostbyname(host);
    memcpy(
        (void*) &addr.sin_addr,
        (void*) hosts->h_addr_list[0],
        sizeof(addr.sin_addr)
    );

    int connect_ret = -1;
    while (connect_ret == -1) {  // сервер может не стартовать к этому моменту
        connect_ret = connect(
            /* socket = */ client_socket,
            /* address = */ (struct sockaddr*)& addr,
            /* address_len */ sizeof(addr)  // попытка в наследование на структурах
        );
        fprintf(stderr, "Unable to connect to server. Retrying...\n");
        fflush(stderr);
        sleep(1);
    }

    interact(client_socket);

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);

    return 0;
}
