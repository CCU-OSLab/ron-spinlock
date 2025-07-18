#ifndef __RONTICK_H__
#define __RONTICK_H__

#include <stdatomic.h>
#include <stdalign.h>

#include "padding.h"
#define LOCK_ALGORITHM "RONTICK"
#define NEED_CONTEXT 0
#define SUPPORT_WAITING 0
#define CORE 64

typedef struct ticketLock{
    union{
        atomic_llong grant_ticket;
        struct{
            int grant;
            int ticket;
        };
    };
}ticketLock_t __attribute__ ((aligned(8)));


typedef struct soa_mutex {
#if COND_VAR
    pthread_mutex_t posix_lock;
    char __pad[pad_to_cache_line(sizeof(pthread_mutex_t))];
#endif
    ticketLock_t arrayLock[CORE];
    volatile atomic_int wait;
} soa_mutex_t __attribute__((aligned(64)));

typedef pthread_cond_t soa_cond_t;
typedef int *soa_context_t;

soa_mutex_t *soa_mutex_create(const pthread_mutexattr_t *attr);
int soa_mutex_lock(soa_mutex_t *impl, soa_context_t *me);
int soa_mutex_trylock(soa_mutex_t *impl, soa_context_t *me);
void soa_mutex_unlock(soa_mutex_t *impl, soa_context_t *me);
int soa_mutex_destroy(soa_mutex_t *lock);
int soa_cond_init(soa_cond_t *cond, const pthread_condattr_t *attr);
int soa_cond_timedwait(soa_cond_t *cond, soa_mutex_t *lock, soa_context_t *me,
                       const struct timespec *ts);
int soa_cond_wait(soa_cond_t *cond, soa_mutex_t *lock, soa_context_t *me);
int soa_cond_signal(soa_cond_t *cond);
int soa_cond_broadcast(soa_cond_t *cond);
int soa_cond_destroy(soa_cond_t *cond);
void soa_thread_start(void);
void soa_thread_exit(void);
void soa_application_init(void);
void soa_application_exit(void);

typedef soa_mutex_t lock_mutex_t;
typedef soa_context_t lock_context_t;
typedef soa_cond_t lock_cond_t;

#define lock_mutex_create soa_mutex_create
#define lock_mutex_lock soa_mutex_lock
#define lock_mutex_trylock soa_mutex_trylock
#define lock_mutex_unlock soa_mutex_unlock
#define lock_mutex_destroy soa_mutex_destroy
#define lock_cond_init soa_cond_init
#define lock_cond_timedwait soa_cond_timedwait
#define lock_cond_wait soa_cond_wait
#define lock_cond_signal soa_cond_signal
#define lock_cond_broadcast soa_cond_broadcast
#define lock_cond_destroy soa_cond_destroy
#define lock_thread_start soa_thread_start
#define lock_thread_exit soa_thread_exit
#define lock_application_init soa_application_init
#define lock_application_exit soa_application_exit
#define lock_init_context soa_init_context

#endif // __SOALOCK_H__
