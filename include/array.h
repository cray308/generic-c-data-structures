#ifndef ARRAY_H
#define ARRAY_H

#include "iterator.h"
#include "alg_helper.h"

#define __ARRAY_INITIAL_CAPACITY 8
#define ARRAY_ERROR (-1)
#define ARRAY_END(a) ((int) (a)->size)

typedef enum {
    ARR_INIT_EMPTY,
    ARR_INIT_SIZE, /* set array to start with a certain size */
    ARR_INIT_BUILTIN, /* like int nums[] = {1, 2, 3} */
} ArrayInitializer;

typedef enum {
    ARR_INSERT_SINGLE, /* like int */
    ARR_INSERT_BUILTIN, /* like int nums[] = {1, 2, 3} */
} ArrayInsertType;

typedef enum {
    ARR_RESIZE_EMPTY,
    ARR_RESIZE_VALUE
} ArrayResizeType;

#define __init_array(id) array_new_##id(ARR_INIT_EMPTY)
#define __insert_single_array array_push_back
#define __insert_multi_1_array(id) __arr_insert_builtin_##id(d_new, ARRAY_END(d_new), first1, (last1 - first1))
#define __insert_multi_2_array(id) __arr_insert_builtin_##id(d_new, ARRAY_END(d_new), first2, (last2 - first2))

/* --------------------------------------------------------------------------
 * PRIMARY ARRAY SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at index i. Performs bounds checking, and if the index is out of bounds,
 *  returns NULL.
 *
 * @param   id  ID used with gen_array.
 * @param   a   Pointer to array.
 * @param   i   The index in the array.
 */
#define array_at(id, a, i) array_at_##id(a, i)


/**
 * Direct access to the element at index i. Does NOT perform bounds checking, but faster than
 *   array_at for element access.
 *
 * @param   a  Pointer to array.
 * @param   i  The index in the array.
 */
#define array_index(a, i) ((a)->arr[(i)])


/**
 * Pointer to the first element in the array, if the array is not empty.
 *
 * @param   a  Pointer to array.
 */
#define array_front(a) iter_begin(ARR, 0, (a)->arr, (a)->size)


/**
 * Pointer to the last element in the array, if the array is not empty.
 *
 * @param   a  Pointer to array.
 */
#define array_back(a) iter_rbegin(ARR, 0, (a)->arr, (a)->size)


/**
 * Returns a pointer to the specified index, or NULL if the index is out of bounds. Negative
 *   indices may be used.
 *
 * @param   id  ID used with gen_array.
 * @param   a   Pointer to array.
 * @param   i   Index in array.
 */
#define array_iterator(id, a, i) array_iterator_##id(a, i)


/**
 * Returns a pointer to AFTER index (array_size - 1) - useful as a terminating condition for
 * iterating over the array using pointers.
 *
 * @param   a  Pointer to array.
 */
#define array_end(a) iter_end(ARR, 0, (a)->arr, (a)->size)


/**
 * Macro for iterating over the array from front to back.
 *
 * @param   a     Pointer to array.
 * @param   ptr   ArrayIterator (t *) which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(ARR, ptr) or *ptr.
 */
#define array_iter(a, ptr)                                                                                   \
    for (ptr = array_front(a); ptr != array_end(a); iter_next(ARR, 0, ptr))                                  \


/**
 * Macro for iterating over the array in reverse (from back to front).
 *
 * @param   a     Pointer to array.
 * @param   ptr   ArrayIterator (t *) which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(ARR, ptr) or *ptr.
 */
#define array_riter(a, ptr)                                                                                  \
    for (ptr = array_back(a); ptr != iter_rend(ARR, 0, (a)->arr, (a)->size); iter_prev(ARR, 0, ptr))         \


/**
 * The number of elements in the array.
 *
 * @param   a  Pointer to array.
 */
#define array_size(a) ((int) (a)->size)


/**
 * The capacity (maximum number of elements prior to resizing) of the array.
 *
 * @param   a  Pointer to array.
 */
#define array_capacity(a) ((int) (a)->capacity)


/**
 * Tests whether there are no elements in the array.
 *
 * @param   a  Pointer to array.
 */
#define array_empty(a) (!((a)->size))


/**
 * Creates a new array.
 * In (1), an empty Array is created.
 * In (2), an Array is initialized to size n, with each element equal to "value".
 * In (3), an Array is initialized from a built-in array data type, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total. The Array type in this file can be used
 *   by passing array_iterator(id, a, index).
 * 
 * (1) init = ARR_INIT_EMPTY:    array_new(id, ArrayInitializer init)
 * (2) init = ARR_INIT_SIZE:     array_new(id, ArrayInitializer init, int size, t value)
 * (3) init = ARR_INIT_BUILTIN:  array_new(id, ArrayInitializer init, t arr[], int n)
 *
 * @param   id      ID used with gen_array.
 * @param   helper  Pointer to DSHelper struct.
 * @param   init    Type of initializer to execute.
 *
 * @return          Pointer to the newly created array.
 */
#define array_new(id, init, ...) array_new_##id(init, ##__VA_ARGS__)


/**
 * Deletes all elements and frees the array.
 *
 * @param  id   ID used with gen_array.
 * @param  a    Pointer to array.
 */
#define array_free(id, a) array_free_##id(a)


/**
 * Resizes the array to a size of n. If n is less than the current size, all but the first n
 *   elements are removed. If n is greater than the current size, then:
 * In (1), elements are appended to the array with a value of 0, using memset.
 * In (2), elements are appended to the array as copies of "value".
 *
 * (1) type = ARR_RESIZE_EMPTY:   array_resize(id, Array_id *a, ArrayResizeType type, size_t n)
 * (2) type = ARR_RESIZE_VALUE:   array_resize(id, Array_id *a, ArrayResizeType type, size_t n, t value)
 *
 * @param  id     ID used with gen_array.
 * @param  a      Pointer to array.
 * @param  type   Type of resize to perform.
 * @param  n      The new array size.
 */
#define array_resize(id, a, type, n, ...) array_resize_##id(a, type, n, ##__VA_ARGS__)


/**
 * Request a change in capacity. If this is smaller than the current capacity, nothing is done. If
 *   you wish to decrease the capacity, see array_shrink_to_fit.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 * @param  n   The new capacity.
 */
#define array_reserve(id, a, n) array_reserve_##id(a, n)


/**
 * Appends a new element to the array.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 * @param  e   Element to be inserted, of the same type used in gen_array.
 */
#define array_push_back(id, a, e) array_push_back_##id(a, e)


/**
 * Inserts new elements BEFORE the specified index. Any elements after this index
 * will be shifted one position to the right. After insertion, the first of the new elements
 * (if there are multiple) will be located at the specified index. If the index is equal to the
 * array's size, this is equivalent to a push_back operation.
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total. The Array type in this file can be used
 *   by passing array_iterator(id, a, index).
 * 
 * (1) type = ARR_INSERT_SINGLE:   array_insert(id, Array_id *a, int index, ArrayInsertType type, t value)
 * (2) type = ARR_INSERT_BUILTIN:  array_insert(id, Array_id *a, int index, ArrayInsertType type, t arr[], int n)
 *
 * @param  id     ID used with gen_array.
 * @param  a      Pointer to Array.
 * @param  index  Index before which the element(s) will be inserted. If this is specified as
 *                  ARRAY_END(a), this is equivalent to push_back.
 * @param  type   Type of insertion to execute.
 *
 * @return        The index where the first item was inserted, or ARRAY_ERROR if there was
 *                  an error.
 */
#define array_insert(id, a, index, type, ...) array_insert_##id(a, index, type, ##__VA_ARGS__)


/**
 * Removes the last element if the array is not empty.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 */
#define array_pop_back(id, a) array_pop_back_##id(a)


/**
 * Removes all elements, leaving the array with a size of 0.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 */
#define array_clear(id, a) array_clear_##id(a)


/**
 * Removes "nelem" elements from the array, starting at index "first".
 *
 * @param  id     ID used with gen_array.
 * @param  a      Pointer to array.
 * @param  first  The first index to delete.
 * @param  nelem  The number of elements to delete. If this is -1, it means to erase all elements
 *                  from "first" to the end of the array.
 *
 * @return        The index after the last element to be deleted. If the last element to be deleted
 *                    was the end of the array, returns ARRAY_END(a). If an error occurred, returns
 *                    ARRAY_ERROR.
 */
#define array_erase(id, a, first, nelem) array_erase_##id(a, first, nelem)


/**
 * If the array's capacity is greater than its size, it will reallocate only enough memory to hold
 *   the array's size number of elements. This should only be used if the array's capacity has
 *   grown to be much larger than its size.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 */
#define array_shrink_to_fit(id, a) array_shrink_to_fit_##id(a)


/**
 * Creates a subarray from "a" with "n" elements, starting at index "start" and moving to
 * the next element to include with a step size of "step_size".
 *
 * @param   id         ID used with gen_array.
 * @param   start      Index to start the subarray.
 * @param   n          Maximum number of elements to include in the new subarray. -1 implies to
 *                       include as many elements as the start and step size allow.
 * @param   step_size  How to adjust the index when copying elements. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return             A newly allocated subarray, or NULL if an error occurred.
 */
#define array_subarr(id, a, start, n, step_size) array_subarr_##id(a, start, n, step_size)


/**
 * Generates array code for a specified type and ID.
 *
 * @param   id  ID to be used for the type stored in the array (must be unique).
 * @param   t   Type to be stored in the array.
 */
#define gen_array(id, t)                                                                                     \
__gen_iter_ARR(id, t)                                                                                        \
__gen_arr_declarations(id, t)                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* array_at_##id(Array_##id *a, int i) {                                                \
    int _idx = modulo(i, a->size);                                                                           \
    if (_idx < 0) {                                                                                          \
        return NULL;                                                                                         \
    }                                                                                                        \
    return a->arr + _idx;                                                                                    \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *array_iterator_##id(Array_##id *a, int i) {                                          \
    int _idx = modulo(i, a->size);                                                                           \
    return (_idx < 0) ? NULL : &(a->arr[_idx]);                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_new_##id(ArrayInitializer init, ...) {                                    \
    Array_##id *a = malloc(sizeof(Array_##id));                                                              \
    if (!a) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
                                                                                                             \
    memset(a, 0, sizeof(Array_##id));                                                                        \
    array_reserve_##id(a, __ARRAY_INITIAL_CAPACITY);                                                         \
    if (init == ARR_INIT_EMPTY) { /* nothing more to do in this case */                                      \
        return a;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    int n = 0;                                                                                               \
    t value = (t) 0;                                                                                         \
    void *arr;                                                                                               \
                                                                                                             \
    /* parse arguments */                                                                                    \
    va_list args;                                                                                            \
    va_start(args, init);                                                                                    \
                                                                                                             \
    if (init == ARR_INIT_SIZE) {                                                                             \
        n = va_arg(args, int);                                                                               \
        value = (t) (long) va_arg(args, void *);                                                             \
    } else {                                                                                                 \
        arr = va_arg(args, void *);                                                                          \
        n = va_arg(args, int);                                                                               \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    switch (init) {                                                                                          \
        case ARR_INIT_SIZE:                                                                                  \
            array_resize_##id(a, ARR_RESIZE_VALUE, n, value);                                                \
            break;                                                                                           \
        default:                                                                                             \
            __arr_insert_builtin_##id(a, 0, (t *) arr, n);                                                   \
            break;                                                                                           \
    }                                                                                                        \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_free_##id(Array_##id *a) {                                                   \
    if (a->capacity) {                                                                                       \
        array_erase_##id(a, 0, ARRAY_END(a));                                                                \
        free(a->arr);                                                                                        \
    }                                                                                                        \
    free(a);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void array_resize_##id(Array_##id *a, ArrayResizeType type, size_t n, ...) {                \
    size_t i;                                                                                                \
    if (n <= a->size) {                                                                                      \
        a->size = n;                                                                                         \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(a, n);                                                                                \
                                                                                                             \
    t value;                                                                                                 \
                                                                                                             \
    va_list args;                                                                                            \
    va_start(args, n);                                                                                       \
                                                                                                             \
    if (type == ARR_RESIZE_VALUE) {                                                                          \
        value = (t) (long) va_arg(args, void *);                                                             \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    switch (type) {                                                                                          \
        case ARR_RESIZE_EMPTY: ;                                                                             \
            size_t len = n - a->size;                                                                        \
            memset(&a->arr[a->size], 0, len * sizeof(t));                                                    \
            break;                                                                                           \
        case ARR_RESIZE_VALUE:                                                                               \
            for (i = a->size; i < n; ++i) {                                                                  \
                a->arr[i] = value;                                                                           \
            }                                                                                                \
            break;                                                                                           \
    }                                                                                                        \
    a->size = n;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void array_reserve_##id(Array_##id *a, size_t n) {                                          \
    if (n <= a->capacity) { /* don't realloc smaller than the current capacity */                            \
        return;                                                                                              \
    }                                                                                                        \
    size_t ncap = a->capacity ? a->capacity : 1;                                                             \
    while (ncap < n) {                                                                                       \
        ncap <<= 1; /* use multiple of 2 */                                                                  \
    }                                                                                                        \
                                                                                                             \
    t *tmp = realloc(a->arr, ncap * sizeof(t));                                                              \
    if (!tmp) {                                                                                              \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    a->capacity = ncap;                                                                                      \
    a->arr = tmp;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_push_back_##id(Array_##id *a, t value) {                                     \
    array_reserve_##id(a, a->size + 1); /* allocate more space */                                            \
    *(a->arr + a->size) = value;                                                                             \
    a->size++;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_insert_##id(Array_##id *a, int index, ArrayInsertType type, ...) {                \
    int n;                                                                                                   \
    void *other;                                                                                             \
    t value;                                                                                                 \
                                                                                                             \
    /* parse arguments */                                                                                    \
    va_list args;                                                                                            \
    va_start(args, type);                                                                                    \
                                                                                                             \
    if (type == ARR_INSERT_SINGLE) {                                                                         \
        value = (t) (long) va_arg(args, void *);                                                             \
    } else {                                                                                                 \
        other = va_arg(args, void *);                                                                        \
        n = va_arg(args, int);                                                                               \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    int rv = 0;                                                                                              \
                                                                                                             \
    switch (type) {                                                                                          \
        case ARR_INSERT_SINGLE:                                                                              \
            rv = __arr_insert_elem_##id(a, index, value);                                                    \
            break;                                                                                           \
        default:                                                                                             \
            rv = __arr_insert_builtin_##id(a, index, (t *) other, n);                                        \
            break;                                                                                           \
    }                                                                                                        \
                                                                                                             \
    return rv;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_pop_back_##id(Array_##id *a) {                                               \
    if (!(a->size)) {                                                                                        \
        return;                                                                                              \
    }                                                                                                        \
    a->size--;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_clear_##id(Array_##id *a) {                                                  \
    array_erase_##id(a, 0, ARRAY_END(a));                                                                    \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_erase_##id(Array_##id *a, int first, int nelem) {                                 \
    if (!nelem || a->size == 0) {                                                                            \
        return ARRAY_ERROR;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    first = modulo(first, a->size);                                                                          \
    if (first < 0) {                                                                                         \
        return ARRAY_ERROR;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    if (nelem < 0) { /* erase from first to end of array */                                                  \
        nelem = (int) a->size - first;                                                                       \
    } else {                                                                                                 \
        nelem = min(nelem, (int) a->size - first);                                                           \
    }                                                                                                        \
                                                                                                             \
    int endIdx = first + nelem;                                                                              \
    int res;                                                                                                 \
                                                                                                             \
    if (endIdx < (int) a->size) { /* move elements from endIdx onward back to first */                       \
        memmove(a->arr + first, a->arr + endIdx, (a->size - endIdx) * sizeof(t));                            \
        res = first;                                                                                         \
    } else {                                                                                                 \
        res = ARRAY_END(a) - nelem;                                                                          \
    }                                                                                                        \
    a->size -= (size_t) nelem;                                                                               \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_shrink_to_fit_##id(Array_##id *a) {                                          \
    if (a->size == a->capacity || a->size == 0) { /* can't shrink any more */                                \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    t *tmp = realloc(a->arr, a->size * sizeof(t));                                                           \
    if (!tmp) {                                                                                              \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    a->capacity = a->size;                                                                                   \
    a->arr = tmp;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_subarr_##id(Array_##id *a, int start, int n, int step_size) {             \
    if (!a->size || !n) {                                                                                    \
        return NULL;                                                                                         \
    } else if (step_size == 0) {                                                                             \
        step_size = 1;                                                                                       \
    }                                                                                                        \
                                                                                                             \
    start = modulo(start, a->size);                                                                          \
    if (start < 0) {                                                                                         \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Array_##id *sub = array_new_##id(ARR_INIT_EMPTY);                                                        \
    int end;                                                                                                 \
    int i;                                                                                                   \
                                                                                                             \
    if (step_size < 0) {                                                                                     \
        end = (n < 0) ? -1 : max(-1, start + (n * step_size));                                               \
        for (i = start; i > end; i += step_size) {                                                           \
            array_push_back_##id(sub, a->arr[i]);                                                            \
        }                                                                                                    \
    } else {                                                                                                 \
        end = (n < 0) ? ARRAY_END(a) : min(ARRAY_END(a), start + (n * step_size));                           \
        for (i = start; i < end; i += step_size) {                                                           \
            array_push_back_##id(sub, a->arr[i]);                                                            \
        }                                                                                                    \
    }                                                                                                        \
    return sub;                                                                                              \
}                                                                                                            \
__gen_arr_helper_funcs(id, t)                                                                                \

#define __gen_arr_declarations(id, t)                                                                        \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t capacity;                                                                                         \
    t *arr;                                                                                                  \
} Array_##id;                                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX int __arr_insert_elem_##id(Array_##id *a, int index, t value);                              \
__DS_FUNC_PREFIX int __arr_insert_builtin_##id(Array_##id *a, int index, t *arr, int nelem);                 \
__DS_FUNC_PREFIX void array_reserve_##id(Array_##id *a, size_t n);                                           \
__DS_FUNC_PREFIX void array_resize_##id(Array_##id *a, ArrayResizeType type, size_t n, ...);                 \
__DS_FUNC_PREFIX void array_clear_##id(Array_##id *a);                                                       \
__DS_FUNC_PREFIX int array_erase_##id(Array_##id *a, int first, int nelem);                                  \

#define __gen_arr_helper_funcs(id, t)                                                                        \
__DS_FUNC_PREFIX int __arr_insert_elem_##id(Array_##id *a, int index, t value) {                             \
    if (!a->size || index >= (int) a->size) { /* append */                                                   \
        array_push_back_##id(a, value);                                                                      \
        return a->size - 1;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    index = modulo(index, a->size); /* account for possible negative index */                                \
    if (index < 0) {                                                                                         \
        return ARRAY_ERROR;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(a, a->size + 1);                                                                      \
    size_t len = (int) a->size - index;                                                                      \
    memmove(a->arr + (index + 1), a->arr + index, len * sizeof(t));                                          \
    *(a->arr + index) = value;                                                                               \
    a->size++;                                                                                               \
    return index;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int __arr_insert_builtin_##id(Array_##id *a, int index, t *arr, int n) {                    \
    if (!arr || !n) {                                                                                        \
        return ARRAY_ERROR;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    bool append = (!a->size || index >= (int) a->size);                                                      \
    if (!append) {                                                                                           \
        index = modulo(index, a->size);                                                                      \
        if (index < 0) {                                                                                     \
            return ARRAY_ERROR;                                                                              \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    int res; /* index where first element from other was inserted */                                         \
    array_reserve_##id(a, a->size + (size_t) n);                                                             \
                                                                                                             \
    if (append) { /* append to a */                                                                          \
        memcpy(&a->arr[a->size], arr, n * sizeof(t));                                                        \
        res = (int) a->size;                                                                                 \
    } else { /* insert in the middle of a */                                                                 \
        int len = ARRAY_END(a) - index;                                                                      \
        memmove(&a->arr[index + n], &a->arr[index], len * sizeof(t));                                        \
        memcpy(&a->arr[index], arr, n * sizeof(t));                                                          \
        res = index;                                                                                         \
    }                                                                                                        \
    a->size += (size_t) n;                                                                                   \
    return res;                                                                                              \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * MATRIX SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at row "row" and column "col", similar to builtin_array[row][col].
 *   Performs bounds checking, and if the row or column is out of bounds, returns NULL.
 *
 * @param   matid  Matrix ID used with gen_matrix.
 * @param   m      Pointer to matrix.
 * @param   row    Matrix row.
 * @param   col    Matrix column.
 */
#define matrix_at(matid, m, row, col) matrix_at_##matid(m, row, col)


/**
 * Direct access to the element at row "row" and column "col". Does not perform bounds checking.
 *
 * @param   m      Pointer to matrix.
 * @param   row    Matrix row.
 * @param   col    Matrix column.
 */
#define matrix_index(m, row, col) ((m)->arr[(row)]->arr[(col)])


/**
 * Creates a new matrix of size (rows X cols).
 *
 * @param   matid  ID used with gen_matrix.
 * @param   rows   Number of rows in matrix.
 * @param   cols   Number of columns in matrix.
 *
 * @return         Pointer to newly allocated matrix.
 */
#define matrix_new(matid, rows, cols) matrix_new_##matid(rows, cols)


/**
 * Frees the matrix.
 *
 * @param   matid  ID used with gen_matrix.
 * @param   m      Pointer to matrix.
 */
#define matrix_free(matid, m) matrix_free_##matid(m)


/**
 * Generates code for a matrix (2D-array) that stores type t. gen_array(id, t) must have been
 * called prior, because the matrix is an array of Array_id.
 *
 * @param   id     ID that was used in the prior call to gen_array.
 * @param   t      Type that was used in the prior call to gen_array (and thus the type stored)
 *                   in the matrix.
 */
#define gen_matrix(id, t)                                                                                    \
gen_array(matrix_##id, Array_##id *)                                                                         \
typedef Array_matrix_##id Matrix_##id;                                                                       \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* matrix_at_##id(Matrix_##id *m, int row, int col) {                                   \
    int _idxRow = modulo(row, m->size);                                                                      \
    if (_idxRow < 0) {                                                                                       \
        return NULL;                                                                                         \
    }                                                                                                        \
    int _idxCol = modulo(col, m->arr[_idxRow]->size);                                                        \
    if (_idxCol < 0) {                                                                                       \
        return NULL;                                                                                         \
    }                                                                                                        \
    return &(m->arr[_idxRow]->arr[_idxCol]);                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Matrix_##id *matrix_new_##id(int rows, int cols) {                                          \
    if (!rows || !cols) return NULL;                                                                         \
    Matrix_##id *m = malloc(sizeof(Matrix_##id));                                                            \
    if (!m) DS_OOM();                                                                                        \
                                                                                                             \
    m->arr = malloc(sizeof(Array_##id *) * rows);                                                            \
    if (!m->arr) DS_OOM();                                                                                   \
                                                                                                             \
    m->size = 0;                                                                                             \
    m->capacity = rows;                                                                                      \
    Array_##id *arr;                                                                                         \
    for (int i = 0; i < rows; ++i) {                                                                         \
        arr = array_new_##id(ARR_INIT_SIZE, cols, (t) 0);                                                    \
        array_push_back_matrix_##id(m, arr);                                                                 \
    }                                                                                                        \
    return m;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void matrix_free_##id(Matrix_##id *m) {                                                 \
    for (size_t i = 0; i < m->size; ++i) {                                                                   \
        array_free_##id((m->arr[i]));                                                                        \
    }                                                                                                        \
    free(m->arr);                                                                                            \
    free(m);                                                                                                 \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * ARRAY ALGORITHM SECTION
 * -------------------------------------------------------------------------- */

/**
 * Sorts the array in increasing order.
 *
 * @param  id  ID used with gen_array.
 * @param  a   Pointer to array.
 */
#define array_sort(id, a) ds_sort_##id(a->arr, a->size)


/**
 * Given a sorted array, finds the provided key.
 *
 * @param  id   ID used with gen_array.
 * @param  a    Pointer to array, which must have been sorted beforehand.
 * @param  key  Value to compare to an array element's data.
 *
 * @return       Pointer to the array element if it was found, or NULL if it was not found.
 */
#define array_find(id, a, key) ds_binary_search_##id((a)->arr, 0, (ARRAY_END(a) - 1), key)


/**
 * Merges the arrays in range [first1, last1) and [first2, last2) into a new Array.
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define merge_array(id, first1, last1, first2, last2) __merge_array_##id(first1, last1, first2, last2)


/**
 * Creates a new Array representing the union of the arrays in range [first1, last1) and
 *   [first2, last2) (i.e. elements that are in the first array, the second array, or both - all
 *   elements).
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_union_array(id, first1, last1, first2, last2) __set_union_array_##id(first1, last1, first2, last2)


/**
 * Creates a new Array representing the intersection of the arrays in range [first1, last1) and
 *   [first2, last2) (i.e. all elements that both arrays have in common).
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_intersection_array(id, first1, last1, first2, last2) __set_intersection_array_##id(first1, last1, first2, last2)


/**
 * Creates a new Array representing the difference of the arrays in range [first1, last1) and
 *   [first2, last2) (i.e. all elements that are unique to the first array).
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_difference_array(id, first1, last1, first2, last2) __set_difference_array_##id(first1, last1, first2, last2)


/**
 * Creates a new Array representing the symmetric difference of the arrays in range [first1, last1)
 *   and [first2, last2) (i.e. all elements that neither array has in common).
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_symmetric_difference_array(id, first1, last1, first2, last2) __set_symmetric_difference_array_##id(first1, last1, first2, last2)


/**
 * Determines whether the array in range [first1, last1) contains each element in the range
 *   [first2, last2).
 *
 * @param   id      ID used with gen_array.
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          True if the first array contains each element in the second array, false if not.
 */
#define includes_array(id, first1, last1, first2, last2) __includes_array_##id(first1, last1, first2, last2)


/**
 * Generates array code for a specified type and ID, including sort, find, and set functions.
 *
 * @param   id      ID to be used for the type stored in the array (must be unique).
 * @param   t       Type to be stored in the array.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_array_withalg(id, t, cmp_lt)                                                                     \
gen_array(id, t)                                                                                             \
gen_alg(id, t, cmp_lt)                                                                                       \
__gen_alg_merge(id, cmp_lt, Array_##id, array_##id, __init_array, t *, __iter_next_ARR, __iter_deref_ARR, __insert_single_array, __insert_multi_1_array, __insert_multi_2_array) \
__gen_alg_set_funcs(id, cmp_lt, Array_##id, array_##id, __init_array, t *, __iter_next_ARR, __iter_deref_ARR, __insert_single_array, __insert_multi_1_array, __insert_multi_2_array) \

#endif
