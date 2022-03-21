#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


enum {
  WORKER_POOL_SIZE = 2,
  TICS_NUM = 1000000
};

typedef struct {
  int64_t data_holder;
  pthread_mutex_t* mtx;
} thread_args_t;

static void*
thread_func(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    for (int tic = 0; tic < TICS_NUM; ++tic) {
        pthread_mutex_lock(thread_args->mtx);
        thread_args->data_holder++; // critical zone
        pthread_mutex_unlock(thread_args->mtx);
    }

    return NULL;
}

int main() {
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

    thread_args_t shared_state = (thread_args_t) {
        .data_holder = 0,
        .mtx = &mtx
    };

    pthread_t worker_pool[WORKER_POOL_SIZE];
    for (int worker = 0; worker < WORKER_POOL_SIZE; ++worker) {
        int code = pthread_create(
            &worker_pool[worker],
            NULL,
            thread_func,
            (void*) &shared_state
        );
        if (code != 0) {
            return 1;
        }
    }

    int exit_code = 0;
    for (int worker = 0; worker < WORKER_POOL_SIZE; ++worker) {
        int code = pthread_join(
            worker_pool[worker],
            NULL
        );
        if (code != 0) {
            exit_code = 2;
        }
    }

    pthread_mutex_destroy(&mtx);
    printf("%"PRId64"\n", shared_state.data_holder);
    return exit_code;
}