#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>
#include <stdatomic.h>

// log_printf - макрос для отладочного вывода, добавляющий время со старта программы, имя функции и номер строки
uint64_t start_time_msec;

void __attribute__ ((constructor)) start_time_setter () {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    start_time_msec = spec.tv_sec * 1000L + spec.tv_nsec / 1000000;
}

const char *log_prefix (const char *func, int line) {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    int delta_msec = spec.tv_sec * 1000L + spec.tv_nsec / 1000000 - start_time_msec;
    const int max_func_len = 13;
    static __thread char prefix[100];
    sprintf(prefix, "%d.%03d %*s():%-3d [tid=%ld]", delta_msec / 1000, delta_msec % 1000, max_func_len, func, line,
            (long) pthread_self());
    return prefix;
}

#define log_printf_impl(fmt, ...) { time_t t = time(0); dprintf(2, "%s: " fmt "%s", log_prefix(__FUNCTION__, __LINE__), __VA_ARGS__); }
// Format: <time_since_start> <func_name>:<line> : <custom_message>
#define log_printf(...) log_printf_impl(__VA_ARGS__, "")


#define fail_with_strerror(code, msg) do { char err_buf[1024]; strerror_r(code, err_buf, sizeof(err_buf));\
    log_printf(msg " (From err code: %s)\n", err_buf);  exit(EXIT_FAILURE);} while (0)

// thread-aware assert
#define ta_verify(stmt) do { if (stmt) break; fail_with_strerror(errno, "'" #stmt "' failed."); } while (0)

// verify pthread call
#define pt_verify(pthread_call) do { int code = (pthread_call); if (code == 0) break; \
    fail_with_strerror(code, "'" #pthread_call "' failed."); } while (0)


typedef struct {
  // рекомендую порядок записи переменных:
  pthread_mutex_t mutex; // мьютекс
  pthread_cond_t condvar; // переменная условия (если нужна)

  int value;
} promise_t;

void promise_init (promise_t *promise) {
    pthread_mutex_init(&promise->mutex, NULL);
    pthread_cond_init(&promise->condvar, NULL);
    promise->value = -1;
}

void promise_set (promise_t *promise, int value) {
    pthread_mutex_lock(&promise->mutex); // try comment lock&unlock out and look at result
    promise->value = value; // криитическую секцию стоит делать как можно меньше
    pthread_mutex_unlock(&promise->mutex);
    // Важно понимать, когда вы уведомляете ожидающий поток о наступившем событии
    // скорее всего вам подойдет вариант сделать это сразу после unlock
    // и скорее всего вам не нужно,
    // чтобы между сохранением полезного состояния и cond_signal находилось ожидание чего-либо
    pthread_cond_signal(&promise->condvar); // notify if there was nothing and now will be elements
}

int promise_get (promise_t *promise) {
    pthread_mutex_lock(&promise->mutex); // try comment lock&unlock out and look at result
    while (promise->value == -1) {
        // Ждем какие-либо данные, если их нет, то спим.
        // идейно convar внутри себя разблокирует mutex, чтобы другой поток мог положить в стейт то, что мы ждем
        pthread_cond_wait(&promise->condvar, &promise->mutex);
        // после завершения wait мьютекс снова заблокирован
    }
    int value = promise->value;
    pthread_mutex_unlock(&promise->mutex);
    return value;
}


typedef struct {
  promise_t promise_1;
  promise_t promise_2;
} thread_args_t;

static void*
thread_a_func (void *arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    log_printf("Func A started\n");

    promise_set(&thread_args->promise_1, 42);
    log_printf("Func A set promise_1 with 42\n");
    int value_2 = promise_get(&thread_args->promise_2);
    log_printf("Func A get promise_2 value = %d\n", value_2);

    return NULL;
}

static void*
thread_b_func (void *arg) {
    thread_args_t* thread_args = (thread_args_t*) arg;

    log_printf("Func B started\n");

    int value_1 = promise_get(&thread_args->promise_1);
    log_printf("Func B get promise_1 value = %d\n", value_1);
    promise_set(&thread_args->promise_2, value_1 * 100);
    log_printf("Func B set promise_2 with %d\n", value_1 * 100);

    return NULL;
}


int main () {
    thread_args_t thread_args;

    promise_init(&thread_args.promise_1);
    promise_init(&thread_args.promise_2);

    log_printf("Main func started\n");

    pthread_t thread_a_id;
    log_printf("Creating thread A\n");
    pt_verify(pthread_create(
        &thread_a_id,
        NULL,
        thread_a_func,
        &thread_args
    ));

    pthread_t thread_b_id;
    log_printf("Creating thread B\n");
    pt_verify(pthread_create(
        &thread_b_id,
        NULL,
        thread_b_func,
        &thread_args
    ));

    pt_verify(pthread_join(thread_a_id, NULL));
    log_printf("Thread A joined\n");

    pt_verify(pthread_join(thread_b_id, NULL));
    log_printf("Thread B joined\n");

    log_printf("Main func finished\n");
    return 0;
}