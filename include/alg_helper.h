#ifndef DS_ALG_HELPER_H
#define DS_ALG_HELPER_H

#include "ds.h"

/* --------------------------------------------------------------------------
 * RAI-Oriented Algorithm macros
 * -------------------------------------------------------------------------- */

/**
 * Sorts the array `a` with `n` elements.
 *
 * @param  a  Start of array.
 * @param  n  Number of elements in the array.
 */
#define sort(id, a, n) ds_sort_##id(a, n)


/**
 * Searches for `val` in an array `a`. The array must have been sorted prior to calling this function.
 *
 * @param   a    Start of array.
 * @param   n    Number of elements in the array.
 * @param   val  Value to search for.
 *
 * @return       Pointer to element if it was found, NULL if it was not found.
 */
#define binary_search(id, a, n, val) ds_binary_search_##id(a, 0, ((n) - 1), val)


/**
 * Creates a max-heap in the range [`first`, `last`).
 *
 * @param  first  Pointer to first index in the heap.
 * @param  last   Pointer to last index in the heap.
 */
#define make_heap(id, first, last) ds_make_heap_##id(first, last)


/**
 * Sorts a heap in the range [`first`, `last`), so that elements are ordered from smallest to largest.
 *
 * @param  first  Pointer to first index in the heap.
 * @param  last   Pointer to last index in the heap.
 */
#define sort_heap(id, first, last) ds_sort_heap_##id(first, last)


/**
 * Extends a heap in the range [`first`, `last - 1`) to [`first`, `last`) (the element in position 
 * `last - 1` is pushed).
 *
 * @param  first  Pointer to first index in the heap.
 * @param  last   Pointer to last index in the heap.
 */
#define push_heap(id, first, last) ds_push_heap_##id(first, last)


/**
 * Shrinks a heap in the range [`first`, `last`) to [`first`, `last - 1`) (the element in position 
 * `first` is popped).
 *
 * @param  first  Pointer to first index in the heap.
 * @param  last   Pointer to last index in the heap.
 */
#define pop_heap(id, first, last) ds_pop_heap_##id(first, last)


/**
 * Generates algorithm function declarations for the specified type, to be used with built-in or dynamic arrays.
 *
 * @param  id  ID to be used for the data type used in these functions (must be unique).
 * @param  t   Type to be stored in the array.
 */
#define gen_alg_headers(id, t)                                                                               \
                                                                                                             \
void ds_make_heap_##id(t* first, t* last);                                                                   \
void ds_sort_heap_##id(t* first, t* last);                                                                   \
void ds_sort_##id(t* arr, unsigned n);                                                                       \
t* ds_binary_search_##id(t* arr, int l, int r, t val);                                                       \
void ds_push_heap_##id(t* first, t* last);                                                                   \
void ds_pop_heap_##id(t* first, t* last);                                                                    \


/**
 * Generates algorithm function definitions for the specified type.
 *
 * @param  id      ID used in @c gen_alg_headers .
 * @param  t       Type used in @c gen_alg_headers .
 * @param  cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_alg_source(id, t, cmp_lt)                                                                        \
                                                                                                             \
void __ds_push_heap_##id(t *first, unsigned i, unsigned top, t *val) {                                       \
    unsigned parent = (i - 1) >> 1;                                                                          \
    while (i > top && (cmp_lt(*(first + parent), *val))) {                                                   \
        *(first + i) = *(first + parent);                                                                    \
        i = parent;                                                                                          \
        parent = (i - 1) >> 1;                                                                               \
    }                                                                                                        \
    *(first + i) = *val;                                                                                     \
}                                                                                                            \
                                                                                                             \
void __ds_adjust_heap_##id(t *first, unsigned i, unsigned len, t *value) {                                   \
    const unsigned top = i;                                                                                  \
    unsigned second = i;                                                                                     \
    while (second < ((len - 1) >> 1)) {                                                                      \
        second = (second + 1) << 1;                                                                          \
        if (cmp_lt(*(first + second), *(first + (second - 1)))) {                                            \
            --second;                                                                                        \
        }                                                                                                    \
        *(first + i) = *(first + second);                                                                    \
        i = second;                                                                                          \
    }                                                                                                        \
    if (((len & 1) == 0) && second == ((len - 2) >> 1)) {                                                    \
        second = (second + 1) << 1;                                                                          \
        *(first + i) = *(first + (second - 1));                                                              \
        i = second - 1;                                                                                      \
    }                                                                                                        \
    __ds_push_heap_##id(first, i, top, value);                                                               \
}                                                                                                            \
                                                                                                             \
void __ds_pop_heap_##id(t *first, t *last, t *result) {                                                      \
    t value = *result;                                                                                       \
    *result = *first;                                                                                        \
    __ds_adjust_heap_##id(first, 0, (unsigned)(last - first), &value);                                       \
}                                                                                                            \
                                                                                                             \
void ds_make_heap_##id(t *first, t *last) {                                                                  \
    const unsigned len = (unsigned)(last - first);                                                           \
    unsigned parent;                                                                                         \
    if (len < 2) return;                                                                                     \
                                                                                                             \
    parent = (len - 2) >> 1;                                                                                 \
    while (1) {                                                                                              \
        t value = *(first + parent);                                                                         \
        __ds_adjust_heap_##id(first, parent, len, &value);                                                   \
        if (!parent) return;                                                                                 \
        --parent;                                                                                            \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void ds_sort_heap_##id(t *first, t *last) {                                                                  \
    while ((last - first) > 1) {                                                                             \
        --last;                                                                                              \
        __ds_pop_heap_##id(first, last, last);                                                               \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void __ds_introsort_##id(t *first, t *last, unsigned depth_limit) {                                          \
    t _temp;                                                                                                 \
    while ((last - first) > 16) {                                                                            \
        t *cut;                                                                                              \
        if (depth_limit == 0) {                                                                              \
            ds_make_heap_##id(first, last);                                                                  \
            ds_sort_heap_##id(first, last);                                                                  \
            return;                                                                                          \
        }                                                                                                    \
        --depth_limit;                                                                                       \
        {                                                                                                    \
            t* b = first + (last - first) / 2; t* a = first + 1; t* c = last - 1;                            \
            _temp = *first;                                                                                  \
            if (cmp_lt(*a, *b)) {                                                                            \
                if (cmp_lt(*b, *c)) {                                                                        \
                    *first = *b;                                                                             \
                    *b = _temp;                                                                              \
                } else if (cmp_lt(*a, *c)) {                                                                 \
                    *first = *c;                                                                             \
                    *c = _temp;                                                                              \
                } else {                                                                                     \
                    *first = *a;                                                                             \
                    *a = _temp;                                                                              \
                }                                                                                            \
            } else if (cmp_lt(*a, *c)) {                                                                     \
                *first = *a;                                                                                 \
                *a = _temp;                                                                                  \
            } else if (cmp_lt(*b, *c)) {                                                                     \
                *first = *c;                                                                                 \
                *c = _temp;                                                                                  \
            } else {                                                                                         \
                *first = *b;                                                                                 \
                *b = _temp;                                                                                  \
            }                                                                                                \
        }                                                                                                    \
        {                                                                                                    \
            t* left = first + 1; t* right = last; t* pivot = first;                                          \
            while (1) {                                                                                      \
                while (cmp_lt(*left, *pivot))                                                                \
                    ++left;                                                                                  \
                --right;                                                                                     \
                while (cmp_lt(*pivot, *right))                                                               \
                    --right;                                                                                 \
                if (!(left < right)) {                                                                       \
                    cut = left;                                                                              \
                    break;                                                                                   \
                }                                                                                            \
                                                                                                             \
                _temp = *left;                                                                               \
                *left = *right;                                                                              \
                *right = _temp;                                                                              \
                ++left;                                                                                      \
            }                                                                                                \
        }                                                                                                    \
        __ds_introsort_##id(cut, last, depth_limit);                                                         \
        last = cut;                                                                                          \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
/* --------------------------------------------------------------------------
 * insertion sort functions
 * -------------------------------------------------------------------------- */                             \
                                                                                                             \
void __ds_unguarded_linear_insert_##id(t *last) {                                                            \
    t const val = *last;                                                                                     \
    t *next = last - 1;                                                                                      \
    while (cmp_lt(val, *next)) {                                                                             \
        *last = *next;                                                                                       \
        last = next;                                                                                         \
        --next;                                                                                              \
    }                                                                                                        \
    *last = val;                                                                                             \
}                                                                                                            \
                                                                                                             \
void __ds_insertion_sort_##id(t *first, t *last) {                                                           \
    t *i;                                                                                                    \
    t* const begin = first + 1;                                                                              \
    if (first == last) return;                                                                               \
                                                                                                             \
    for (i = first + 1; i != last; ++i) {                                                                    \
        t val;                                                                                               \
        if (cmp_lt(*i, *first)) {                                                                            \
            val = *i;                                                                                        \
            memmove(begin, first, (unsigned)(i - first) * sizeof(t));                                        \
            *first = val;                                                                                    \
        } else {                                                                                             \
            __ds_unguarded_linear_insert_##id(i);                                                            \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void ds_sort_##id(t *arr, unsigned n) {                                                                      \
    t *first; t *last;                                                                                       \
    unsigned depth = 0;                                                                                      \
    if (n <= 1) return;                                                                                      \
                                                                                                             \
    first = arr, last = &arr[n];                                                                             \
    while ((n >>= 1) >= 1) ++depth;                                                                          \
    depth <<= 1;                                                                                             \
                                                                                                             \
    __ds_introsort_##id(first, last, depth);                                                                 \
    if ((last - first) > 16) {                                                                               \
        t *i;                                                                                                \
        __ds_insertion_sort_##id(first, first + 16);                                                         \
        for (i = first + 16; i != last; ++i) {                                                               \
            __ds_unguarded_linear_insert_##id(i);                                                            \
        }                                                                                                    \
    } else {                                                                                                 \
        __ds_insertion_sort_##id(first, last);                                                               \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
t *ds_binary_search_##id(t *arr, int l, int r, t val) {                                                      \
    while (l <= r) {                                                                                         \
        int mid = l + (r - l) / 2;                                                                           \
        if (cmp_lt(arr[mid], val)) {                                                                         \
            l = mid + 1;                                                                                     \
        } else if (cmp_lt(val, arr[mid])) {                                                                  \
            r = mid - 1;                                                                                     \
        } else {                                                                                             \
            return &arr[mid];                                                                                \
        }                                                                                                    \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
void ds_push_heap_##id(t *first, t *last) {                                                                  \
    t value = *(last - 1);                                                                                   \
    __ds_push_heap_##id(first, (unsigned)(last - first) - 1, 0, &value);                                     \
}                                                                                                            \
                                                                                                             \
void ds_pop_heap_##id(t *first, t *last) {                                                                   \
    if (last - first > 1) {                                                                                  \
        --last;                                                                                              \
        __ds_pop_heap_##id(first, last, last);                                                               \
    }                                                                                                        \
}                                                                                                            \

#endif
