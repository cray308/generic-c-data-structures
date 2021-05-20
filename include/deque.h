#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "ds.h"

#define __deque_frontCount(q) ((q)->front.size - (q)->pointers.frontStart)
#define __deque_backCount(q) ((q)->back.size - (q)->pointers.backStart)


/**
 * The number of elements in the deque.
 */
#define deque_size(q) ((int)(__deque_frontCount(q) + __deque_backCount(q)))


/**
 * Tests whether there are no elements in the deque.
 */
#define deque_empty(q) (!(deque_size(q)))


/**
 * Pointer to the first element in the deque, if it is not empty.
 */
#define deque_front(q) (__deque_frontCount(q) ? &((q)->front.arr[(q)->front.size - 1]) : (__deque_backCount(q) ? &((q)->back.arr[(q)->pointers.backStart]) : NULL))


/**
 * Pointer to the last element in the deque, if it is not empty.
 */
#define deque_back(q) (__deque_backCount(q) ? &((q)->back.arr[(q)->back.size - 1]) : (__deque_frontCount(q) ? &((q)->front.arr[(q)->pointers.frontStart]) : NULL))


/**
 * Creates a new, empty deque.
 *
 * @return      Pointer to the newly allocated deque.
 */
#define deque_new(id) __dq_new_##id()


/**
 * Frees the deque and deletes any remaining elements.
 */
#define deque_free(id, q) __dq_free_##id(q)


/**
 * Removes the first element in the deque, if it is not empty.
 */
#define deque_pop_front(id, q) __dq_pop_front_##id(q)


/**
 * Appends `value` to the back of the deque.
 *
 * @param  value  Value to be emplaced.
 */
#define deque_push_back(id, q, value) __dq_push_back_##id(q, value)


/**
 * Removes the last element in the deque, if it is not empty.
 */
#define deque_pop_back(id, q) __dq_pop_back_##id(q)


/**
 * Places `value` in the front of the deque.
 *
 * @param  value  Value to be emplaced.
 */
#define deque_push_front(id, q, value) __dq_push_front_##id(q, value)


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
__DS_FUNC_PREFIX void __dq_free_##id(TypeName *q) {                                                          \
    for (size_t i = q->pointers.frontStart; i < q->front.size; ++i) {                                        \
        deleteValue(q->front.arr[i]);                                                                        \
    }                                                                                                        \
    for (size_t i = q->pointers.backStart; i < q->back.size; ++i) {                                          \
        deleteValue(q->back.arr[i]);                                                                         \
    }                                                                                                        \
    free(q->front.arr);                                                                                      \
    free(q->back.arr);                                                                                       \
    free(q);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_pop_front_##id(TypeName *q) {                                                     \
    if (__deque_frontCount(q)) {                                                                             \
        deleteValue(q->front.arr[q->front.size - 1]);                                                        \
        --q->front.size;                                                                                     \
    } else if (__deque_backCount(q)) {                                                                       \
        deleteValue(q->back.arr[q->pointers.backStart]);                                                     \
        ++q->pointers.backStart;                                                                             \
        if (q->back.size > 32 && q->pointers.backStart > (q->back.size >> 1)) {                              \
            memmove(q->back.arr, q->back.arr + q->pointers.backStart, (q->back.size - q->pointers.backStart) * sizeof(t)); \
            q->back.size -= q->pointers.backStart;                                                           \
            q->pointers.backStart = 0;                                                                       \
            size_t half = q->back.cap >> 1;                                                                  \
            if (half > 8 && q->back.size < half) {                                                           \
                t *tmp = __ds_realloc(q->back.arr, half * sizeof(t));                                        \
                q->back.arr = tmp;                                                                           \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_push_back_##id(TypeName *q, t item) {                                             \
    if (q->back.size == q->back.cap) {                                                                       \
        q->back.cap <<= 1;                                                                                   \
        t *tmp = __ds_realloc(q->back.arr, q->back.cap * sizeof(t));                                         \
        q->back.arr = tmp;                                                                                   \
    }                                                                                                        \
    t* loc = &q->back.arr[q->back.size];                                                                     \
    copyValue((*loc), item);                                                                                 \
    ++q->back.size;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_pop_back_##id(TypeName *q) {                                                      \
    if (__deque_backCount(q)) {                                                                              \
        deleteValue(q->back.arr[q->back.size - 1]);                                                          \
        --q->back.size;                                                                                      \
    } else if (__deque_frontCount(q)) {                                                                      \
        deleteValue(q->front.arr[q->pointers.frontStart]);                                                   \
        ++q->pointers.frontStart;                                                                            \
        if (q->front.size > 32 && q->pointers.frontStart > (q->front.size >> 1)) {                           \
            memmove(q->front.arr, q->front.arr + q->pointers.frontStart, (q->front.size - q->pointers.frontStart) * sizeof(t)); \
            q->front.size -= q->pointers.frontStart;                                                         \
            q->pointers.frontStart = 0;                                                                      \
            size_t half = q->front.cap >> 1;                                                                 \
            if (half > 8 && q->front.size < half) {                                                          \
                t *tmp = __ds_realloc(q->front.arr, half * sizeof(t));                                       \
                q->front.arr = tmp;                                                                          \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __dq_push_front_##id(TypeName *q, t item) {                                            \
    if (q->front.size == q->front.cap) {                                                                     \
            q->front.cap <<= 1;                                                                              \
            t *tmp = __ds_realloc(q->front.arr, q->front.cap * sizeof(t));                                   \
            q->front.arr = tmp;                                                                              \
    }                                                                                                        \
    t* loc = &q->front.arr[q->front.size];                                                                   \
    copyValue((*loc), item);                                                                                 \
    ++q->front.size;                                                                                         \
}                                                                                                            \

#endif
