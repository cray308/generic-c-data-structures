#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "ds.h"

#define __deque_frontCount(this) ((this)->front.size - (this)->pointers.frontStart)
#define __deque_backCount(this) ((this)->back.size - (this)->pointers.backStart)


/**
 * The number of elements in the deque.
 */
#define deque_size(this) ((int)(__deque_frontCount(this) + __deque_backCount(this)))


/**
 * Tests whether there are no elements in the deque.
 */
#define deque_empty(this) (!deque_size(this))


/**
 * Pointer to the first element in the deque, if it is not empty.
 */
#define deque_front(this) (__deque_frontCount(this) ? &((this)->front.arr[(this)->front.size - 1]) : (__deque_backCount(this) ? &((this)->back.arr[(this)->pointers.backStart]) : NULL))


/**
 * Pointer to the last element in the deque, if it is not empty.
 */
#define deque_back(this) (__deque_backCount(this) ? &((this)->back.arr[(this)->back.size - 1]) : (__deque_frontCount(this) ? &((this)->front.arr[(this)->pointers.frontStart]) : NULL))


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
 * @param  value  Value to be emplaced.
 */
#define deque_push_back(id, this, value) __dq_push_back_##id(this, value)


/**
 * Removes the last element in the deque, if it is not empty.
 */
#define deque_pop_back(id, this) __dq_pop_back_##id(this)


/**
 * Places @c value in the front of the deque.
 *
 * @param  value  Value to be emplaced.
 */
#define deque_push_front(id, this, value) __dq_push_front_##id(this, value)


/**
 * Generates @c Deque function declarations for the specified type and ID.
 *
 * @param   id  ID to be used for the deque (must be unique).
 * @param   t   Type to be stored in the deque.
 */
#define gen_deque_headers(id, t) __setup_deque_headers(id, t, Deque_##id)


/**
 * Generates @c Deque function definitions for the specified type and ID.
 *
 * @param   id           ID used in @c gen_deque_headers .
 * @param   t            Type used in @c gen_deque_headers .
 * @param   copyValue    Macro of the form (x, y) which copies y into x to store the element in the deque.
 *                         - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                         - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param   deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                         - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                         - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_deque_source(id, t, copyValue, deleteValue) __setup_deque_source(id, t, Deque_##id, copyValue, deleteValue)

#define __setup_deque_headers(id, t, TypeName)                                                               \
                                                                                                             \
typedef struct {                                                                                             \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        unsigned size, cap;                                                                                  \
    } front;                                                                                                 \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        unsigned size, cap;                                                                                  \
    } back;                                                                                                  \
    struct {                                                                                                 \
        unsigned frontStart, backStart;                                                                      \
    } pointers;                                                                                              \
} TypeName;                                                                                                  \
                                                                                                             \
TypeName *__dq_new_##id(void);                                                                               \
void __dq_free_##id(TypeName *this);                                                                         \
void __dq_pop_front_##id(TypeName *this);                                                                    \
void __dq_push_back_##id(TypeName *this, t item);                                                            \
void __dq_pop_back_##id(TypeName *this);                                                                     \
void __dq_push_front_##id(TypeName *this, t item);                                                           \

#define __setup_deque_source(id, t, TypeName, copyValue, deleteValue)                                        \
                                                                                                             \
TypeName *__dq_new_##id(void) {                                                                              \
    TypeName *q;                                                                                             \
    __ds_malloc(q, sizeof(TypeName))                                                                         \
    __ds_malloc(q->front.arr, 8 * sizeof(t))                                                                 \
    __ds_malloc(q->back.arr, 8 * sizeof(t))                                                                  \
    q->front.cap = q->back.cap = 8;                                                                          \
    q->front.size = q->back.size = q->pointers.frontStart = q->pointers.backStart = 0;                       \
    return q;                                                                                                \
}                                                                                                            \
                                                                                                             \
void __dq_free_##id(TypeName *this) {                                                                        \
    unsigned i;                                                                                              \
    for (i = this->pointers.frontStart; i < this->front.size; ++i) {                                         \
        deleteValue(this->front.arr[i]);                                                                     \
    }                                                                                                        \
    for (i = this->pointers.backStart; i < this->back.size; ++i) {                                           \
        deleteValue(this->back.arr[i]);                                                                      \
    }                                                                                                        \
    free(this->front.arr);                                                                                   \
    free(this->back.arr);                                                                                    \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
void __dq_pop_front_##id(TypeName *this) {                                                                   \
    if (__deque_frontCount(this)) {                                                                          \
        deleteValue(this->front.arr[this->front.size - 1]);                                                  \
        --this->front.size;                                                                                  \
    } else if (__deque_backCount(this)) {                                                                    \
        deleteValue(this->back.arr[this->pointers.backStart]);                                               \
        ++this->pointers.backStart;                                                                          \
        if (this->back.size > 32 && this->pointers.backStart > (this->back.size >> 1)) {                     \
            unsigned half = this->back.cap >> 1;                                                             \
            memmove(this->back.arr, this->back.arr + this->pointers.backStart, (this->back.size - this->pointers.backStart) * sizeof(t)); \
            this->back.size -= this->pointers.backStart;                                                     \
            this->pointers.backStart = 0;                                                                    \
            if (half > 8 && this->back.size < half) {                                                        \
                  t *tmp;                                                                                    \
                __ds_realloc(tmp, this->back.arr, half * sizeof(t))                                          \
                this->back.arr = tmp;                                                                        \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void __dq_push_back_##id(TypeName *this, t item) {                                                           \
    t* loc;                                                                                                  \
    if (this->back.size == this->back.cap) {                                                                 \
        t *tmp;                                                                                              \
        this->back.cap <<= 1;                                                                                \
        __ds_realloc(tmp, this->back.arr, this->back.cap * sizeof(t))                                        \
        this->back.arr = tmp;                                                                                \
    }                                                                                                        \
    loc = &this->back.arr[this->back.size];                                                                  \
    copyValue((*loc), item);                                                                                 \
    ++this->back.size;                                                                                       \
}                                                                                                            \
                                                                                                             \
void __dq_pop_back_##id(TypeName *this) {                                                                    \
    if (__deque_backCount(this)) {                                                                           \
        deleteValue(this->back.arr[this->back.size - 1]);                                                    \
        --this->back.size;                                                                                   \
    } else if (__deque_frontCount(this)) {                                                                   \
        deleteValue(this->front.arr[this->pointers.frontStart]);                                             \
        ++this->pointers.frontStart;                                                                         \
        if (this->front.size > 32 && this->pointers.frontStart > (this->front.size >> 1)) {                  \
            unsigned half = this->front.cap >> 1;                                                            \
            memmove(this->front.arr, this->front.arr + this->pointers.frontStart, (this->front.size - this->pointers.frontStart) * sizeof(t)); \
            this->front.size -= this->pointers.frontStart;                                                   \
            this->pointers.frontStart = 0;                                                                   \
            if (half > 8 && this->front.size < half) {                                                       \
                  t *tmp;                                                                                    \
                __ds_realloc(tmp, this->front.arr, half * sizeof(t))                                         \
                this->front.arr = tmp;                                                                       \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void __dq_push_front_##id(TypeName *this, t item) {                                                          \
    t* loc;                                                                                                  \
    if (this->front.size == this->front.cap) {                                                               \
        t *tmp;                                                                                              \
        this->front.cap <<= 1;                                                                               \
        __ds_realloc(tmp, this->front.arr, this->front.cap * sizeof(t))                                      \
        this->front.arr = tmp;                                                                               \
    }                                                                                                        \
    loc = &this->front.arr[this->front.size];                                                                \
    copyValue((*loc), item);                                                                                 \
    ++this->front.size;                                                                                      \
}                                                                                                            \

#endif
