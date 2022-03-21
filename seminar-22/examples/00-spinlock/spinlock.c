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
  pthread_spinlock_t* spinlock;
} thread_args_t;

static void*
thread_func(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    for (int tic = 0; tic < TICS_NUM; ++tic) {
        pthread_spin_lock(thread_args->spinlock);
        thread_args->data_holder++; // critical zone
        pthread_spin_unlock(thread_args->spinlock);
    }

    return NULL;
}

int main() {
    pthread_spinlock_t spinlock;
    pthread_spin_init(&spinlock, 0);

    thread_args_t shared_state = (thread_args_t) {
        .data_holder = 0,
        .spinlock = &spinlock
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

    printf("%"PRId64"\n", shared_state.data_holder);
    pthread_spin_destroy(&spinlock);
    return exit_code;
}