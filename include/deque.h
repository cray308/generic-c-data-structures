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
 * Appends `value` to the back of the deque.
 *
 * @param  value  Value to be emplaced.
 */
#define deque_push_back(id, this, value) __dq_push_back_##id(this, value)


/**
 * Removes the last element in the deque, if it is not empty.
 */
#define deque_pop_back(id, this) __dq_pop_back_##id(this)


/**
 * Places `value` in the front of the deque.
 *
 * @param  value  Value to be emplaced.
 */
#define deque_push_front(id, this, value) __dq_push_front_##id(this, value)


/**
 * Generates `Deque` code for a specified type and ID.
 *
 * @param   id           ID to be used for the deque (must be unique).
 * @param   t            Type to be stored in the deque.
 * @param   copyValue    Macro of the form (x, y) which copies y into x to store the element in the deque.
 *                         - If no special copying is required, pass DSDefault_shallowCopy.
 *                         - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param   deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                         - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                         - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_deque(id, t, copyValue, deleteValue) __setup_deque(id, t, Deque_##id, copyValue, deleteValue)

#define __setup_deque(id, t, TypeName, copyValue, deleteValue)                                               \
                                                                                                             \
typedef struct {                                                                                             \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        size_t size, cap;                                                                                    \
    } front;                                                                                                 \
    struct {                                                                                                 \
        t *arr;                                                                                              \
        size_t size, cap;                                                                                    \
    } back;                                                                                                  \
    struct {                                                                                                 \
        size_t frontStart, backStart;                                                                        \
    } pointers;                                                                                              \
} TypeName;                                                                                                  \
                                                                                                             \
__DS_FUNC_PREFIX TypeName *__dq_new_##id(void) {                                                             \
    TypeName *q = __ds_malloc(sizeof(TypeName));                                                             \
    q->front.arr = __ds_malloc(8 * sizeof(t));                                                               \
    q->back.arr = __ds_malloc(8 * sizeof(t));                                                                \
    q->front.cap = q->back.cap = 8;                                                                          \
    q->front.size = q->back.size = q->pointers.frontStart = q->pointers.backStart = 0;                       \
    return q;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_free_##id(TypeName *this) {                                                       \
    size_t i;                                                                                                \
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
__DS_FUNC_PREFIX void __dq_pop_front_##id(TypeName *this) {                                                  \
    if (__deque_frontCount(this)) {                                                                          \
        deleteValue(this->front.arr[this->front.size - 1]);                                                  \
        --this->front.size;                                                                                  \
    } else if (__deque_backCount(this)) {                                                                    \
        deleteValue(this->back.arr[this->pointers.backStart]);                                               \
        ++this->pointers.backStart;                                                                          \
        if (this->back.size > 32 && this->pointers.backStart > (this->back.size >> 1)) {                     \
            size_t half = this->back.cap >> 1;                                                               \
            memmove(this->back.arr, this->back.arr + this->pointers.backStart, (this->back.size - this->pointers.backStart) * sizeof(t)); \
            this->back.size -= this->pointers.backStart;                                                     \
            this->pointers.backStart = 0;                                                                    \
            if (half > 8 && this->back.size < half) {                                                        \
                t *tmp = __ds_realloc(this->back.arr, half * sizeof(t));                                     \
                this->back.arr = tmp;                                                                        \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_push_back_##id(TypeName *this, t item) {                                          \
    t* loc;                                                                                                  \
    if (this->back.size == this->back.cap) {                                                                 \
        t *tmp;                                                                                              \
        this->back.cap <<= 1;                                                                                \
        tmp = __ds_realloc(this->back.arr, this->back.cap * sizeof(t));                                      \
        this->back.arr = tmp;                                                                                \
    }                                                                                                        \
    loc = &this->back.arr[this->back.size];                                                                  \
    copyValue((*loc), item);                                                                                 \
    ++this->back.size;                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_pop_back_##id(TypeName *this) {                                                   \
    if (__deque_backCount(this)) {                                                                           \
        deleteValue(this->back.arr[this->back.size - 1]);                                                    \
        --this->back.size;                                                                                   \
    } else if (__deque_frontCount(this)) {                                                                   \
        deleteValue(this->front.arr[this->pointers.frontStart]);                                             \
        ++this->pointers.frontStart;                                                                         \
        if (this->front.size > 32 && this->pointers.frontStart > (this->front.size >> 1)) {                  \
            size_t half = this->front.cap >> 1;                                                              \
            memmove(this->front.arr, this->front.arr + this->pointers.frontStart, (this->front.size - this->pointers.frontStart) * sizeof(t)); \
            this->front.size -= this->pointers.frontStart;                                                   \
            this->pointers.frontStart = 0;                                                                   \
            if (half > 8 && this->front.size < half) {                                                       \
                t *tmp = __ds_realloc(this->front.arr, half * sizeof(t));                                    \
                this->front.arr = tmp;                                                                       \
                this->back.cap = half;                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_push_front_##id(TypeName *this, t item) {                                         \
    t* loc;                                                                                                  \
    if (this->front.size == this->front.cap) {                                                               \
        t *tmp;                                                                                              \
        this->front.cap <<= 1;                                                                               \
        tmp = __ds_realloc(this->front.arr, this->front.cap * sizeof(t));                                    \
        this->front.arr = tmp;                                                                               \
    }                                                                                                        \
    loc = &this->front.arr[this->front.size];                                                                \
    copyValue((*loc), item);                                                                                 \
    ++this->front.size;                                                                                      \
}                                                                                                            \

#endif
