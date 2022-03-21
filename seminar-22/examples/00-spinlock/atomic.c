#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

enum {
  WORKER_POOL_SIZE = 2,
  TICS_NUM = 1000000
};

typedef struct {
  _Atomic(int) data_holder;
} thread_args_t;


static void*
thread_func(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    for (int tic = 0; tic < TICS_NUM; ++tic) {
        thread_args->data_holder++;
    }

    return NULL;
}

int main() {
    thread_args_t shared_state = (thread_args_t) {
        .data_holder = 0
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

    printf("%d\n", shared_state.data_holder);

    return exit_code;
}