#ifndef LIST_H
#define LIST_H

#include "iterator.h"
#include "alg_helper.h"

#define __list_swap(id, x, y) {                                                                              \
    register ListEntry_##id *ltemp_front = (x)->front;                                                       \
    register ListEntry_##id *ltemp_back = (x)->back;                                                         \
    register size_t ltemp_size = (x)->size;                                                                  \
    (x)->front = (y)->front;                                                                                 \
    (x)->back = (y)->back;                                                                                   \
    (x)->size = (y)->size;                                                                                   \
    (y)->front = ltemp_front;                                                                                \
    (y)->back = ltemp_back;                                                                                  \
    (y)->size = ltemp_size;                                                                                  \
}

#define __list_elem_removal_body(id, l, condition, currStart, prevStart)                                     \
    if (!(l)->front) return;                                                                                 \
                                                                                                             \
    ListEntry_##id *curr = currStart;                                                                        \
    ListEntry_##id *prev = prevStart;                                                                        \
    ListEntry_##id *next;                                                                                    \
                                                                                                             \
    while (curr) {                                                                                           \
        next = curr->next;                                                                                   \
        if (condition) {                                                                                     \
            if (prev) {                                                                                      \
                prev->next = next;                                                                           \
            } else {                                                                                         \
                (l)->front = next;                                                                           \
            }                                                                                                \
                                                                                                             \
            if (next) {                                                                                      \
                next->prev = prev;                                                                           \
            } else {                                                                                         \
                (l)->back = prev;                                                                            \
            }                                                                                                \
                                                                                                             \
            free(curr);                                                                                      \
            (l)->size--;                                                                                     \
            curr = next;                                                                                     \
        } else {                                                                                             \
            prev = curr;                                                                                     \
            curr = curr->next;                                                                               \
        }                                                                                                    \
    }                                                                                                        \
    (l)->back = prev;                                                                                        \

#define __list_push_body(id, dir, rev, loc)                                                                  \
    ListEntry_##id *new = __ds_calloc(1, sizeof(ListEntry_##id));                                            \
    new->data = value;                                                                                       \
                                                                                                             \
    if (!(l->front)) {                                                                                       \
        l->front = l->back = new;                                                                            \
    } else {                                                                                                 \
        new->dir = loc;                                                                                      \
        loc->rev = new;                                                                                      \
        loc = new;                                                                                           \
    }                                                                                                        \
    l->size++;                                                                                               \

#define __list_pop_body(id, loc, dir, rev, tail)                                                             \
    if (!(l->front)) return;                                                                                 \
    ListEntry_##id *repl = loc;                                                                              \
                                                                                                             \
    loc = repl->dir;                                                                                         \
    if (loc) {                                                                                               \
        loc->rev = NULL;                                                                                     \
    } else {                                                                                                 \
        tail = NULL;                                                                                         \
    }                                                                                                        \
    free(repl);                                                                                              \
    l->size--;                                                                                               \


/* --------------------------------------------------------------------------
 * PRIMARY LIST SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the front element's data, if the list is not empty.
 */
#define list_front(l) ((l)->front ? &((l)->front->data) : NULL)


/**
 * Pointer to the back element's data, if the list is not empty.
 */
#define list_back(l) ((l)->back ? &((l)->back->data) : NULL)


/**
 * Whether the list has no elements.
 */
#define list_empty(l) (!((l)->front))


/**
 * The number of elements in the list.
 */
#define list_size(l) ((int) (l)->size)


/**
 * Macro for iterating over the list from front to back.
 *
 * @param  ptr  `ListEntry` which is assigned to the current element. May be dereferenced with ptr->data.
 */
#define list_iter(id, l, ptr) for (ptr = iter_begin(LIST, id, l, 0); ptr != iter_end(LIST, id, l, 0); iter_next(LIST, id, ptr))


/**
 * Macro for iterating over the list in reverse (from back to front).
 *
 * @param  ptr  `ListEntry` which is assigned to the current element. May be dereferenced with ptr->data.
 */
#define list_riter(id, l, ptr) for (ptr = iter_rbegin(LIST, id, l, 0); ptr != iter_rend(LIST, id, l, 0); iter_prev(LIST, id, ptr))


/**
 * Creates a new, empty list.
 *
 * @return  Pointer to the newly allocated list.
 */
#define list_new(id) __ds_calloc(1, sizeof(List_##id))


/**
 * Creates a new list using `n` elements in a built-in array `arr`.
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly allocated list.
 */
#define list_new_fromArray(id, arr, n) list_new_fromArray_##id(arr, n)


/**
 * Creates a new list as a copy of `list`.
 *
 * @param   list  `List` to copy.
 *
 * @return        Pointer to the newly allocated list.
 */
#define list_createCopy(id, list) list_createCopy_##id(list)


/**
 * Deletes all elements and frees the list.
 */
#define list_free(id, l) list_free_##id(l) 


/**
 * Prepends `value` to the start of the list.
 *
 * @param  value  Value to insert.
 */
#define list_push_front(id, l, value) list_push_front_##id(l, value)


/**
 * Appends `value` to the end of the list.
 *
 * @param  value  Value to insert.
 */
#define list_push_back(id, l, value) list_push_back_##id(l, value)


/**
 * Removes the first element from the list, if it is not empty.
 */
#define list_pop_front(id, l) list_pop_front_##id(l)


/**
 * Removes the last element from the list, if it is not empty.
 */
#define list_pop_back(id, l) list_pop_back_##id(l)


/**
 * Inserts `value` before `pos`.
 *
 * @param   pos     `ListEntry` before which the element should be inserted. If this is NULL, it
 *                    defaults to `list_push_back`.
 * @param   value   Value to insert.
 *
 * @return          If successful, returns a `ListEntry` corresponding to the inserted element. If
 *                  an error occurred, returns NULL.
 */
#define list_insert(id, l, pos, value) list_insert_##id(l, pos, value)


/**
 * Inserts `value` in sorted order.
 *
 * @param   value   Value to insert.
 *
 * @return          If successful, returns a `ListEntry` corresponding to the inserted element. If an
 *                  error occurred, returns NULL.
 */
#define list_insert_sorted(id, l, value) list_insert_sorted_##id(l, value)


/**
 * Inserts `n` elements from the built-in array `arr` before `pos`.
 *
 * @param   pos     `ListEntry` before which the elements should be inserted. If this is NULL, it
 *                    defaults to `list_push_back`.
 * @param   sorted  Whether elements should be inserted in sorted order. If this is true, `pos` is ignored.
 * @param   arr     Pointer to the first element to insert.
 * @param   n       Number of elements to include.
 *
 * @return          If successful, and `sorted` is false, returns a `ListEntry` corresponding to the
 *                  first inserted element. If `sorted` is true, returns a `ListEntry` corresponding
 *                  to the front of the list. If an error occurred, returns NULL.
 */
#define list_insert_fromArray(id, l, pos, sorted, arr, n) list_insert_fromArray_##id(l, pos, sorted, arr, n)


/**
 * Inserts new elements from another `List` in the range [`start`, `end`) before `pos`.
 *
 * @param   pos     `ListEntry` before which the elements should be inserted. If this is NULL, it
 *                    defaults to `list_push_back`.
 * @param   sorted  Whether elements should be inserted in sorted order. If this is true, `pos` is ignored.
 * @param   start   First `ListEntry` to insert. Must not be NULL.
 * @param   end     `ListEntry` after the last entry to insert. If this is NULL, all elements from
 *                    `start` through the end of the other list will be inserted.
 *
 * @return          If successful, and `sorted` is false, returns a `ListEntry` corresponding to the
 *                  first inserted element. If `sorted` is true, returns a `ListEntry` corresponding
 *                  to the front of the list. If an error occurred, returns NULL.
 */
#define list_insert_fromList(id, l, pos, sorted, start, end) list_insert_fromList_##id(l, pos, sorted, start, end)


/**
 * Erases elements within the range [`first`, `last`).
 *
 * @param   first  First `ListEntry` to be removed - must be provided.
 * @param   last   `ListEntry` after the last entry to be deleted. If this is NULL, all elements
 *                   from start through the end of the list will be removed.
 *
 * @return         If successful, returns a `ListEntry` corresponding to the element after the last
 *                 deleted element. If an error occurred or if the last deleted element was at the
 *                 end of the list, returns NULL.
 */
#define list_erase(id, l, first, last) list_erase_##id(l, first, last)


/**
 * Removes all elements from the list.
 */
#define list_clear(id, l) list_erase_##id(l, l->front, NULL)


/**
 * Reverses the list; thus what was the last element will now be the first.
 */
#define list_reverse(id, l) list_reverse_##id(l)


/**
 * Sorts the list according to the `cmp_lt` macro provided in `gen_list`.
 * 
 * Time complexity: approx. O(n * log(n))
 */
#define list_sort(id, l) list_sort_##id(l)


/**
 * Scans the list and removes any adjacent elements that compare equal to the preceding element.
 * 
 * Example:
 *     Input : [1, 2, 2, 2, 3, 3]
 *     Output: [1, 2, 3]
 */
#define list_unique(id, l) list_unique_##id(l)


/**
 * Removes any elements equal to `value`.
 *
 * @param  value  Value to compare to a list element's data.
 */
#define list_remove_value(id, l, value) list_remove_value_##id(l, value)


/**
 * Removes any elements satisfying `condition`.
 *
 * @param  condition  Function pointer to check if an element meets the condition.
 */
#define list_remove_if(id, l, condition) list_remove_if_##id(l, condition)


/**
 * Finds the first instance of `value`.
 *
 * @param   value  Value to search for.
 *
 * @return         If `value` was found, returns a `ListEntry` corresponding to that element. If it
 *                 was not found, returns NULL.
 */
#define list_find(id, l, value) list_find_##id(l, value)


/**
 * Creates a sublist from this list in the range [`first`, `last`).
 *
 * @param   first  First `ListEntry` in the sublist.
 * @param   last   `ListEntry` after the last entry in the sublist. If this is NULL, the sublist will
 *                   include all elements from `first` to the end of the list.
 *
 * @return         Newly created sublist. If this list is empty or `first` is NULL, returns NULL.
 */
#define list_sublist(id, l, first, last) list_sublist_##id(l, first, last)


/**
 * Merges `other` into this list, both of which must be in sorted order prior to this operation. 
 * `other` is left with a size of 0, and this list grows by as many elements as `other` previously 
 * contained.
 *
 * @param  other  Other `List`, which will be merged with this list.
 */
#define list_merge(id, l, other) list_merge_##id(l, other)


/**
 * Moves all elements from `other` into this list before `pos`.
 *
 * @param  pos    `ListEntry` in this list before which elements in `other` will be moved. If this
 *                  is NULL, elements from `other` will be appended to this list.
 * @param  other  Other `List` from which elements will be moved.
 */
#define list_splice(id, l, pos, other) list_splice_range_##id(l, pos, other, (other)->front, NULL)


/**
 * Moves `entry` from `other` into this list before `pos`.
 *
 * @param  pos     `ListEntry` in this list before which `entry` will be moved. If this is NULL,
 *                   `entry` is appended to this list.
 * @param  other   Other `List` from which `entry` will be moved.
 * @param  entry  `ListEntry` to move.
 */
#define list_splice_element(id, l, pos, other, entry) list_splice_range_##id(l, pos, other, entry, entry ? (entry)->next : NULL)


/**
 * Moves elements from `other` in the range [`first`, `last`) into this list before `pos`.
 *
 * @param  pos    `ListEntry` in this list before which elements in `other` will be moved. If this
 *                  is NULL, elements from `other` will be appended to this list.
 * @param  other  Other `List` from which elements will be moved.
 * @param  first  First `ListEntry` from `other` to move.
 * @param  last   `ListEntry` after the last entry in `other` to move. If this is NULL, all entries
 *                  from `first` through the end of `other` are moved.
 */
#define list_splice_range(id, l, pos, other, first, last) list_splice_range_##id(l, pos, other, first, last)

/* --------------------------------------------------------------------------
 * List iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_LIST(id, l, n)    ((l)->front)
#define iter_end_LIST(id, l, n)      NULL
#define iter_rbegin_LIST(id, l, n)   ((l)->back)
#define iter_rend_LIST(id, l, n)     NULL
#define iter_next_LIST(id, p)        ((p) = (p)->next)
#define iter_prev_LIST(id, p)        ((p) = (p)->prev)
#define iter_deref_LIST(p)           ((p)->data)
#define iter_advance_LIST(id, p, n)  iterator_advance_helper(LIST, id, p, n)
#define iter_dist_LIST(id, p1, p2)   __iter_dist_helper_LIST_##id(p1, p2)


/**
 * Generates `List` code for a specified type and ID.
 *
 * @param  id      ID to be used for the `List` and `ListEntry` types (must be unique).
 * @param  t       Type to be stored in the list.
 * @param  cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_list(id, t, cmp_lt)                                                                              \
                                                                                                             \
typedef struct ListEntry_##id ListEntry_##id;                                                                \
struct ListEntry_##id {                                                                                      \
    ListEntry_##id *prev;                                                                                    \
    ListEntry_##id *next;                                                                                    \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    ListEntry_##id *front;                                                                                   \
    ListEntry_##id *back;                                                                                    \
} List_##id;                                                                                                 \
                                                                                                             \
create_iterator_distance_helper(LIST, id, ListEntry_##id *)                                                  \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_##id(List_##id *l, ListEntry_##id *pos, t value);               \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_sorted_##id(List_##id *l, t value);                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_fromArray_##id(List_##id *l, ListEntry_##id *pos, bool sorted, t *arr, size_t n); \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_fromList_##id(List_##id *l, ListEntry_##id *pos, bool sorted, ListEntry_##id *start, ListEntry_##id *end); \
__DS_FUNC_PREFIX void list_splice_range_##id(List_##id *this, ListEntry_##id *position, List_##id *other, ListEntry_##id *first, ListEntry_##id *last); \
__DS_FUNC_PREFIX ListEntry_##id *list_erase_##id(List_##id *l, ListEntry_##id *first, ListEntry_##id *last); \
__DS_FUNC_PREFIX void list_merge_##id(List_##id *this, List_##id *other);                                    \
                                                                                                             \
__DS_FUNC_PREFIX List_##id *list_new_fromArray_##id(t *arr, size_t size) {                                   \
    List_##id *l = list_new(id);                                                                             \
    list_insert_fromArray_##id(l, NULL, false, arr, size);                                                   \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX List_##id *list_createCopy_##id(List_##id *list) {                                          \
    List_##id *l = list_new(id);                                                                             \
    list_insert_fromList_##id(l, NULL, false, list->front, NULL);                                            \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_free_##id(List_##id *l) {                                                     \
    list_erase_##id(l, l->front, NULL);                                                                      \
    free(l);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_push_front_##id(List_##id *l, t value) {                                      \
    __list_push_body(id, next, prev, l->front)                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_push_back_##id(List_##id *l, t value) {                                       \
    __list_push_body(id, prev, next, l->back)                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_pop_front_##id(List_##id *l) {                                                \
    __list_pop_body(id, l->front, next, prev, l->back)                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_pop_back_##id(List_##id *l) {                                                 \
    __list_pop_body(id, l->back, prev, next, l->front)                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_##id(List_##id *l, ListEntry_##id *pos, t value) {              \
    if (!pos) {                                                                                              \
        list_push_back_##id(l, value);                                                                       \
        return l->back;                                                                                      \
    }                                                                                                        \
                                                                                                             \
    ListEntry_##id *prev = pos->prev;                                                                        \
    ListEntry_##id *new = __ds_calloc(1, sizeof(ListEntry_##id));                                            \
    new->data = value;                                                                                       \
    new->next = pos;                                                                                         \
    pos->prev = new;                                                                                         \
    new->prev = prev;                                                                                        \
    if (prev) {                                                                                              \
        prev->next = new;                                                                                    \
    } else {                                                                                                 \
        l->front = new;                                                                                      \
    }                                                                                                        \
    l->size++;                                                                                               \
    return new;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_sorted_##id(List_##id *l, t value) {                            \
    ListEntry_##id *curr = l->front;                                                                         \
                                                                                                             \
    if (!curr || ds_cmp_leq(cmp_lt, value, curr->data)) {                                                    \
        list_push_front_##id(l, value);                                                                      \
        return l->front;                                                                                     \
    } else {                                                                                                 \
        ListEntry_##id *prev = l->front;                                                                     \
        curr = curr->next;                                                                                   \
        while (curr != NULL) {                                                                               \
            if (ds_cmp_eq(cmp_lt, value, curr->data) ||                                                      \
               (cmp_lt(value, curr->data) && cmp_lt(prev->data, value))) {                                   \
                return list_insert_##id(l, curr, value);                                                     \
            }                                                                                                \
            prev = prev->next;                                                                               \
            curr = curr->next;                                                                               \
        }                                                                                                    \
        list_push_back_##id(l, value);                                                                       \
        return l->back;                                                                                      \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_fromArray_##id(List_##id *l, ListEntry_##id *pos, bool sorted, t *arr, size_t n) { \
    if (!arr || !n) return NULL;                                                                             \
                                                                                                             \
    ListEntry_##id *rv = NULL; /* ListEntry where first element from arr was inserted */                     \
    t *end = &arr[n];                                                                                        \
                                                                                                             \
    if (sorted) {                                                                                            \
        for (; arr != end; ++arr) {                                                                          \
            list_insert_sorted_##id(l, *arr);                                                                \
        }                                                                                                    \
        rv = l->front;                                                                                       \
    } else {                                                                                                 \
        ListEntry_##id *prev = pos ? pos->prev : NULL;                                                       \
        ListEntry_##id *first = __ds_calloc(1, sizeof(ListEntry_##id));                                      \
        first->data = *arr;                                                                                  \
        ++arr;                                                                                               \
        int count = 1;                                                                                       \
        ListEntry_##id *curr, *temp = first;                                                                 \
        while (arr != end) {                                                                                 \
            curr = __ds_calloc(1, sizeof(ListEntry_##id));                                                   \
            curr->data = *arr;                                                                               \
            curr->prev = temp;                                                                               \
            temp->next = curr;                                                                               \
            temp = curr;                                                                                     \
            ++arr;                                                                                           \
            ++count;                                                                                         \
        }                                                                                                    \
        ListEntry_##id *last = temp;                                                                         \
                                                                                                             \
        first->prev = prev;                                                                                  \
        last->next = pos;                                                                                    \
        if (prev) {                                                                                          \
            prev->next = first;                                                                              \
        } else if (!l->front) {                                                                              \
            l->front = first;                                                                                \
        }                                                                                                    \
        if (pos) {                                                                                           \
            pos->prev = last;                                                                                \
        } else if (!l->back) {                                                                               \
            l->back = last;                                                                                  \
        } else {                                                                                             \
            l->back->next = first;                                                                           \
            first->prev = l->back;                                                                           \
            l->back = last;                                                                                  \
        }                                                                                                    \
        rv = first;                                                                                          \
        l->size += (size_t) count;                                                                           \
    }                                                                                                        \
    return rv;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_insert_fromList_##id(List_##id *l, ListEntry_##id *pos, bool sorted, ListEntry_##id *start, ListEntry_##id *end) { \
    if (!start) return NULL;                                                                                 \
    ListEntry_##id *rv = NULL;                                                                               \
                                                                                                             \
    if (sorted) {                                                                                            \
        while (start != end) {                                                                               \
            list_insert_sorted_##id(l, start->data);                                                         \
            start = start->next;                                                                             \
        }                                                                                                    \
        rv = l->front;                                                                                       \
    } else {                                                                                                 \
        ListEntry_##id *prev = pos ? pos->prev : NULL;                                                       \
        ListEntry_##id *first = __ds_calloc(1, sizeof(ListEntry_##id));                                      \
        first->data = start->data;                                                                           \
        start = start->next;                                                                                 \
        int count = 1;                                                                                       \
        ListEntry_##id *curr = first, *temp = first;                                                         \
        while (start != end) {                                                                               \
            curr = __ds_calloc(1, sizeof(ListEntry_##id));                                                   \
            curr->data = start->data;                                                                        \
            curr->prev = temp;                                                                               \
            temp->next = curr;                                                                               \
            temp = curr;                                                                                     \
            start = start->next;                                                                             \
            ++count;                                                                                         \
        }                                                                                                    \
        ListEntry_##id *last = temp;                                                                         \
                                                                                                             \
        first->prev = prev;                                                                                  \
        last->next = pos;                                                                                    \
        if (prev) {                                                                                          \
            prev->next = first;                                                                              \
        } else if (!l->front) {                                                                              \
            l->front = first;                                                                                \
        }                                                                                                    \
        if (pos) {                                                                                           \
            pos->prev = last;                                                                                \
        } else if (!l->back) {                                                                               \
            l->back = last;                                                                                  \
        } else {                                                                                             \
            l->back->next = first;                                                                           \
            first->prev = l->back;                                                                           \
            l->back = last;                                                                                  \
        }                                                                                                    \
        rv = first;                                                                                          \
        l->size += (size_t) count;                                                                           \
    }                                                                                                        \
    return rv;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX ListEntry_##id *list_erase_##id(List_##id *l, ListEntry_##id *first, ListEntry_##id *last) { \
    if (!first || !l->front || first == last) return NULL;                                                   \
                                                                                                             \
    ListEntry_##id *before = first->prev;                                                                    \
    ListEntry_##id *tmp, *res;                                                                               \
                                                                                                             \
    while (first != last) {                                                                                  \
        tmp = first->next;                                                                                   \
        free(first);                                                                                         \
        first = tmp;                                                                                         \
        l->size--;                                                                                           \
    }                                                                                                        \
                                                                                                             \
    if (before) {                                                                                            \
        before->next = last;                                                                                 \
    } else {                                                                                                 \
        l->front = last;                                                                                     \
    }                                                                                                        \
                                                                                                             \
    if (last) {                                                                                              \
        res = last;                                                                                          \
        last->prev = before;                                                                                 \
    } else {                                                                                                 \
        res = NULL;                                                                                          \
        l->back = before;                                                                                    \
    }                                                                                                        \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_reverse_##id(List_##id *l) {                                                  \
    ListEntry_##id *newFront = l->back, *newBack = l->front;                                                 \
    ListEntry_##id *prev = NULL, *next = NULL;                                                               \
    ListEntry_##id *curr = l->front;                                                                         \
                                                                                                             \
    while (curr) {                                                                                           \
        prev = curr->prev;                                                                                   \
        next = curr->next;                                                                                   \
        curr->next = prev;                                                                                   \
        curr->prev = next;                                                                                   \
        curr = next;                                                                                         \
    }                                                                                                        \
    l->front = newFront;                                                                                     \
    l->back = newBack;                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_sort_##id(List_##id *l) {                                                         \
    if (l->front == l->back) {                                                                               \
        return;                                                                                              \
    } else if (l->size == 2 && cmp_lt(l->back->data, l->front->data)) {                                      \
        ListEntry_##id *temp = l->back;                                                                      \
        l->front = l->back;                                                                                  \
        l->back = temp;                                                                                      \
        l->front->prev = l->back->next = NULL;                                                               \
        l->front->next = l->back;                                                                            \
        l->back->prev = l->front;                                                                            \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    List_##id *carry = list_new(id);                                                                         \
    List_##id *tmp = __ds_calloc(64, sizeof(List_##id));                                                     \
    List_##id *fill = tmp;                                                                                   \
    List_##id *counter;                                                                                      \
                                                                                                             \
    do {                                                                                                     \
        list_splice_element(id, carry, carry->front, l, l->front);                                           \
                                                                                                             \
        for (counter = tmp; counter != fill && !(list_empty(counter)); ++counter) {                          \
            list_merge_##id(counter, carry);                                                                 \
            __list_swap(id, carry, counter)                                                                  \
        }                                                                                                    \
                                                                                                             \
        __list_swap(id, carry, counter)                                                                      \
        if (counter == fill) {                                                                               \
            ++fill;                                                                                          \
        }                                                                                                    \
    } while (!(list_empty(l)));                                                                              \
                                                                                                             \
    for (counter = tmp + 1; counter != fill; ++counter) {                                                    \
        list_merge_##id(counter, (counter - 1));                                                             \
    }                                                                                                        \
                                                                                                             \
    __list_swap(id, l, (fill - 1))                                                                           \
    list_free_##id(carry);                                                                                   \
    free(tmp);                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_unique_##id(List_##id *l) {                                                       \
    __list_elem_removal_body(id, l, ds_cmp_eq(cmp_lt, prev->data, curr->data), l->front->next, l->front)     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_remove_value_##id(List_##id *l, t val) {                                          \
    __list_elem_removal_body(id, l, ds_cmp_eq(cmp_lt, val, curr->data), l->front, NULL)                      \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_remove_if_##id(List_##id *l, int (*cond)(t*)) {                                   \
    __list_elem_removal_body(id, l, cond(&(curr->data)), l->front, NULL)                                     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL ListEntry_##id *list_find_##id(List_##id *l, t val) {                                   \
    ListEntry_##id *curr = l->front;                                                                         \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->data, val)) return curr;                                                 \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX List_##id *list_sublist_##id(List_##id *this, ListEntry_##id *first, ListEntry_##id *last) { \
    if (!this->front || !first || (first == last)) return NULL;                                              \
                                                                                                             \
    List_##id *sub = list_new(id);                                                                           \
                                                                                                             \
    while (first != last) {                                                                                  \
        list_push_back_##id(sub, first->data);                                                               \
        first = first->next;                                                                                 \
    }                                                                                                        \
    return sub;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_merge_##id(List_##id *this, List_##id *other) {                                   \
    if (!other || !other->front) { /* nothing to merge */                                                    \
        return;                                                                                              \
    } else if (!this->front) { /* "this" is empty, set it to other and return */                             \
        this->front = other->front;                                                                          \
        this->back = other->back;                                                                            \
        this->size = other->size;                                                                            \
        other->front = other->back = NULL;                                                                   \
        other->size = 0;                                                                                     \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                                            \
    ListEntry_##id *last1 = NULL, *last2 = NULL;                                                             \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(first2->data, first1->data)) {                                                            \
            ListEntry_##id *next = first2->next;                                                             \
            ListEntry_##id *prev = first1->prev;                                                             \
            if (prev) {                                                                                      \
                prev->next = first2;                                                                         \
            } else {                                                                                         \
                this->front = first2;                                                                        \
            }                                                                                                \
            first2->prev = prev;                                                                             \
            first2->next = first1;                                                                           \
            first1->prev = first2;                                                                           \
            first2 = next;                                                                                   \
        } else {                                                                                             \
            first1 = first1->next;                                                                           \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (first2 != last2) { /* still elements in first2, but have reached end of first1 */                    \
        first2->prev = this->back;                                                                           \
        this->back->next = first2;                                                                           \
        this->back = other->back;                                                                            \
    }                                                                                                        \
    this->size += other->size;                                                                               \
    other->front = other->back = NULL;                                                                       \
    other->size = 0;                                                                                         \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void list_splice_range_##id(List_##id *this, ListEntry_##id *position, List_##id *other, ListEntry_##id *first, ListEntry_##id *last) { \
    if (!other->front || !first || first == last) return;                                                    \
    ListEntry_##id *firstprev = first->prev;                                                                 \
    size_t count = 0;                                                                                        \
                                                                                                             \
    /* get number of elements */                                                                             \
    ListEntry_##id *curr = first;                                                                            \
    while (curr != last) {                                                                                   \
        ++count;                                                                                             \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
                                                                                                             \
    if (!this->front) {                                                                                      \
        this->front = first;                                                                                 \
        this->back = last ? last->prev : other->back;                                                        \
        this->front->prev = this->back->next = NULL;                                                         \
    } else if (!position) { /* append */                                                                     \
        this->back->next = first;                                                                            \
        first->prev = this->back;                                                                            \
        this->back = last ? last->prev : other->back;                                                        \
        this->back->next = NULL;                                                                             \
    } else {                                                                                                 \
        ListEntry_##id *prev = position->prev;                                                               \
        first->prev = prev;                                                                                  \
        position->prev = last ? last->prev : other->back;                                                    \
        position->prev->next = position;                                                                     \
        if (prev) {                                                                                          \
            prev->next = first;                                                                              \
        } else {                                                                                             \
            this->front = first;                                                                             \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (firstprev) {                                                                                         \
        firstprev->next = last;                                                                              \
    } else {                                                                                                 \
        other->front = last;                                                                                 \
    }                                                                                                        \
                                                                                                             \
    if (last) {                                                                                              \
        last->prev = firstprev;                                                                              \
    } else {                                                                                                 \
        other->back = firstprev;                                                                             \
    }                                                                                                        \
    this->size += count;                                                                                     \
    other->size -= count;                                                                                    \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * LIST ALGORITHM SECTION
 * -------------------------------------------------------------------------- */

/**
 * Creates a new `List` representing the union of this list and `other` (i.e. elements that are in 
 * this list, `other`, or both - all elements).
 *
 * @param   other   Second `List`.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_union_list(id, l, other) __set_union_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new `List` representing the intersection of this list and `other` (i.e. all elements 
 * that both lists have in common).
 *
 * @param   other   Second `List`.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_intersection_list(id, l, other) __set_intersection_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new `List` representing the difference of this list and `other` (i.e. all elements that 
 * are unique to this list).
 *
 * @param   other   Second `List`.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_difference_list(id, l, other) __set_difference_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new `List` representing the symmetric difference of this list and `other` (i.e. all 
 * elements that neither list has in common).
 *
 * @param   other   Second `List`.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_symmetric_difference_list(id, l, other) __set_symmetric_difference_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Determines whether this list contains each element in `other`.
 *
 * @param   other   Second `List`.
 *
 * @return          True if this list contains each element in `other`, false if not.
 */
#define includes_list(id, l, other) __includes_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Generates `List` code for a specified type and ID, including set functions.
 *
 * @param  id      ID to be used for the `List` and `ListEntry` types (must be unique).
 * @param  t       Type to be stored in the list.
 * @param  cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_list_withalg(id, t, cmp_lt)                                                                      \
gen_list(id, t, cmp_lt)                                                                                      \
__gen_alg_set_funcs(id, cmp_lt, List_##id, list_##id, list_new, ListEntry_##id *, iter_next_LIST, iter_deref_LIST, list_push_back, list_insert_fromList_##id(d_new, NULL, 0, first1, last1), list_insert_fromList_##id(d_new, NULL, 0, first2, last2)) \

#endif
