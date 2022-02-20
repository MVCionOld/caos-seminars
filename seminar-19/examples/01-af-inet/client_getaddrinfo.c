#include <arpa/inet.h>
#include <errno.h>
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
    MAX_RETRIES = 10
};

int create_client_socket(const char* hostname, const char* service, int ai_family) {
    struct addrinfo hints = (struct addrinfo){
        .ai_family = ai_family,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = 0,
        .ai_protocol = 0  // any
    };

    struct addrinfo* result = NULL;
    int err = getaddrinfo(
        /* hostname = */ hostname,
        /* service = */ service,
        /* hints = */ &hints,
        /* res = */ &result
    );
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return -1;
    }

    // Function getaddrinfo returns a list of addrinfo structures.
    // Try each address until we successfully connect(2).
    // If socket(2) (or connect(2)) fails, we (close the socket and) try the next address.
    int client_socket;
    struct addrinfo *addr;

    int connect_ret;
    for (addr = result; addr != NULL; addr = addr->ai_next) {
#ifdef DEBUG
        char hbuf[NI_MAXHOST];
        char sbuf[NI_MAXSERV];
        if (getnameinfo(
            addr->ai_addr, addr->ai_addrlen,
            hbuf,
            sizeof(hbuf),
            sbuf,
            sizeof(sbuf),
            NI_NUMERICHOST | NI_NUMERICSERV
            ) == 0) {
            printf(
                "Try ai_family=%d host=%s, serv=%s\n",
                addr->ai_family,
                hbuf,
                sbuf
            );
            fflush(stdout);
        }
#endif
        client_socket = socket(
            /* domain = */ addr->ai_family,
            /* type = */ addr->ai_socktype,
            /* protocol = */ addr->ai_protocol
        );
        if (client_socket == -1) {
            continue;
        }

        connect_ret = -1;
        for (int attempt = 1; attempt <= MAX_RETRIES; ++attempt) {
            connect_ret = connect( // сервер может не стартовать к этому моменту
                /* socket = */ client_socket,
                /* address = */ addr->ai_addr,
                /* address_len */ addr->ai_addrlen // попытка в наследование на структурах
            );
            if (connect_ret != -1) {
                break;
            }
            fprintf(stderr, "Unable to connect to host (errno=%d). Retrying...\n", errno);
            fflush(stderr);
            struct timespec ts = (struct timespec) {
                .tv_sec = 1,
                .tv_nsec = 0
            };
            nanosleep((struct timespec*)&ts, &ts);
        }

        if (connect_ret != -1) {
            printf("client_socket=%d\n", client_socket);
            break;
        }

        close(client_socket); // не забываем закрыть
    }

    freeaddrinfo(result);
    return connect_ret != -1 ? client_socket : -1;
}

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
    const char* port = argv[2];

    int client_socket = create_client_socket(host, port, AF_INET);
    if (client_socket < 0) {
        return 2;
    }

    interact(client_socket);

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);

    return 0;
}
