#include <pthread.h>
#include <stdio.h>

int main() {
    pthread_mutex_t mtx;
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mtx, &attr);

    pthread_mutex_lock(&mtx);
    pthread_mutex_lock(&mtx); // <-- ordinary mutex cause deadlock

    printf("Hello!\n");

    pthread_mutex_unlock(&mtx);
    pthread_mutex_unlock(&mtx);

    pthread_mutex_destroy(&mtx);
    return 0;
}