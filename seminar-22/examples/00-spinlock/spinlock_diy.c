#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>


/*

 Принцип работы atomic_compare_exchange_* (псевдокод):

        bool atomic_compare_exchange(int* addr, int* expected, int new) {
            if (*addr != *expected) {
                *expected = *addr;
                return false;
            }
            *addr = new;
            return true;
        }

 */
void spinlock_acquire(_Atomic int* lock) {
    int expected = 0;
    // также есть atomic_compare_exchange_weak, который может выдавать иногда ложный false
    // но он быстрее работает и доступен не на всех платформах
    while (!atomic_compare_exchange_strong(lock, &expected, 1)) {
        expected = 0;
    }
}

void spinlock_release(_Atomic int* lock) {
    atomic_fetch_sub(lock, 1);
}


enum {
  WORKER_POOL_SIZE = 2,
  TICS_NUM = 1000000
};

typedef struct {
  _Atomic(int) lock;
  int64_t data_holder;
} thread_args_t;

static void*
thread_func(void* arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    for (int tic = 0; tic < TICS_NUM; ++tic) {
        spinlock_acquire(&thread_args->lock);
        thread_args->data_holder++; // critical zone
        spinlock_release(&thread_args->lock);
    }

    return NULL;
}

int main() {
    thread_args_t shared_state = (thread_args_t) {
        .data_holder = 0,
        .lock = 0
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

    return exit_code;
}