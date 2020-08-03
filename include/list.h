#ifndef LIST_H
#define LIST_H

#include "iterator.h"
#include "alg_helper.h"

#define LIST_ERROR(id) (DLLNode_##id *)(-1)
#define LIST_END(id) (DLLNode_##id *)(0)

#define __list_swap(id, x, y)                                                                                \
do {                                                                                                         \
    register DLLNode_##id *_ltemp_front = (x)->front;                                                        \
    register DLLNode_##id *_ltemp_back = (x)->back;                                                          \
    register size_t _ltemp_size = (x)->size;                                                                 \
    (x)->front = (y)->front;                                                                                 \
    (x)->back = (y)->back;                                                                                   \
    (x)->size = (y)->size;                                                                                   \
    (y)->front = _ltemp_front;                                                                               \
    (y)->back = _ltemp_back;                                                                                 \
    (y)->size = _ltemp_size;                                                                                 \
} while(0)

typedef enum {
    LIST_INIT_EMPTY,
    LIST_INIT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    LIST_INIT_LIST /* like List *nums */
} ListInitializer;

typedef enum {
    LIST_INSERT_SINGLE, /* like int */
    LIST_INSERT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    LIST_INSERT_LIST /* like List *nums */
} ListInsertType;

typedef enum {
    LIST_SPLICE_ALL, /* move entire list */
    LIST_SPLICE_SINGLE, /* move single element */
    LIST_SPLICE_RANGE /* move range of elements in [first, last) */
} ListSpliceType;

#define __init_list(id) list_new_##id(LIST_INIT_EMPTY)
#define __insert_single_list list_push_back
#define __insert_multi_1_list(id) __list_insert_list_##id(d_new, LIST_END(id), first1, last1, false)
#define __insert_multi_2_list(id) __list_insert_list_##id(d_new, LIST_END(id), first2, last2, false)


/* --------------------------------------------------------------------------
 * PRIMARY LIST SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the front element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_front(l) ((l)->front ? &((l)->front->data) : NULL)


/**
 * Pointer to the back element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_back(l) ((l)->back ? &((l)->back->data) : NULL)


/**
 * Whether the list has no elements.
 *
 * @param   l  The List struct pointer.
 */
#define list_empty(l) (!((l)->front))


/**
 * The number of elements in the list.
 *
 * @param   l  The List struct pointer.
 */
#define list_size(l) ((int) (l)->size)


/**
 * Macro for iterating over the list from front to back.
 *
 * @param   id    ID used with gen_list.
 * @param   l     The List struct pointer.
 * @param   ptr   ListIterator which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(LIST, ptr) or ptr->data.
 */
#define list_iter(id, l, ptr)                                                                                \
    for (ptr = iter_begin(LIST, id, l, 0); ptr != iter_end(LIST, id, l, 0); iter_next(LIST, id, ptr))


/**
 * Macro for iterating over the list in reverse (from back to front).
 *
 * @param   id    ID used with gen_list.
 * @param   l     The List struct pointer.
 * @param   ptr   ListIterator which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(LIST, ptr) or ptr->data.
 */
#define list_riter(id, l, ptr)                                                                               \
    for (ptr = iter_rbegin(LIST, id, l, 0); ptr != iter_rend(LIST, id, l, 0); iter_prev(LIST, id, ptr))


/**
 * Creates a new list.
 * In (1), an empty List is created.
 * In (2), a List is initialized from a built-in array data type, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total.
 * In (3), a List is initialized from another List.
 * 
 * (1) init = LIST_INIT_EMPTY:    list_new(id, ListInitializer init)
 * (2) init = LIST_INIT_BUILTIN:  list_new(id, ListInitializer init, t arr[], int n)
 * (3) init = LIST_INIT_LIST:     list_new(id, ListInitializer init, List_id *other)
 *
 * @param  id      ID used with gen_list.
 * @param  init    Type of initializer to execute.
 *
 * @return          A pointer to the newly allocated list.
 */
#define list_new(id, init, ...) list_new_##id(init, ##__VA_ARGS__)


/**
 * Deletes all elements and frees the list.
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_free(id, l) list_free_##id(l) 


/**
 * Prepends the provided element to the start of the list.
 *
 * @param  id     ID used with gen_list.
 * @param  l      Pointer to list.
 * @param  value  Pointer to the element to be inserted.
 */
#define list_push_front(id, l, value) __list_push_val_##id(l, value, true)


/**
 * Appends the provided element to the end of the list.
 *
 * @param  id     ID used with gen_list.
 * @param  l      Pointer to list.
 * @param  value  Pointer to the element to be inserted.
 */
#define list_push_back(id, l, value) __list_push_val_##id(l, value, false)


/**
 * Removes the first element from the list.
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_pop_front(id, l) __list_pop_val_##id(l, true)


/**
 * Removes the last element from the list.
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_pop_back(id, l) __list_pop_val_##id(l, false)


/**
 * Inserts new elements BEFORE the ListIterator at "pos".
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total.
 * In (3), elements from another List are inserted in the range [start, end); "start" must not be
 *   NULL. If "end" is LIST_END, then all elements from "start" through the end of the other list
 *   will be inserted.
 * 
 * (1) type = LIST_INSERT_SINGLE:   list_insert(id, List_id *l, DLLNode_id *pos, int sorted, ListInsertType type, t value)
 * (2) type = LIST_INSERT_BUILTIN:  list_insert(id, List_id *l, DLLNode_id *pos, int sorted, ListInsertType type, t arr[], int n)
 * (3) type = LIST_INSERT_LIST:     list_insert(id, List_id *l, DLLNode_id *pos, int sorted, ListInsertType type, DLLNode_id *start, DLLNode_id *end)
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to list.
 * @param   pos     ListIterator before which the element(s) should be inserted. If this is LIST_END,
 *                    it defaults to list_push_back.
 * @param   sorted  Whether elements should be inserted in sorted order. If this is true,
 *                    "pos" is ignored.
 * @param   type    Type of insertion to execute.
 *
 * @return         If successful, and "sorted" is false, returns a ListIterator corresponding to the
 *                   first inserted element. If "sorted" is true and multiple elements were
 *                   inserted, returns a ListIterator corresponding to the front of the list. If an
 *                   error occurred, returns LIST_ERROR.
 */
#define list_insert(id, l, pos, sorted, type, ...) list_insert_##id(l, pos, sorted, type, ##__VA_ARGS__)


/**
 * Erases elements from the list within the range [first, last). To erase a single element, an
 *   example function call would be list_erase(l, e, e->next).
 *
 * @param   id     ID used with gen_list.
 * @param   l      Pointer to list.
 * @param   first  ListIterator pointing to the first element to be removed - must be provided.
 * @param   last   ListIterator AFTER the last element to be deleted. If this is LIST_END, then
 *                   all elements from start through the end of the list will be removed.
 *
 * @return        If successful, returns a ListIterator corresponding to the element after the
 *                   last deleted element. If an error occurred, returns LIST_ERROR. If the last
 *                   deleted element was at the end of the list, returns LIST_END.
 */
#define list_erase(id, l, first, last) list_erase_##id(l, first, last)


/**
 * Removes all elements from the list.
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_clear(id, l) list_erase_##id(l, l->front, LIST_END(id))


/**
 * Reverses the list; thus what was the last element will now be the first.
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_reverse(id, l) list_reverse_##id(l)


/**
 * Sorts the list according to the cmp_lt macro provided in gen_list.
 * 
 * Time complexity: approx. O(n * log(n))
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_sort(id, l) list_sort_##id(l)


/**
 * Scans the list and removes any adjacent elements that compare equal to the preceding element.
 * 
 * Example:
 *     Input : [1, 2, 2, 2, 3, 3]
 *     Output: [1, 2, 3]
 *
 * @param  id  ID used with gen_list.
 * @param  l   Pointer to list.
 */
#define list_unique(id, l) _list_unique_##id(l)


/**
 * Removes any elements that compare equal to the provided value.
 *
 * @param  id   ID used with gen_list.
 * @param  l    Pointer to list.
 * @param  val  Pointer to the value to compare to a list element's data.
 */
#define list_remove_value(id, l, val) _list_remove_value_##id(l, val)


/**
 * Removes any elements satisfying the provided condition.
 *
 * @param  id         ID used with gen_list.
 * @param  l          Pointer to list.
 * @param  condition  Function pointer to check if an element's data meets the condition.
 */
#define list_remove_if(id, l, condition) _list_remove_if_##id(l, condition)


/**
 * Finds the first instance of the provided value.
 *
 * @param  id    ID used with gen_list.
 * @param   l    Pointer to list.
 * @param   val  Pointer to the value to compare to a list element's data.
 *
 * @return       If the value was found, returns a ListIterator pointing to that element. If it was
 *                  not found, returns NULL.
 */
#define list_find(id, l, val) list_find_##id(l, val)


/**
 * Creates a sublist from "l" in the range [first,last) (non-inclusive for "last").
 *
 * @param   id     ID used with gen_list.
 * @param   l      Pointer to list.
 * @param   first  Element to start the sublist.
 * @param   last   Element at which the sublist will stop. If this is LIST_END, the sublist will
 *                   include all elements from "first" to the end of the list.
 *
 * @return         Newly created sublist from the list. If "l" is empty or "first" is NULL,
 *                   returns NULL.
 */
#define list_sublist(id, l, first, last) list_sublist_##id(l, first, last)


/**
 * Merges "other" into "l", both of which must be in sorted order prior to this operation.
 *  "other" is left with a size of 0, and "l" grows by as many elements as "other" previously
 *  contained.
 *
 * @param   id     ID used with gen_list.
 * @param   l      Pointer to list.
 * @param   other  Pointer to other list, which will be merged with "l".
 */
#define list_merge(id, l, other) list_merge_##id(l, other)


/**
 * Moves elements from "other" before the ListIterator in "l" at "pos". No new elements are created;
 *  they are simply transferred from "other" into "l".
 * 
 * (1) type = LIST_SPLICE_ALL:     list_splice(id, List_id *this, DLLNode_id *position, List_id *other, LIST_SPLICE_ALL)
 * (2) type = LIST_SPLICE_SINGLE:  list_splice(id, List_id *this, DLLNode_id *position, List_id *other, LIST_SPLICE_SINGLE, DLLNode_id *elem)
 * (3) type = LIST_SPLICE_RANGE:   list_splice(id, List_id *this, DLLNode_id *position, List_id *other, LIST_SPLICE_RANGE, DLLNode_id *first, DLLNode_id *last)
 *
 * @param   id        ID used with gen_list.
 * @param   l         Pointer to list into which elements will be moved.
 * @param   position  Pointer to element in "l" before which elements in "other" will be
 *                      moved. If this is LIST_END, elements from "other" will be appended to
 *                      "l".
 * @param   other     Pointer to other list from which elements will be moved.
 * @param   type      Type of splice to perform.
 */
#define list_splice(id, l, position, other, type, ...) list_splice_##id(l, position, other, type, ##__VA_ARGS__)


/**
 * Generates list code for a specified type and ID.
 *
 * @param   id      ID to be used for the type stored in the list (must be unique).
 * @param   t       Type to be stored in the list.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_list(id, t, cmp_lt)                                                                              \
__gen_dllnode_list_typedef(id, t)                                                                            \
__gen_iter_LIST(id)                                                                                          \
__gen_list_declarations(id, t)                                                                               \
                                                                                                             \
__DS_FUNC_PREFIX List_##id *list_new_##id(ListInitializer init, ...) {                                       \
    List_##id *l = malloc(sizeof(List_##id));                                                                \
    if (!l) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(l, 0, sizeof(List_##id));                                                                         \
                                                                                                             \
    if (init == LIST_INIT_EMPTY) { /* nothing more to do in this case */                                     \
        return l;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    int n;                                                                                                   \
    void *other;                                                                                             \
                                                                                                             \
    /* parse arguments */                                                                                    \
    va_list args;                                                                                            \
    va_start(args, init);                                                                                    \
                                                                                                             \
    other = va_arg(args, void *);                                                                            \
                                                                                                             \
    if (init == LIST_INIT_BUILTIN) {                                                                         \
        n = va_arg(args, int);                                                                               \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    if (init == LIST_INIT_BUILTIN) {                                                                         \
        __list_insert_builtin_##id(l, LIST_END(id), (t *) other, n, false);                                  \
    } else {                                                                                                 \
        __list_insert_list_##id(l, LIST_END(id), ((List_##id *) other)->front, LIST_END(id), false);         \
    }                                                                                                        \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_free_##id(List_##id *l) {                                                     \
    list_erase_##id(l, l->front, LIST_END(id));                                                              \
    free(l);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __list_push_val_##id(List_##id *l, t value, bool front) {                          \
    DLLNode_##id *new = dll_node_new_##id();                                                                 \
    new->data = value;                                                                                       \
                                                                                                             \
    if (!(l->front)) {                                                                                       \
        l->front = new;                                                                                      \
        l->back = new;                                                                                       \
    } else {                                                                                                 \
        if (front) {                                                                                         \
            new->next = l->front;                                                                            \
            l->front->prev = new;                                                                            \
            l->front = new;                                                                                  \
        } else {                                                                                             \
            new->prev = l->back;                                                                             \
            l->back->next = new;                                                                             \
            l->back = new;                                                                                   \
        }                                                                                                    \
    }                                                                                                        \
    l->size++;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __list_pop_val_##id(List_##id *l, bool front) {                                    \
    if (!(l->front)) {                                                                                       \
        return;                                                                                              \
    }                                                                                                        \
    DLLNode_##id *repl = front ? l->front : l->back;                                                         \
                                                                                                             \
    if (front) {                                                                                             \
        l->front = repl->next;                                                                               \
        if (l->front) {                                                                                      \
            l->front->prev = NULL;                                                                           \
        } else {                                                                                             \
            l->back = NULL;                                                                                  \
        }                                                                                                    \
    } else {                                                                                                 \
        l->back = repl->prev;                                                                                \
        if (l->back) {                                                                                       \
            l->back->next = NULL;                                                                            \
        } else {                                                                                             \
            l->front = NULL;                                                                                 \
        }                                                                                                    \
    }                                                                                                        \
    free(repl);                                                                                              \
    l->size--;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DLLNode_##id *list_insert_##id(List_##id *l, DLLNode_##id *pos, bool sorted, ListInsertType type, ...) { \
    void *arr;                                                                                               \
    t value = (t) 0;                                                                                         \
    int n;                                                                                                   \
    void *l_start;                                                                                           \
    void *l_end;                                                                                             \
                                                                                                             \
    va_list args;                                                                                            \
    va_start(args, type);                                                                                    \
                                                                                                             \
    if (type == LIST_INSERT_SINGLE) {                                                                        \
        value = (t) (long) va_arg(args, void *);                                                             \
    } else if (type == LIST_INSERT_BUILTIN) {                                                                \
        arr = va_arg(args, void *);                                                                          \
        n = va_arg(args, int);                                                                               \
    } else {                                                                                                 \
        l_start = va_arg(args, void *);                                                                      \
        l_end = va_arg(args, void *);                                                                        \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    DLLNode_##id *rv = LIST_END(id);                                                                         \
                                                                                                             \
    switch (type) {                                                                                          \
        case LIST_INSERT_SINGLE:                                                                             \
            rv = sorted ? __list_insert_elem_sorted_##id(l, value) : __list_insert_elem_##id(l, pos, value); \
            break;                                                                                           \
        case LIST_INSERT_BUILTIN:                                                                            \
            rv = __list_insert_builtin_##id(l, pos, (t *) arr, n, sorted);                                   \
            break;                                                                                           \
        case LIST_INSERT_LIST:                                                                               \
            rv = __list_insert_list_##id(l, pos, (DLLNode_##id *) l_start, (DLLNode_##id *) l_end, sorted);  \
            break;                                                                                           \
    }                                                                                                        \
    return rv;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DLLNode_##id *list_erase_##id(List_##id *l, DLLNode_##id *first, DLLNode_##id *last) {      \
    if (!first || !l->front || (first == last)) {                                                            \
        return LIST_ERROR(id);                                                                               \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *before = first->prev;                                                                      \
    DLLNode_##id *tmp;                                                                                       \
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
    DLLNode_##id *res;                                                                                       \
                                                                                                             \
    if (last) {                                                                                              \
        res = last;                                                                                          \
        last->prev = before;                                                                                 \
    } else {                                                                                                 \
        res = LIST_END(id);                                                                                  \
        l->back = before;                                                                                    \
    }                                                                                                        \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void list_reverse_##id(List_##id *l) {                                                  \
    DLLNode_##id *newFront = l->back;                                                                        \
    DLLNode_##id *newBack = l->front;                                                                        \
    DLLNode_##id *prev = NULL;                                                                               \
    DLLNode_##id *curr = l->front;                                                                           \
    DLLNode_##id *next = NULL;                                                                               \
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
        DLLNode_##id *temp = l->back;                                                                        \
        l->front = l->back;                                                                                  \
        l->back = temp;                                                                                      \
        l->front->prev = l->back->next = NULL;                                                               \
        l->front->next = l->back;                                                                            \
        l->back->prev = l->front;                                                                            \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    List_##id *carry = list_new_##id(LIST_INIT_EMPTY);                                                       \
    List_##id *tmp = malloc(64 * sizeof(List_##id));                                                         \
    if (!tmp) DS_OOM();                                                                                      \
    memset(tmp, 0, 64 * sizeof(List_##id));                                                                  \
                                                                                                             \
    List_##id *fill = tmp;                                                                                   \
    List_##id *counter;                                                                                      \
                                                                                                             \
    do {                                                                                                     \
        list_splice_##id(carry, carry->front, l, LIST_SPLICE_SINGLE, l->front);                              \
                                                                                                             \
        for (counter = tmp; counter != fill && !(list_empty(counter)); ++counter) {                          \
            list_merge_##id(counter, carry);                                                                 \
            __list_swap(id, carry, counter);                                                                 \
        }                                                                                                    \
                                                                                                             \
        __list_swap(id, carry, counter);                                                                     \
        if (counter == fill) {                                                                               \
            ++fill;                                                                                          \
        }                                                                                                    \
    } while (!(list_empty(l)));                                                                              \
                                                                                                             \
    for (counter = tmp + 1; counter != fill; ++counter) {                                                    \
        list_merge_##id(counter, (counter - 1));                                                             \
    }                                                                                                        \
                                                                                                             \
    __list_swap(id, l, (fill - 1));                                                                          \
    list_free_##id(carry);                                                                                   \
    free(tmp);                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _list_unique_##id(List_##id *l) {                                                      \
    if (!(l->front)) {                                                                                       \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *curr = l->front->next;                                                                     \
    DLLNode_##id *prev = l->front;                                                                           \
    DLLNode_##id *next;                                                                                      \
                                                                                                             \
    while (curr) {                                                                                           \
        next = curr->next;                                                                                   \
        if (ds_cmp_eq(cmp_lt, prev->data, curr->data)) {                                                     \
            prev->next = next;                                                                               \
            if (next) {                                                                                      \
                next->prev = prev;                                                                           \
            } else {                                                                                         \
                l->back = prev;                                                                              \
            }                                                                                                \
                                                                                                             \
            free(curr);                                                                                      \
            l->size--;                                                                                       \
            curr = next;                                                                                     \
        } else {                                                                                             \
            prev = curr;                                                                                     \
            curr = curr->next;                                                                               \
        }                                                                                                    \
    }                                                                                                        \
    l->back = prev;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _list_remove_value_##id(List_##id *l, t val) {                                         \
    if (!(l->front)) {                                                                                       \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *curr = l->front;                                                                           \
    DLLNode_##id *prev =  NULL;                                                                              \
    DLLNode_##id *next;                                                                                      \
                                                                                                             \
    while (curr) {                                                                                           \
        next = curr->next;                                                                                   \
        if (ds_cmp_eq(cmp_lt, val, curr->data)) {                                                            \
            if (prev) {                                                                                      \
                prev->next = next;                                                                           \
            } else {                                                                                         \
                l->front = next;                                                                             \
            }                                                                                                \
                                                                                                             \
            if (next) {                                                                                      \
                next->prev = prev;                                                                           \
            } else {                                                                                         \
                l->back = prev;                                                                              \
            }                                                                                                \
                                                                                                             \
            free(curr);                                                                                      \
            l->size--;                                                                                       \
            curr = next;                                                                                     \
        } else {                                                                                             \
            prev = curr;                                                                                     \
            curr = curr->next;                                                                               \
        }                                                                                                    \
    }                                                                                                        \
    l->back = prev;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void _list_remove_if_##id(List_##id *l, meetsCondition_##id cond) {                         \
    if (!(l->front)) {                                                                                       \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *curr = l->front;                                                                           \
    DLLNode_##id *prev = NULL;                                                                               \
    DLLNode_##id *next;                                                                                      \
                                                                                                             \
    while (curr) {                                                                                           \
        next = curr->next;                                                                                   \
        if (cond(&(curr->data))) {                                                                           \
            if (prev) {                                                                                      \
                prev->next = next;                                                                           \
            } else {                                                                                         \
                l->front = next;                                                                             \
            }                                                                                                \
                                                                                                             \
            if (next) {                                                                                      \
                next->prev = prev;                                                                           \
            } else {                                                                                         \
                l->back = prev;                                                                              \
            }                                                                                                \
                                                                                                             \
            free(curr);                                                                                      \
            l->size--;                                                                                       \
            curr = next;                                                                                     \
        } else {                                                                                             \
            prev = curr;                                                                                     \
            curr = curr->next;                                                                               \
        }                                                                                                    \
    }                                                                                                        \
    l->back = prev;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL DLLNode_##id *list_find_##id(List_##id *l, t val) {                                     \
    DLLNode_##id *curr = l->front;                                                                           \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->data, val)) return curr;                                                 \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX List_##id *list_sublist_##id(List_##id *this, DLLNode_##id *first, DLLNode_##id *last) {    \
    if (!this->front || !first || (first == last)) {                                                         \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    List_##id *sub = list_new_##id(LIST_INIT_EMPTY);                                                         \
                                                                                                             \
    while (first != last) {                                                                                  \
        __list_push_val_##id(sub, first->data, false);                                                       \
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
    DLLNode_##id *first1 = this->front;                                                                      \
    DLLNode_##id *last1 = NULL;                                                                              \
    DLLNode_##id *first2 = other->front;                                                                     \
    DLLNode_##id *last2 = NULL;                                                                              \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(first2->data, first1->data)) {                                                            \
            DLLNode_##id *next = first2->next;                                                               \
            DLLNode_##id *prev = first1->prev;                                                               \
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
__DS_FUNC_PREFIX void list_splice_##id(List_##id *this, DLLNode_##id *position, List_##id *other, ListSpliceType type, ...) { \
    if (!other->front) return;                                                                               \
    else if (type == LIST_SPLICE_ALL) {                                                                      \
        __list_transfer_all_##id(this, position, other);                                                     \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    void *first = NULL;                                                                                      \
    void *last = NULL;                                                                                       \
                                                                                                             \
    va_list args;                                                                                            \
    va_start(args, type);                                                                                    \
                                                                                                             \
    first = va_arg(args, void *);                                                                            \
                                                                                                             \
    if (type == LIST_SPLICE_RANGE) {                                                                         \
        last = va_arg(args, void *);                                                                         \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    switch (type) {                                                                                          \
        case LIST_SPLICE_SINGLE:                                                                             \
            __list_transfer_single_##id(this, position, other, (DLLNode_##id *) first);                      \
            break;                                                                                           \
        default:                                                                                             \
            __list_transfer_range_##id(this, position, other, (DLLNode_##id *) first, (DLLNode_##id *) last);\
            break;                                                                                           \
    }                                                                                                        \
}                                                                                                            \
__gen_list_helper_func(id, t, cmp_lt)                                                                        \

#define __gen_dllnode_list_typedef(id, t)                                                                    \
typedef struct DLLNode_##id DLLNode_##id;                                                                    \
struct DLLNode_##id {                                                                                        \
    DLLNode_##id *prev;                                                                                      \
    DLLNode_##id *next;                                                                                      \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
DLLNode_##id *dll_node_new_##id(void) {                                                                      \
    DLLNode_##id *node = malloc(sizeof(DLLNode_##id));                                                       \
    if (!node) {                                                                                             \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(node, 0, sizeof(DLLNode_##id));                                                                   \
    return node;                                                                                             \
}                                                                                                            \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    DLLNode_##id *front;                                                                                     \
    DLLNode_##id *back;                                                                                      \
} List_##id;                                                                                                 \
                                                                                                             \
typedef int (*meetsCondition_##id)(t *val);                                                                  \

#define __gen_list_declarations(id, t)                                                                       \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_elem_##id(List_##id *l, DLLNode_##id *pos, t value);            \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_builtin_##id(List_##id *l, DLLNode_##id *pos, t *arr, int n, bool sorted); \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_list_##id(List_##id *l, DLLNode_##id *pos, DLLNode_##id *start, DLLNode_##id *end, bool sorted); \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_elem_sorted_##id(List_##id *l, t value);                        \
__DS_FUNC_PREFIX void __list_transfer_all_##id(List_##id *this, DLLNode_##id *position, List_##id *other);   \
__DS_FUNC_PREFIX void __list_transfer_single_##id(List_##id *this, DLLNode_##id *position, List_##id *other, DLLNode_##id *e); \
__DS_FUNC_PREFIX void __list_transfer_range_##id(List_##id *this, DLLNode_##id *position, List_##id *other, DLLNode_##id *first, DLLNode_##id *last); \
__DS_FUNC_PREFIX void __list_push_val_##id(List_##id *l, t value, bool front);                               \
__DS_FUNC_PREFIX DLLNode_##id *list_erase_##id(List_##id *l, DLLNode_##id *first, DLLNode_##id *last);       \
__DS_FUNC_PREFIX void list_merge_##id(List_##id *this, List_##id *other);                                    \
__DS_FUNC_PREFIX void list_splice_##id(List_##id *this, DLLNode_##id *position, List_##id *other, ListSpliceType type, ...); \
__DS_FUNC_PREFIX DLLNode_##id *__list_copy_builtin_##id(t *start, t *end, int *count, DLLNode_##id **last); \
__DS_FUNC_PREFIX DLLNode_##id *__list_copy_list_##id(DLLNode_##id *start, DLLNode_##id *end, int *count, DLLNode_##id **last); \

#define __gen_list_helper_func(id, t, cmp_lt)                                                                \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_elem_##id(List_##id *l, DLLNode_##id *pos, t value) {           \
    if (!pos) {                                                                                              \
        __list_push_val_##id(l, value, false);                                                               \
        return l->back;                                                                                      \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *prev = pos->prev;                                                                          \
    DLLNode_##id *new = dll_node_new_##id();                                                                 \
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
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_elem_sorted_##id(List_##id *l, t value) {                       \
    DLLNode_##id *curr = l->front;                                                                           \
                                                                                                             \
    if (!curr || ds_cmp_leq(cmp_lt, value, curr->data)) {                                                    \
        __list_push_val_##id(l, value, true);                                                                \
        return l->front;                                                                                     \
    } else {                                                                                                 \
        DLLNode_##id *prev = l->front;                                                                       \
        curr = curr->next;                                                                                   \
        while (curr != NULL) {                                                                               \
            if (ds_cmp_eq(cmp_lt, value, curr->data) ||                                                      \
               (cmp_lt(value, curr->data) && cmp_lt(prev->data, value))) {                                   \
                return __list_insert_elem_##id(l, curr, value);                                              \
            }                                                                                                \
            prev = prev->next;                                                                               \
            curr = curr->next;                                                                               \
        }                                                                                                    \
        __list_push_val_##id(l, value, false);                                                               \
        return l->back;                                                                                      \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DLLNode_##id *__list_copy_list_##id(DLLNode_##id *start, DLLNode_##id *end, int *count, DLLNode_##id **last) { \
    DLLNode_##id *first = dll_node_new_##id();                                                               \
    first->data = start->data;                                                                               \
    start = start->next;                                                                                     \
    ++(*count);                                                                                              \
    DLLNode_##id *curr = first;                                                                              \
    DLLNode_##id *temp = first;                                                                              \
    while (start != end) {                                                                                   \
        curr = dll_node_new_##id();                                                                          \
        curr->data = start->data;                                                                            \
        curr->prev = temp;                                                                                   \
        temp->next = curr;                                                                                   \
        temp = curr;                                                                                         \
        start = start->next;                                                                                 \
        ++(*count);                                                                                          \
    }                                                                                                        \
    *last = temp;                                                                                            \
    return first;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DLLNode_##id *__list_copy_builtin_##id(t *start, t *end, int *count, DLLNode_##id **last) { \
    DLLNode_##id *first = dll_node_new_##id();                                                               \
    first->data = *start;                                                                                    \
    ++start;                                                                                                 \
    ++(*count);                                                                                              \
    DLLNode_##id *curr;                                                                                      \
    DLLNode_##id *temp = first;                                                                              \
    while (start != end) {                                                                                   \
        curr = dll_node_new_##id();                                                                          \
        curr->data = *start;                                                                                 \
        curr->prev = temp;                                                                                   \
        temp->next = curr;                                                                                   \
        temp = curr;                                                                                         \
        ++start;                                                                                             \
        ++(*count);                                                                                          \
    }                                                                                                        \
    *last = temp;                                                                                            \
    return first;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_builtin_##id(List_##id *l, DLLNode_##id *pos, t *arr, int n, bool sorted) { \
    if (!arr || !n) {                                                                                        \
        return LIST_ERROR(id);                                                                               \
    }                                                                                                        \
                                                                                                             \
    DLLNode_##id *rv = LIST_END(id); /* ListIterator where first element from arr was inserted */            \
    t *end = &arr[n];                                                                                        \
                                                                                                             \
    if (sorted) {                                                                                            \
        for (; arr != end; ++arr) {                                                                          \
            __list_insert_elem_sorted_##id(l, *arr);                                                         \
        }                                                                                                    \
        rv = l->front;                                                                                       \
    } else {                                                                                                 \
        int count = 0;                                                                                       \
        DLLNode_##id *prev = pos ? pos->prev : NULL;                                                         \
        DLLNode_##id *last;                                                                                  \
        DLLNode_##id *first = __list_copy_builtin_##id(arr, end, &count, &last);                             \
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
__DS_FUNC_PREFIX DLLNode_##id *__list_insert_list_##id(List_##id *l, DLLNode_##id *pos, DLLNode_##id *start, DLLNode_##id *end, bool sorted) { \
    if (!start) {                                                                                            \
        return LIST_ERROR(id);                                                                               \
    }                                                                                                        \
    DLLNode_##id *rv = NULL;                                                                                 \
                                                                                                             \
    if (sorted) {                                                                                            \
        while (start != end) {                                                                               \
            __list_insert_elem_sorted_##id(l, start->data);                                                  \
            start = start->next;                                                                             \
        }                                                                                                    \
        rv = l->front;                                                                                       \
    } else {                                                                                                 \
        int count = 0;                                                                                       \
        DLLNode_##id *prev = pos ? pos->prev : NULL;                                                         \
        DLLNode_##id *last;                                                                                  \
        DLLNode_##id *first = __list_copy_list_##id(start, end, &count, &last);                              \
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
__DS_FUNC_PREFIX void __list_transfer_all_##id(List_##id *this, DLLNode_##id *position, List_##id *other) {  \
    if (!this->front) {                                                                                      \
        this->front = other->front;                                                                          \
        this->back = other->back;                                                                            \
    } else if (!position) {                                                                                  \
        this->back->next = other->front;                                                                     \
        other->front->prev = this->back;                                                                     \
    } else {                                                                                                 \
        DLLNode_##id *prev = position->prev;                                                                 \
        other->front->prev = prev;                                                                           \
        other->back->next = position;                                                                        \
        position->prev = other->back;                                                                        \
                                                                                                             \
        if (prev) {                                                                                          \
            prev->next = other->front;                                                                       \
        } else {                                                                                             \
            this->front = other->front;                                                                      \
        }                                                                                                    \
    }                                                                                                        \
    this->size += other->size;                                                                               \
    other->size = 0;                                                                                         \
    other->front = other->back = NULL;                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __list_transfer_single_##id(List_##id *this, DLLNode_##id *position, List_##id *other, DLLNode_##id *e) { \
    if (!e) return;                                                                                          \
    DLLNode_##id *eprev = e->prev;                                                                           \
    DLLNode_##id *enext = e->next;                                                                           \
    if (!this->front) {                                                                                      \
        this->front = this->back = e;                                                                        \
        this->front->prev = this->back->next = NULL;                                                         \
    } else if (!position) {                                                                                  \
        this->back->next = e;                                                                                \
        e->prev = this->back;                                                                                \
        this->back = e;                                                                                      \
        this->back->next = NULL;                                                                             \
    } else {                                                                                                 \
        DLLNode_##id *prev = position->prev;                                                                 \
        e->prev = prev;                                                                                      \
        e->next = position;                                                                                  \
        position->prev = e;                                                                                  \
        if (prev) {                                                                                          \
            prev->next = e;                                                                                  \
        } else {                                                                                             \
            this->front = e;                                                                                 \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (eprev) {                                                                                             \
        eprev->next = enext;                                                                                 \
    } else {                                                                                                 \
        other->front = enext;                                                                                \
    }                                                                                                        \
                                                                                                             \
    if (enext) {                                                                                             \
        enext->prev = eprev;                                                                                 \
    } else {                                                                                                 \
        other->back = eprev;                                                                                 \
    }                                                                                                        \
    other->size--;                                                                                           \
    this->size++;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __list_transfer_range_##id(List_##id *this, DLLNode_##id *position, List_##id *other, DLLNode_##id *first, DLLNode_##id *last) { \
    if (!first || first == last) return;                                                                     \
    DLLNode_##id *firstprev = first->prev;                                                                   \
    size_t count = 0;                                                                                        \
                                                                                                             \
    /* get number of elements */                                                                             \
    DLLNode_##id *curr = first;                                                                              \
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
        DLLNode_##id *prev = position->prev;                                                                 \
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
 * Creates a new List representing the union of "l" and "other" (i.e. elements that are in "l",
 * "other", or both - all elements).
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to first list.
 * @param   other   Pointer to second list.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_union_list(id, l, other) __set_union_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new List representing the intersection of "l" and "other" (i.e. all elements that both
 * lists have in common).
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to first list.
 * @param   other   Pointer to second list.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_intersection_list(id, l, other) __set_intersection_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new List representing the difference of "l" and "other" (i.e. all elements that are
 * unique to "l").
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to first list.
 * @param   other   Pointer to second list.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_difference_list(id, l, other) __set_difference_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Creates a new List representing the symmetric difference of "l" and "other" (i.e. all elements
 * that neither list has in common).
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to first list.
 * @param   other   Pointer to second list.
 *
 * @return          Pointer to newly allocated list.
 */
#define set_symmetric_difference_list(id, l, other) __set_symmetric_difference_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Determines whether "l" contains each element in "other".
 *
 * @param   id      ID used with gen_list.
 * @param   l       Pointer to first list.
 * @param   other   Pointer to second list.
 *
 * @return          True if "l" contains each element in "other", false if not.
 */
#define includes_list(id, l, other) __includes_list_##id((l)->front, NULL, (other)->front, NULL)


/**
 * Generates list code for a specified type and ID, including set functions.
 *
 * @param   id      ID to be used for the type stored in the list (must be unique).
 * @param   t       Type to be stored in the list.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_list_withalg(id, t, cmp_lt)                                                                      \
gen_list(id, t, cmp_lt)                                                                                      \
__gen_alg_set_funcs(id, cmp_lt, List_##id, list_##id, __init_list, DLLNode_##id *, __iter_next_LIST, __iter_deref_LIST, __insert_single_list, __insert_multi_1_list, __insert_multi_2_list) \

#endif
