#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "ds.h"

#if UINT_MAX == 0xffffffff
#define DS_DQ_MAX_SIZE 0x7fffffff
#define DS_DQ_SHIFT_THRESHOLD 0x3fffffff
#elif UINT_MAX == 0xffff
#define DS_DQ_MAX_SIZE 0x7fff
#define DS_DQ_SHIFT_THRESHOLD 0x3fff
#endif

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * The number of elements in the deque.
 */
#define deque_size(this)                                                                                     \
        (((this)->front.size - (this)->front.start) +                                                        \
         ((this)->back.size - (this)->back.start))


/**
 * Tests whether there are no elements in the deque.
 */
#define deque_empty(this) !deque_size(this)


/**
 * Pointer to the first element in the deque, if it is not empty.
 */
#define deque_front(this)                                                                                    \
        (((this)->front.size - (this)->front.start) ?                                                        \
            &(this)->front.arr[(this)->front.size - 1] :                                                     \
            (((this)->back.size - (this)->back.start) ?                                                      \
                &(this)->back.arr[(this)->back.start] : NULL))


/**
 * Pointer to the last element in the deque, if it is not empty.
 */
#define deque_back(this)                                                                                     \
        (((this)->back.size - (this)->back.start) ?                                                          \
            &(this)->back.arr[(this)->back.size - 1] :                                                       \
            (((this)->front.size - (this)->front.start) ?                                                    \
                &(this)->front.arr[(this)->front.start] : NULL))

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty deque.
 *
 * @return  Pointer to the newly allocated deque.
 */
#define deque_new(id) __dq_new_##id()


/**
 * Frees the deque and deletes any remaining elements.
 */
#define deque_free(id, this) __dq_free_##id(this)


/**
 * Removes the first element in the deque, if it is not empty.
 */
#define deque_pop_front(id, this) __dq_pop_front_##id(this)


/**
 * Appends @c value to the back of the deque.
 *
 * @param   value  Value to be emplaced.
 *
 * @return         Whether the operation succeeded.
 */
#define deque_push_back(id, this, value) __dq_push_back_##id(this, value)


/**
 * Removes the last element in the deque, if it is not empty.
 */
#define deque_pop_back(id, this) __dq_pop_back_##id(this)


/**
 * Places @c value in the front of the deque.
 *
 * @param   value  Value to be emplaced.
 *
 * @return         Whether the operation succeeded.
 */
#define deque_push_front(id, this, value) __dq_push_front_##id(this, value)


/**
 * Generates @c Deque function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the deque (must be unique).
 * @param  t   Type to be stored in the deque.
 */
#define gen_deque_headers(id, t) __setup_deque_headers(id, t, Deque_##id)


/**
 * Generates @c Deque function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_deque_headers .
 * @param  t            Type used in @c gen_deque_headers .
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the deque.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass
 *                         @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was
 *                       dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass
 *                         @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass
 *                         @c DSDefault_deepDelete here.
 */
#define gen_deque_source(id, t, copyValue, deleteValue)                                                      \
        __setup_deque_source(id, t, Deque_##id, copyValue, deleteValue)

#define __setup_deque_headers(id, t, TypeName)                                                               \
                                                                                                             \
typedef struct {                                                                                             \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        unsigned size, cap, start;                                                                           \
    } front;                                                                                                 \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        unsigned size, cap, start;                                                                           \
    } back;                                                                                                  \
} TypeName;                                                                                                  \
                                                                                                             \
TypeName *__dq_new_##id(void);                                                                               \
void __dq_free_##id(TypeName *this);                                                                         \
void __dq_pop_front_##id(TypeName *this);                                                                    \
unsigned char __dq_push_back_##id(TypeName *this, const t item);                                             \
void __dq_pop_back_##id(TypeName *this);                                                                     \
unsigned char __dq_push_front_##id(TypeName *this, const t item);                                            \

#define __setup_deque_source(id, t, TypeName, copyValue, deleteValue)                                        \
                                                                                                             \
TypeName *__dq_new_##id(void) {                                                                              \
    TypeName *q = malloc(sizeof(TypeName));                                                                  \
    if (!q) return NULL;                                                                                     \
    else if (!(q->front.arr = malloc(8 * sizeof(t)))) {                                                      \
        free(q);                                                                                             \
        return NULL;                                                                                         \
    } else if (!(q->back.arr = malloc(8 * sizeof(t)))) {                                                     \
        free(q->front.arr);                                                                                  \
        free(q);                                                                                             \
        return NULL;                                                                                         \
    }                                                                                                        \
    q->front.cap = q->back.cap = 8;                                                                          \
    q->front.size = q->back.size = q->front.start = q->back.start = 0;                                       \
    return q;                                                                                                \
}                                                                                                            \
                                                                                                             \
void __dq_free_##id(TypeName *this) {                                                                        \
    unsigned i;                                                                                              \
    for (i = this->front.start; i < this->front.size; ++i) {                                                 \
        deleteValue(this->front.arr[i]);                                                                     \
    }                                                                                                        \
    for (i = this->back.start; i < this->back.size; ++i) {                                                   \
        deleteValue(this->back.arr[i]);                                                                      \
    }                                                                                                        \
    free(this->front.arr);                                                                                   \
    free(this->back.arr);                                                                                    \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
void __dq_pop_front_##id(TypeName *this) {                                                                   \
    if (this->front.size - this->front.start) {                                                              \
        deleteValue(this->front.arr[this->front.size - 1]);                                                  \
        --this->front.size;                                                                                  \
    } else if (this->back.size - this->back.start) {                                                         \
        deleteValue(this->back.arr[this->back.start]);                                                       \
        ++this->back.start;                                                                                  \
        if (this->back.size > 32 &&                                                                          \
                this->back.start > (this->back.size >> 1)) {                                                 \
            const unsigned half = this->back.cap >> 1;                                                       \
            memmove(this->back.arr, this->back.arr + this->back.start,                                       \
                    (this->back.size - this->back.start) * sizeof(t));                                       \
            this->back.size -= this->back.start;                                                             \
            this->back.start = 0;                                                                            \
            if (half > 8 && this->back.size < half) {                                                        \
                t *tmp = realloc(this->back.arr, half * sizeof(t));                                          \
                if (!tmp) return;                                                                            \
                this->back.arr = tmp;                                                                        \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
unsigned char __dq_push_back_##id(TypeName *this, const t item) {                                            \
    t *tmp;                                                                                                  \
    unsigned cap = this->back.cap;                                                                           \
    if (this->back.size == cap) {                                                                            \
        if (cap == DS_DQ_MAX_SIZE) return 0;                                                                 \
        else if (cap < DS_DQ_SHIFT_THRESHOLD) cap <<= 1;                                                     \
        else cap = DS_DQ_MAX_SIZE;                                                                           \
        if (!(tmp = realloc(this->back.arr, cap * sizeof(t)))) return 0;                                     \
        this->back.arr = tmp;                                                                                \
        this->back.cap = cap;                                                                                \
    }                                                                                                        \
    copyValue(this->back.arr[this->back.size], item);                                                        \
    ++this->back.size;                                                                                       \
    return 1;                                                                                                \
}                                                                                                            \
                                                                                                             \
void __dq_pop_back_##id(TypeName *this) {                                                                    \
    if (this->back.size - this->back.start) {                                                                \
        deleteValue(this->back.arr[this->back.size - 1]);                                                    \
        --this->back.size;                                                                                   \
    } else if (this->front.size - this->front.start) {                                                       \
        deleteValue(this->front.arr[this->front.start]);                                                     \
        ++this->front.start;                                                                                 \
        if (this->front.size > 32 &&                                                                         \
                this->front.start > (this->front.size >> 1)) {                                               \
            const unsigned half = this->front.cap >> 1;                                                      \
            memmove(this->front.arr, this->front.arr + this->front.start,                                    \
                    (this->front.size - this->front.start) * sizeof(t));                                     \
            this->front.size -= this->front.start;                                                           \
            this->front.start = 0;                                                                           \
            if (half > 8 && this->front.size < half) {                                                       \
                t *tmp = realloc(this->front.arr, half * sizeof(t));                                         \
                if (!tmp) return;                                                                            \
                this->front.arr = tmp;                                                                       \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
unsigned char __dq_push_front_##id(TypeName *this, const t item) {                                           \
    t *tmp;                                                                                                  \
    unsigned cap = this->front.cap;                                                                          \
    if (this->front.size == cap) {                                                                           \
        if (cap == DS_DQ_MAX_SIZE) return 0;                                                                 \
        else if (cap < DS_DQ_SHIFT_THRESHOLD) cap <<= 1;                                                     \
        else cap = DS_DQ_MAX_SIZE;                                                                           \
        if (!(tmp = realloc(this->front.arr, cap * sizeof(t)))) return 0;                                    \
        this->front.arr = tmp;                                                                               \
        this->front.cap = cap;                                                                               \
    }                                                                                                        \
    copyValue(this->front.arr[this->front.size], item);                                                      \
    ++this->front.size;                                                                                      \
    return 1;                                                                                                \
}                                                                                                            \

#endif /* DS_DEQUE_H */
