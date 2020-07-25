#ifndef DS_ALG_HELPER_H
#define DS_ALG_HELPER_H

#include "ds.h"

#define __SORT_DEFAULT_THRESHOLD_FACTOR 16

#define __sort_swap(t, x, y)                                                                                 \
do {                                                                                                         \
    register t _temp = *(x);                                                                                 \
    *(x) = *(y);                                                                                             \
    *(y) = _temp;                                                                                            \
} while(0)

#define __sort_move_backward(t, first, last, result)                                                         \
do {                                                                                                         \
    size_t _len = (size_t)((last) - (first));                                                                \
    if (_len) {                                                                                              \
        memmove((result) - _len, (first), _len * sizeof(t));                                                 \
    }                                                                                                        \
} while(0)

/* --------------------------------------------------------------------------
 * RAI-Oriented Algorithm macros
 * -------------------------------------------------------------------------- */

/**
 * Sorts a given array with n elements.
 *
 * @param   id  ID used with gen_alg.
 * @param   a   Start of array.
 * @param   n   Number of elements in the array.
 */
#define sort(id, a, n) ds_sort_##id(a, n)


/**
 * Searches for the provided value in the array. The array must have been sorted prior to calling
 *   this function.
 *
 * @param   id   ID used with gen_alg.
 * @param   a    Start of array.
 * @param   n    Number of elements in the array.
 * @param   val  Value to search for.
 *
 * @return       Pointer to element if it was found, NULL if it was not found.
 */
#define binary_search(id, a, n, val) ds_binary_search_##id(a, 0, ((n) - 1), val)


/**
 * Creates a max-heap in the range [first, last).
 *
 * @param   id     ID used with gen_alg.
 * @param   first  Pointer to first index in the heap.
 * @param   last   Pointer to last index in the heap.
 */
#define make_heap(id, first, last) _ds_make_heap_##id(first, last)


/**
 * Sorts a heap in the range [first, last), so that elements are ordered from smallest to
 *   largest.
 *
 * @param   id     ID used with gen_alg.
 * @param   first  Pointer to first index in the heap.
 * @param   last   Pointer to last index in the heap.
 */
#define sort_heap(id, first, last) _ds_sort_heap_##id(first, last)


/**
 * Extends a heap in the range [first, last - 1) to [first, last) (the element in position
 *   (last - 1) is pushed).
 *
 * @param   id     ID used with gen_alg.
 * @param   first  Pointer to first index in the heap.
 * @param   last   Pointer to last index in the heap.
 */
#define push_heap(id, first, last) ds_push_heap_##id(first, last)


/**
 * Shrinks a heap in the range [first, last) to [first, last - 1) (the element in position
 *   (first) is popped).
 *
 * @param   id     ID used with gen_alg.
 * @param   first  Pointer to first index in the heap.
 * @param   last   Pointer to last index in the heap.
 */
#define pop_heap(id, first, last) ds_pop_heap_##id(first, last)


/**
 * Generates algorithm code for a specified type, to be used with built-in arrays or dynamic arrays.
 *
 * @param   id      ID to be used for the data type used in these functions (must be unique).
 * @param   t       Type to be used in the array (i.e. int, char *)
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_alg(id, t, cmp_lt)                                                                               \
__DS_FUNC_PREFIX void _ds_push_heap_##id(t *first, size_t i, size_t top, t *val) {                           \
    size_t parent = (i - 1) >> 1;                                                                            \
    while (i > top && (cmp_lt(*(first + parent), *val))) {                                                   \
        *(first + i) = *(first + parent);                                                                    \
        i = parent;                                                                                          \
        parent = (i - 1) >> 1;                                                                               \
    }                                                                                                        \
    *(first + i) = *val;                                                                                     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_adjust_heap_##id(t *first, size_t i, size_t len, t *value) {                       \
    const size_t top = i;                                                                                    \
    size_t second = i;                                                                                       \
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
    _ds_push_heap_##id(first, i, top, value);                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void _ds_pop_heap_##id(t *first, t *last, t *result) {                                  \
    t value = *result;                                                                                       \
    *result = *first;                                                                                        \
    _ds_adjust_heap_##id(first, 0, last - first, &value);                                                    \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_make_heap_##id(t *first, t *last) {                                                \
    const size_t len = last - first;                                                                         \
    if (len < 2) return;                                                                                     \
                                                                                                             \
    size_t parent = (len - 2) >> 1;                                                                          \
    t value;                                                                                                 \
    while (1) {                                                                                              \
        value = *(first + parent);                                                                           \
        _ds_adjust_heap_##id(first, parent, len, &value);                                                    \
        if (!parent) return;                                                                                 \
        --parent;                                                                                            \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_sort_heap_##id(t *first, t *last) {                                                \
    while ((last - first) > 1) {                                                                             \
        --last;                                                                                              \
        _ds_pop_heap_##id(first, last, last);                                                                \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_heap_select_##id(t *first, t *mid, t *last) {                                      \
    _ds_make_heap_##id(first, mid);                                                                          \
    for (t *i = mid; i < last; ++i) {                                                                        \
        if (cmp_lt(*i, *first)) {                                                                            \
            _ds_pop_heap_##id(first, mid, i);                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void _ds_partial_sort_##id(t *first, t *mid, t *last) {                                 \
    _ds_heap_select_##id(first, mid, last);                                                                  \
    _ds_sort_heap_##id(first, mid);                                                                          \
}                                                                                                            \
                                                                                                             \
/* --------------------------------------------------------------------------
 * partitioning functions
 * -------------------------------------------------------------------------- */                             \
                                                                                                             \
__DS_FUNC_PREFIX_INL void _ds_move_median_to_first_##id(t *result, t *a, t *b, t *c) {                       \
    if (cmp_lt(*a, *b)) {                                                                                    \
        if (cmp_lt(*b, *c))                                                                                  \
            __sort_swap(t, result, b);                                                                       \
         else if (cmp_lt(*a, *c))                                                                            \
            __sort_swap(t, result, c);                                                                       \
         else                                                                                                \
            __sort_swap(t, result, a);                                                                       \
    } else if (cmp_lt(*a, *c))                                                                               \
        __sort_swap(t, result, a);                                                                           \
     else if (cmp_lt(*b, *c))                                                                                \
        __sort_swap(t, result, c);                                                                           \
     else                                                                                                    \
        __sort_swap(t, result, b);                                                                           \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX t *_ds_unguarded_partition_##id(t *first, t *last, t *pivot) {                              \
    while (1) {                                                                                              \
        while (cmp_lt(*first, *pivot))                                                                       \
            ++first;                                                                                         \
        --last;                                                                                              \
        while (cmp_lt(*pivot, *last))                                                                        \
            --last;                                                                                          \
        if (!(first < last))                                                                                 \
            return first;                                                                                    \
        __sort_swap(t, first, last);                                                                         \
        ++first;                                                                                             \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *_ds_unguarded_partition_pivot_##id(t *first, t *last) {                              \
    t* mid = first + (last - first) / 2;                                                                     \
    _ds_move_median_to_first_##id(first, first + 1, mid, last - 1);                                          \
    return _ds_unguarded_partition_##id(first + 1, last, first);                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_introsort_##id(t *first, t *last, unsigned depth_limit) {                          \
    while ((last - first) > __SORT_DEFAULT_THRESHOLD_FACTOR) {                                               \
        if (depth_limit == 0) {                                                                              \
            _ds_partial_sort_##id(first, last, last);                                                        \
            return;                                                                                          \
        }                                                                                                    \
        --depth_limit;                                                                                       \
        t *cut = _ds_unguarded_partition_pivot_##id(first, last);                                            \
        _ds_introsort_##id(cut, last, depth_limit);                                                          \
        last = cut;                                                                                          \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
/* --------------------------------------------------------------------------
 * insertion sort functions
 * -------------------------------------------------------------------------- */                             \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_unguarded_linear_insert_##id(t *last) {                                            \
    t val = *last;                                                                                           \
    t *next = last - 1;                                                                                      \
    while (cmp_lt(val, *next)) {                                                                             \
        *last = *next;                                                                                       \
        last = next;                                                                                         \
        --next;                                                                                              \
    }                                                                                                        \
    *last = val;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _ds_insertion_sort_##id(t *first, t *last) {                                           \
    if (first == last) return;                                                                               \
                                                                                                             \
    t val;                                                                                                   \
    for (t *i = first + 1; i != last; ++i) {                                                                 \
        if (cmp_lt(*i, *first)) {                                                                            \
            val = *i;                                                                                        \
            __sort_move_backward(t, first, i, i + 1);                                                        \
            *first = val;                                                                                    \
        } else {                                                                                             \
            _ds_unguarded_linear_insert_##id(i);                                                             \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void _ds_unguarded_insertion_sort_##id(t *first, t *last) {                             \
    for (t *i = first; i != last; ++i) {                                                                     \
        _ds_unguarded_linear_insert_##id(i);                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void _ds_final_insertion_##id(t *first, t *last) {                                      \
    if ((last - first) > __SORT_DEFAULT_THRESHOLD_FACTOR) {                                                  \
        _ds_insertion_sort_##id(first, first + __SORT_DEFAULT_THRESHOLD_FACTOR);                             \
        _ds_unguarded_insertion_sort_##id(first + __SORT_DEFAULT_THRESHOLD_FACTOR, last);                    \
    } else {                                                                                                 \
        _ds_insertion_sort_##id(first, last);                                                                \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void ds_sort_##id(t *arr, size_t n) {                                                   \
    if (n > 1) {                                                                                             \
        unsigned depth = 0;                                                                                  \
        size_t tempN = n;                                                                                    \
        while ((tempN >>= 1) >= 1) ++depth;                                                                  \
        depth <<= 1;                                                                                         \
        t *first = arr;                                                                                      \
        t *last = &first[n];                                                                                 \
                                                                                                             \
        _ds_introsort_##id(first, last, depth);                                                              \
        _ds_final_insertion_##id(first, last);                                                               \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *ds_binary_search_##id(t *arr, int l, int r, t val) {                                 \
    int mid;                                                                                                 \
    while (l <= r) {                                                                                         \
        mid = l + (r - l) / 2;                                                                               \
        if (cmp_lt(arr[mid], val)) {                                                                         \
            l = mid + 1;                                                                                     \
        } else if (cmp_lt(val, arr[mid])) {                                                                  \
            r = mid - 1;                                                                                     \
        }else {                                                                                              \
            return &arr[mid];                                                                                \
        }                                                                                                    \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void ds_push_heap_##id(t *first, t *last) {                                             \
    t value = *(last - 1);                                                                                   \
    _ds_push_heap_##id(first, (last - first) - 1, 0, &value);                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void ds_pop_heap_##id(t *first, t *last) {                                              \
    if (last - first > 1) {                                                                                  \
        --last;                                                                                              \
        _ds_pop_heap_##id(first, last, last);                                                                \
    }                                                                                                        \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * TEMPLATE MERGE
 * -------------------------------------------------------------------------- */

#define __gen_alg_merge(id, cmp_lt, Adt, ds_type, init, iter_type, iter_next, deref, insert_single, insert_multi_1, insert_multi_2) \
__DS_FUNC_PREFIX Adt *__merge_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Adt *d_new = init(id);                                                                                   \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first2), deref(first1))) {                                                          \
            insert_single(id, d_new, deref(first2));                                                         \
            iter_next(id, first2);                                                                           \
        } else {                                                                                             \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (first1 != last1) {                                                                                   \
        insert_multi_1(id);                                                                                  \
    }                                                                                                        \
                                                                                                             \
    if (first2 != last2) {                                                                                   \
        insert_multi_2(id);                                                                                  \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * TEMPLATE SET OPERATIONS
 * -------------------------------------------------------------------------- */

#define __gen_alg_set_funcs(id, cmp_lt, Adt, ds_type, init, iter_type, iter_next, deref, insert_single, insert_multi_1, insert_multi_2) \
__DS_FUNC_PREFIX Adt *__set_union_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Adt *d_new = init(id);                                                                                   \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first1), deref(first2))) {                                                          \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
        } else if (cmp_lt(deref(first2), deref(first1))) {                                                   \
            insert_single(id, d_new, deref(first2));                                                         \
            iter_next(id, first2);                                                                           \
        } else {                                                                                             \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
            iter_next(id, first2);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        insert_multi_1(id);                                                                                  \
    }                                                                                                        \
    if (first2 != last2) {                                                                                   \
        insert_multi_2(id);                                                                                  \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Adt *__set_intersection_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Adt *d_new = init(id);                                                                                   \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first1), deref(first2))) {                                                          \
            iter_next(id, first1);                                                                           \
        } else if (cmp_lt(deref(first2), deref(first1))) {                                                   \
            iter_next(id, first2);                                                                           \
        } else {                                                                                             \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
            iter_next(id, first2);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Adt *__set_difference_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Adt *d_new = init(id);                                                                                   \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first1), deref(first2))) {                                                          \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
        } else if (cmp_lt(deref(first2), deref(first1))) {                                                   \
            iter_next(id, first2);                                                                           \
        } else {                                                                                             \
            iter_next(id, first1);                                                                           \
            iter_next(id, first2);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        insert_multi_1(id);                                                                                  \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Adt *__set_symmetric_difference_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    Adt *d_new = init(id);                                                                                   \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first1), deref(first2))) {                                                          \
            insert_single(id, d_new, deref(first1));                                                         \
            iter_next(id, first1);                                                                           \
        } else if (cmp_lt(deref(first2), deref(first1))) {                                                   \
            insert_single(id, d_new, deref(first2));                                                         \
            iter_next(id, first2);                                                                           \
        } else {                                                                                             \
            iter_next(id, first1);                                                                           \
            iter_next(id, first2);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        insert_multi_1(id);                                                                                  \
    }                                                                                                        \
    if (first2 != last2) {                                                                                   \
        insert_multi_2(id);                                                                                  \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX bool __includes_##ds_type(iter_type first1, iter_type last1, iter_type first2, iter_type last2) { \
    if (!first1 || !first2) {                                                                                \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(deref(first2), deref(first1))) {                                                          \
            return false;                                                                                    \
        } else if (cmp_lt(deref(first1), deref(first2))) {                                                   \
            iter_next(id, first1);                                                                           \
        } else {                                                                                             \
            iter_next(id, first1);                                                                           \
            iter_next(id, first2);                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    return first2 == last2;                                                                                  \
}                                                                                                            \

#endif
