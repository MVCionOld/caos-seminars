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

enum {
  LISTEN_BACKLOG = 32  // не более SOMAXCONN = 128 на Linux!
};

void interact(int client_fd) {
    int payload;
    int server_answer;

    // Пример взаимодействия через read/write
    read(client_fd, &payload, sizeof(payload));
    server_answer = payload + 1;
    write(client_fd, &server_answer, sizeof(server_answer));
    printf("[read/write] Client payload: %d\n", payload);
    printf("[read/write] Server answer: %d\n", server_answer);

    // Пример взаимодействия через send/recv
    recv(client_fd, &payload, sizeof(payload), 0);
    server_answer = payload + 1;
    send(client_fd, &server_answer, sizeof(server_answer), 0);
    printf("[send/recv] Client payload: %d\n", payload);
    printf("[send/recv] Server answer: %d\n", server_answer);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        return 1;
    }
    const char* host = argv[1];
    const int port = (int) strtol(argv[2], NULL, 10);

    int server_socket = socket(
        /* domain = */ AF_INET,   // семейство протоколов
        /* type = */ SOCK_STREAM, // указываем, что обрабатываем TCP-соединение
        /* protocol = */ 0
    );
    if (server_socket == -1) {
        return 2;
    }

    // Принудительное повторное использование адреса и порта.
    // В release-сборке такого обычно быть не должно!
#ifdef DEBUG
    int val = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
#endif

    struct sockaddr_in addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = 0 // принимать соединения со всех адресов
    };
    int bind_ret = bind(
        /* socket = */ server_socket,
        /* address = */ (const struct sockaddr*)& addr,
        /* address_len = */ sizeof(struct sockaddr_in)
    );
    if (bind_ret == -1) {
        return 3;
    }

     int listen_ret = listen(
         /* socket = */ server_socket,
         /* backlog = */ LISTEN_BACKLOG // указываем размер очереди ожидающих соединений
     );
    if (listen_ret == -1) {
        return 4;
    }

    struct sockaddr_in client_addr;  // сюда сохраним инфу о клиенте
    socklen_t socklen = (socklen_t) sizeof(struct sockaddr_in);
    memset(&client_addr, 0, socklen);
    int client_fd = accept(  // принимаем соединение и записываем адрес
        /* socket = */ server_socket,
        /* address = */ (struct sockaddr*)& client_addr,
        /* address_len = */&socklen
    );

    interact(client_fd);

    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);

    return 0;
}
