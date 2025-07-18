#ifndef __RONLOCK_H__
#define __RONLOCK_H__

#include <stdatomic.h>

#include "padding.h"
#define LOCK_ALGORITHM "RON"
#define NEED_CONTEXT 0
#define SUPPORT_WAITING 0
#define CORE 64

union extended_atomic {
    volatile atomic_int wait;
    char padding[128]; // Let cores in the same CCX share the same cache line
}__attribute__ ((aligned (32)));

typedef struct ron_mutex {
#if COND_VAR
    pthread_mutex_t posix_lock;
    char __pad[pad_to_cache_line(sizeof(pthread_mutex_t))];
#endif
    volatile atomic_int locked;
    union extended_atomic wait_array[CORE];
} ron_mutex_t __attribute__((aligned(L_CACHE_LINE_SIZE)));

typedef pthread_cond_t ron_cond_t;
typedef int *ron_context_t;

ron_mutex_t *ron_mutex_create(const pthread_mutexattr_t *attr);
int ron_mutex_lock(ron_mutex_t *impl, ron_context_t *me);
int ron_mutex_trylock(ron_mutex_t *impl, ron_context_t *me);
void ron_mutex_unlock(ron_mutex_t *impl, ron_context_t *me);
int ron_mutex_destroy(ron_mutex_t *lock);
int ron_cond_init(ron_cond_t *cond, const pthread_condattr_t *attr);
int ron_cond_timedwait(ron_cond_t *cond, ron_mutex_t *lock, ron_context_t *me,
                       const struct timespec *ts);
int ron_cond_wait(ron_cond_t *cond, ron_mutex_t *lock, ron_context_t *me);
int ron_cond_signal(ron_cond_t *cond);
int ron_cond_broadcast(ron_cond_t *cond);
int ron_cond_destroy(ron_cond_t *cond);
void ron_thread_start(void);
void ron_thread_exit(void);
void ron_application_init(void);
void ron_application_exit(void);

typedef ron_mutex_t lock_mutex_t;
typedef ron_context_t lock_context_t;
typedef ron_cond_t lock_cond_t;

#define lock_mutex_create ron_mutex_create
#define lock_mutex_lock ron_mutex_lock
#define lock_mutex_trylock ron_mutex_trylock
#define lock_mutex_unlock ron_mutex_unlock
#define lock_mutex_destroy ron_mutex_destroy
#define lock_cond_init ron_cond_init
#define lock_cond_timedwait ron_cond_timedwait
#define lock_cond_wait ron_cond_wait
#define lock_cond_signal ron_cond_signal
#define lock_cond_broadcast ron_cond_broadcast
#define lock_cond_destroy ron_cond_destroy
#define lock_thread_start ron_thread_start
#define lock_thread_exit ron_thread_exit
#define lock_application_init ron_application_init
#define lock_application_exit ron_application_exit
#define lock_init_context ron_init_context

#endif // __SOALOCK_H__
