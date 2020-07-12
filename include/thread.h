#ifndef DS_THREAD_H
#define DS_THREAD_H

#include <pthread.h>
#include <stdio.h>

/* --------------------------------------------------------------------------
 * Credit goes to Peter Bui at the University of Notre Dame for these thread macros.
 * -------------------------------------------------------------------------- */

/* Macros */

#define PTHREAD_CHECK(f) \
    do { \
        int _rc; \
        if ((_rc = (f)) != 0) { \
            fprintf(stderr, "%s\n", strerror(_rc)); \
            exit(1); \
        } \
    } while (0)

/* Thread */

typedef pthread_t                   Thread;
#define thread_create(t, a, f, v)   PTHREAD_CHECK(pthread_create(t, a, f, v))
#define thread_join(t, r)           PTHREAD_CHECK(pthread_join(t, r))
#define thread_detach(t)            PTHREAD_CHECK(pthread_detach(t))

/* Mutex */

typedef pthread_mutex_t		    Mutex;
#define mutex_init(l, a)            PTHREAD_CHECK(pthread_mutex_init(l, a))
#define mutex_lock(l)               PTHREAD_CHECK(pthread_mutex_lock(l))
#define mutex_unlock(l)             PTHREAD_CHECK(pthread_mutex_unlock(l))

/* Condition Variables */

typedef pthread_cond_t              Cond;
#define cond_init(c, a)             PTHREAD_CHECK(pthread_cond_init(c, a))
#define cond_wait(c, l)             PTHREAD_CHECK(pthread_cond_wait(c, l))
#define cond_signal(c)              PTHREAD_CHECK(pthread_cond_signal(c))

#endif /* DS_THREAD_H */
