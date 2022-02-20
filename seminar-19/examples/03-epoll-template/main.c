#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <wait.h>
#include <sys/epoll.h>
#include <assert.h>


#define conditional_handle_error(stmt, msg) \
    do { if (stmt) { perror(msg " (" #stmt ")"); exit(EXIT_FAILURE); } } while (0)

int process_request(...) {
    // ...
    return 0;
}

// должен работать до тех пор, пока в stop_fd не появится что-нибудь доступное для чтения
int server_main (int argc, char **argv, int stop_fd) {
    assert(argc >= 2);

    //...
    // int socket_fd = ....

    // создаем специальную штуку, чтобы ждать события из двух файловых дескрипторов разом: из сокета и из останавливающего дескриптора
    int epoll_fd = epoll_create1(0);
    {
        int fds[] = {stop_fd, socket_fd, -1};
        for (int *fd = fds; *fd != -1; ++fd) {
            struct epoll_event event = {
                .events = EPOLLIN | EPOLLERR | EPOLLHUP, // запись, ошибка, whatever
                .data = {.fd = *fd}
            };
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, *fd, &event);
        }
    }

    while (true) {
        // Ждем события в epoll_fd (произойдет при появлении данных в stop_fd или socket_fd)
        struct epoll_event event;
        // Читаем события из epoll-объект (то есть из множества fds, по которым есть события)
        int epoll_ret = epoll_wait(
            epoll_fd,
            &event,
            1,
            1000
        );
        if (epoll_ret <= 0) {
            continue;
        }
        // Если пришло событие из stop_fd - пора останавливаться
        if (event.data.fd == stop_fd) {
            break;
        }
        // Иначе пришло событие из socket_fd и accept
        // отработает мгновенно, так как уже подождали в epoll
        int fd = accept(socket_fd, NULL, NULL);
        // ... а тут обрабатываем соединение
        process_request();
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }

    close(epoll_fd);

    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);

    return 0;
}

static int stop_pipe_fds[2] = {-1, -1};

static void stop_signal_handler (int signum) {
    // Самая первая запись одного символа в пайп пройдет всегда успешно, так как буффер пуст.
    write(stop_pipe_fds[1], "X",1);
}

int main (int argc, char **argv) {
    // Идея такая: в момент прихода терминирующего сигнала запишем что-то в пайп
    pipe(stop_pipe_fds);
    fcntl( // Делаем запись неблокирующей, чтобы никогда не зависнуть в хендлере (даже если придет 100500 сигналов)
        stop_pipe_fds[1],
        F_SETFL,
        fcntl(stop_pipe_fds[1], F_GETFL, 0) | O_NONBLOCK
    );

    // Пусть при получении указанных сигналов, что-нибудь запишется в пайп
    int signals[] = {SIGINT, SIGTERM, 0};
    for (int *signal = signals; *signal; ++signal) {
        sigaction(
            *signal,
            &(struct sigaction) {
                .sa_handler = stop_signal_handler,
                .sa_flags = SA_RESTART
            },
            NULL
        );
    }

    int ret = server_main(argc, argv, stop_pipe_fds[0]);

    close(stop_pipe_fds[0]);
    close(stop_pipe_fds[1]]);
    return ret;
}