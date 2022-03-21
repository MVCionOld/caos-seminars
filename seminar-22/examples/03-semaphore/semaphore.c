#include <semaphore.h>   /* sem_init, sem_post, sem_wait */
#include <stdio.h>       /* printf, fprintf, fflush, stdout, stderr */
#include <stdlib.h>      /* exit */
#include <sys/fcntl.h>   /* O_CREAT, O_RDWR */
#include <sys/sysinfo.h> /* get_nproc */
#include <sys/mman.h>    /* mmap, munmap, PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANONYMOUS */
#include <sys/wait.h>    /* waitpid */
#include <unistd.h>      /* sysconf */


typedef double (*function_t) (double);


double *
pmap_process (function_t func, const double *in, size_t count) {
    const size_t proc_amt = (const size_t) get_nprocs();
    const size_t page_size = (const size_t) sysconf(_SC_PAGESIZE);
    size_t array_size = count * sizeof(double);
    if (array_size % page_size > 0) {
        array_size = (array_size / page_size + 1) * page_size;
    }
    double *mapped_out = (double *) mmap(
        /* void*  addr   = */NULL,
        /* size_t len    = */array_size,
        /* int    prot   = */PROT_READ | PROT_WRITE,
        /* int    flags  = */MAP_SHARED | MAP_ANONYMOUS,
        /* int    fd     = */-1,
        /* off_t  offset = */0
    );
    size_t sem_t_size = count * sizeof(sem_t);
    if (sem_t_size % page_size > 0) {
        sem_t_size = (sem_t_size / page_size + 1) * page_size;
    }
    sem_t *semaphore = (sem_t *) mmap(
        /* void*  addr   = */NULL,
        /* size_t len    = */sem_t_size,
        /* int    prot   = */PROT_READ | PROT_WRITE,
        /* int    flags  = */MAP_SHARED | MAP_ANONYMOUS,
        /* int    fd     = */-1,
        /* off_t  offset = */0
    );

    signal(SIGCHLD, SIG_IGN);

    pid_t* pids = (pid_t*) malloc(proc_amt * sizeof(pid_t));
    for (size_t id = 0; id < proc_amt; ++id) {
        pid_t pid;
        sem_init(
            /* sem_t*   sem     =*/&semaphore[id],
            /* int      pshared =*/1,
            /* uint32_t value   =*/0
        );

        if ((pid = fork()) < 0) {
            fprintf(stderr, "FORK ERROR");
            exit(1);
        } else if (!pid) {
            size_t offset = id * count / proc_amt;
            while (offset < ((id + 1) * count) / proc_amt && offset < count) {
                mapped_out[offset] = func(in[offset]);
                ++offset;
            }
            sem_post(&semaphore[id]);
            exit(0);
        } else {
            continue;
        }

        pids[id] = pid;
    }
    for (size_t id = 0; id < proc_amt; ++id) {
        sem_wait(&semaphore[id]);
        sem_destroy(&semaphore[id]);
        waitpid(pids[id], NULL, NULL);
    }
    munmap(semaphore, sem_t_size);
    free(pids);
    return mapped_out;
}

void
pmap_free (double *ptr, size_t count) {
    munmap(ptr, count * sizeof(double));
}


double pow3(double x) {
    return x * x * x;
}

enum {
  ARRAY_SIZE = 1024000
};

int main() {
    double* in = malloc(sizeof(double) * ARRAY_SIZE);
    for (int idx = 0; idx < ARRAY_SIZE; ++idx) {
        in[idx] = (idx % 42) * 1.0;
    }

    double* out = pmap_process(pow3, in, ARRAY_SIZE);

    for (int idx = 0; idx < 100; ++idx) {
        printf("in[%d]^3 = %lf^3 = %lf\n", idx, in[idx], out[idx]);
    }

    pmap_free(out, ARRAY_SIZE);
    free(in);

    return 0;
}
