#ifndef LIST_H
#define LIST_H

#include "iterator.h"
#include "alg_helper.h"

#define LIST_END ((void*)-1)

#define __list_swap(id, x, y) {                                                                              \
    register ListEntry_##id *ltemp_front = (x)->front, *ltemp_back = (x)->back;                              \
    register unsigned ltemp_size = (x)->size;                                                                \
    (x)->front = (y)->front;                                                                                 \
    (x)->back = (y)->back;                                                                                   \
    (x)->size = (y)->size;                                                                                   \
    (y)->front = ltemp_front;                                                                                \
    (y)->back = ltemp_back;                                                                                  \
    (y)->size = ltemp_size;                                                                                  \
}

#define __list_elem_removal_body(id, this, condition, currStart, prevStart, deleteValue)                     \
    ListEntry_##id *curr, *prev, *next;                                                                      \
    if (!(this)->front) return;                                                                              \
                                                                                                             \
    curr = currStart, prev = prevStart;                                                                      \
                                                                                                             \
    while (curr) {                                                                                           \
        next = curr->next;                                                                                   \
        if (condition) {                                                                                     \
            if (prev) {                                                                                      \
                prev->next = next;                                                                           \
            } else {                                                                                         \
                (this)->front = next;                                                                        \
            }                                                                                                \
                                                                                                             \
            if (next) {                                                                                      \
                next->prev = prev;                                                                           \
            } else {                                                                                         \
                (this)->back = prev;                                                                         \
            }                                                                                                \
                                                                                                             \
            deleteValue((curr->data));                                                                       \
            free(curr);                                                                                      \
            (this)->size--;                                                                                  \
            curr = next;                                                                                     \
        } else {                                                                                             \
            prev = curr;                                                                                     \
            curr = curr->next;                                                                               \
        }                                                                                                    \
    }                                                                                                        \
    (this)->back = prev;                                                                                     \

#define __list_iterable_insert_body(id, this, pos, start, end, decls, earlyReturn, assignment, iter_next, getData, copyValue) \
    decls                                                                                                    \
    ListEntry_##id *prev = (pos) ? (pos)->prev : NULL;                                                       \
    ListEntry_##id *first, *last, *curr;                                                                     \
    unsigned count = 1;                                                                                      \
    earlyReturn                                                                                              \
    assignment                                                                                               \
    __ds_calloc(first, 1, sizeof(ListEntry_##id))                                                            \
    last = first;                                                                                            \
    copyValue((first->data), (getData(start)));                                                              \
    iter_next(id, start);                                                                                    \
    while ((start) != (end)) {                                                                               \
        __ds_calloc(curr, 1, sizeof(ListEntry_##id))                                                         \
        copyValue((curr->data), (getData(start)));                                                           \
        curr->prev = last;                                                                                   \
        last->next = curr;                                                                                   \
        last = curr;                                                                                         \
        iter_next(id, start);                                                                                \
        ++count;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    first->prev = prev;                                                                                      \
    last->next = pos;                                                                                        \
    if (prev) {                                                                                              \
        prev->next = first;                                                                                  \
    } else if ((pos) == (this)->front) {                                                                     \
        (this)->front = first;                                                                               \
    }                                                                                                        \
    if (pos) {                                                                                               \
        (pos)->prev = last;                                                                                  \
    } else {                                                                                                 \
        if ((this)->back) {                                                                                  \
            (this)->back->next = first;                                                                      \
            first->prev = (this)->back;                                                                      \
        }                                                                                                    \
        (this)->back = last;                                                                                 \
    }                                                                                                        \
    (this)->size += count;                                                                                   \
    return first;                                                                                            \

/* --------------------------------------------------------------------------
 * PRIMARY LIST SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the front element's data, if the list is not empty.
 */
#define list_front(this) ((this)->front ? &((this)->front->data) : NULL)


/**
 * Pointer to the back element's data, if the list is not empty.
 */
#define list_back(this) ((this)->back ? &((this)->back->data) : NULL)


/**
 * Whether the list has no elements.
 */
#define list_empty(this) (!((this)->front))


/**
 * The number of elements in the list.
 */
#define list_size(this) ((this)->size)


/**
 * Macro for iterating over the list from front to back.
 *
 * @param  it  @c ListEntry which is assigned to the current element. May be dereferenced with it->data.
 */
#define list_iter(this, it) for (it = iter_begin(LIST, 0, this, 0); it != iter_end(LIST, 0, this, 0); iter_next(LIST, 0, it))


/**
 * Macro for iterating over the list in reverse (from back to front).
 *
 * @param  it  @c ListEntry which is assigned to the current element. May be dereferenced with it->data.
 */
#define list_riter(this, it) for (it = iter_rbegin(LIST, 0, this, 0); it != iter_rend(LIST, 0, this, 0); iter_prev(LIST, 0, it))


/**
 * Creates a new, empty list.
 *
 * @return  Pointer to the newly allocated list.
 */
#define list_new(id) list_new_fromArray_##id(NULL, 0)


/**
 * Creates a new list with size @c n , where each element is set to @c value .
 *
 * @param   n      Number of elements to initialize.
 * @param   value  Value to set for each of the elements.
 *
 * @return         Pointer to the newly created list.
 */
#define list_new_repeatingValue(id, n, value) list_new_repeatingValue_##id(n, value)


/**
 * Creates a new list using @c n elements in a built-in array @c arr .
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly allocated list.
 */
#define list_new_fromArray(id, arr, n) list_new_fromArray_##id(arr, n)


/**
 * Creates a new list as a copy of @c other .
 *
 * @param   other  @c List to copy.
 *
 * @return         Pointer to the newly allocated list.
 */
#define list_createCopy(id, other) list_createCopy_##id(other)


/**
 * Deletes all elements and frees the list.
 */
#define list_free(id, this) do { list_erase(id, this, (this)->front, NULL); free(this); } while(0)


/**
 * Resizes the list to a size of @c n . If this is less than the current size, all but the first @c n 
 * elements are removed. If this is greater than the current size, elements are appended to the list 
 * with a value of 0.
 *
 * @param  n  The new list size.
 */
#define list_resize(id, this, n) list_resize_usingValue_##id(this, n, 0)


/**
 * Resizes the list to a size of @c n . If this is less than the current size, all but the first @c n 
 * elements are removed. If this is greater than the current size, elements are appended to the list 
 * with a value of @c value .
 *
 * @param  n      The new list size.
 * @param  value  Value to hold in the new elements if @c n is greater than the current size.
 */
#define list_resize_usingValue(id, this, n, value) list_resize_usingValue_##id(this, n, value)


/**
 * Prepends @c value to the start of the list.
 *
 * @param  value  Value to insert.
 */
#define list_push_front(id, this, value) list_insert(id, this, (this)->front, value)


/**
 * Appends @c value to the end of the list.
 *
 * @param  value  Value to insert.
 */
#define list_push_back(id, this, value) list_insert(id, this, NULL, value)


/**
 * Removes the first element from the list, if it is not empty.
 */
#define list_pop_front(id, this) list_erase_##id(this, (this)->front, (this)->front ? (this)->front->next : NULL)


/**
 * Removes the last element from the list, if it is not empty.
 */
#define list_pop_back(id, this) list_erase_##id(this, (this)->back, NULL)


/**
 * Inserts @c value before @c pos .
 *
 * @param   pos     @c ListEntry before which the element should be inserted. If this is NULL, the element
 *                    is appended.
 * @param   value   Value to insert.
 *
 * @return          @c ListEntry corresponding to the inserted element.
 */
#define list_insert(id, this, pos, value) list_insert_repeatingValue_##id(this, pos, 1, value)


/**
 * Inserts @c n copies of @c value before @c pos .
 *
 * @param   pos     @c ListEntry before which the elements should be inserted. If this is NULL, the
 *                    elements are appended.
 * @param   n       Number of copies of @c value to insert.
 * @param   value   Value to insert.
 *
 * @return          If successful, returns a @c ListEntry corresponding to the first inserted element.
 *                  If an error occurred, returns NULL.
 */
#define list_insert_repeatingValue(id, this, pos, n, value) list_insert_repeatingValue_##id(this, pos, n, value)


/**
 * Inserts @c n elements from the built-in array @c arr before @c pos .
 *
 * @param   pos  @c ListEntry before which the elements should be inserted. If this is NULL, the elements
 *                 are appended.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       If successful, returns a @c ListEntry corresponding to the first inserted element.
 *               If an error occurred, returns NULL.
 */
#define list_insert_fromArray(id, this, pos, arr, n) list_insert_fromArray_##id(this, pos, arr, n)


/**
 * Inserts new elements from another @c List in the range [@c start , @c end ) before @c pos .
 *
 * @param   pos    @c ListEntry before which the elements should be inserted. If this is NULL, the
 *                   elements are appended.
 * @param   start  First @c ListEntry to insert. Must not be NULL.
 * @param   end    @c ListEntry after the last entry to insert. If this is NULL, all elements from
 *                   @c start through the end of the other list will be inserted.
 *
 * @return         If successful, returns a @c ListEntry corresponding to the first inserted element.
 *                 If an error occurred, returns NULL.
 */
#define list_insert_fromList(id, this, pos, start, end) list_insert_fromList_##id(this, pos, start, end)


/**
 * Removes the element at @c pos .
 *
 * @param   pos  @c ListEntry to be removed.
 *
 * @return       If successful, returns a @c ListEntry corresponding to the element that was after @c pos ;
 *               if @c pos was the last element in the list, this is @c LIST_END. If an error occurred,
 *               returns NULL.
 */
#define list_remove(id, this, pos) list_erase_##id(this, pos, pos ? (pos)->next : NULL)


/**
 * Erases elements within the range [@c first , @c last ).
 *
 * @param   first  First @c ListEntry to be removed - must be provided.
 * @param   last   @c ListEntry after the last entry to be deleted. If this is NULL, all elements
 *                   from @c first through the end of the list will be removed.
 *
 * @return         If successful, returns a @c ListEntry corresponding to the element after the last
 *                 deleted element; if the last deleted element was the last element in the list, this
 *                 is @c LIST_END. If an error occurred, returns NULL.
 */
#define list_erase(id, this, first, last) list_erase_##id(this, first, last)


/**
 * Removes all elements from the list.
 */
#define list_clear(id, this) list_erase_##id(this, this->front, NULL)


/**
 * Reverses the list; thus what was the last element will now be the first.
 */
#define list_reverse(id, this) list_reverse_##id(this)


/**
 * Removes any elements satisfying @c condition .
 *
 * @param  condition  Function pointer to check if an element meets the condition.
 */
#define list_remove_if(id, this, condition) list_remove_if_##id(this, condition)


/**
 * Moves all elements from @c other into this list before @c pos .
 *
 * @param  pos    @c ListEntry in this list before which elements in @c other will be moved. If this
 *                  is NULL, elements from @c other will be appended to this list.
 * @param  other  Other @c List from which elements will be moved.
 */
#define list_splice(id, this, pos, other) list_splice_range_##id(this, pos, other, (other)->front, NULL)


/**
 * Moves @c entry from @c other into this list before @c pos .
 *
 * @param  pos     @c ListEntry in this list before which @c entry will be moved. If this is NULL,
 *                   @c entry is appended to this list.
 * @param  other   Other @c List from which @c entry will be moved.
 * @param  entry  @c ListEntry to move.
 */
#define list_splice_element(id, this, pos, other, entry) list_splice_range_##id(this, pos, other, entry, entry ? (entry)->next : NULL)


/**
 * Moves elements from @c other in the range [@c first , @c last ) into this list before @c pos .
 *
 * @param  pos    @c ListEntry in this list before which elements in @c other will be moved. If this
 *                  is NULL, elements from @c other will be appended to this list.
 * @param  other  Other @c List from which elements will be moved.
 * @param  first  First @c ListEntry from @c other to move.
 * @param  last   @c ListEntry after the last entry in @c other to move. If this is NULL, all entries
 *                  from @c first through the end of @c other are moved.
 */
#define list_splice_range(id, this, pos, other, first, last) list_splice_range_##id(this, pos, other, first, last)


/**
 * Generates @c List function declarations for the specified type and ID.
 *
 * @param  id           ID to be used for the @c List and @c ListEntry types (must be unique).
 * @param  t            Type to be stored in the list.
 */
#define gen_list_headers(id, t)                                                                              \
                                                                                                             \
typedef struct ListEntry_##id ListEntry_##id;                                                                \
struct ListEntry_##id {                                                                                      \
    ListEntry_##id *prev;                                                                                    \
    ListEntry_##id *next;                                                                                    \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    unsigned size;                                                                                           \
    ListEntry_##id *front;                                                                                   \
    ListEntry_##id *back;                                                                                    \
} List_##id;                                                                                                 \
                                                                                                             \
create_iterator_distance_helper(LIST, id, ListEntry_##id *)                                                  \
                                                                                                             \
ListEntry_##id *list_insert_repeatingValue_##id(List_##id *this, ListEntry_##id *pos, unsigned n, t value);  \
ListEntry_##id *list_insert_fromArray_##id(List_##id *this, ListEntry_##id *pos, t *arr, unsigned n);        \
ListEntry_##id *list_insert_fromList_##id(List_##id *this, ListEntry_##id *pos, ListEntry_##id *start, ListEntry_##id *end); \
List_##id *list_new_fromArray_##id(t *arr, unsigned size);                                                   \
List_##id *list_new_repeatingValue_##id(unsigned n, t value);                                                \
List_##id *list_createCopy_##id(List_##id *other);                                                           \
ListEntry_##id *list_erase_##id(List_##id *this, ListEntry_##id *first, ListEntry_##id *last);               \
void list_resize_usingValue_##id(List_##id *this, unsigned n, t value);                                      \
void list_reverse_##id(List_##id *this);                                                                     \
void list_remove_if_##id(List_##id *this, int (*cond)(t*));                                                  \
void list_splice_range_##id(List_##id *this, ListEntry_##id *position, List_##id *other, ListEntry_##id *first, ListEntry_##id *last); \


/**
 * Generates @c List function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_list_headers .
 * @param  t            Type used in @c gen_list_headers .
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the set.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_list_source(id, t, copyValue, deleteValue)                                                       \
                                                                                                             \
ListEntry_##id *list_insert_repeatingValue_##id(List_##id *this, ListEntry_##id *pos, unsigned n, t value) { \
    unsigned i;                                                                                              \
    if (!n) return NULL;                                                                                     \
    for (i = 0; i < n; ++i) {                                                                                \
        ListEntry_##id *prev, *new;                                                                          \
        __ds_calloc(new, 1, sizeof(ListEntry_##id))                                                          \
        copyValue((new->data), (value));                                                                     \
        if (!pos) {                                                                                          \
            if (!(this->front)) {                                                                            \
                this->front = this->back = new;                                                              \
            } else {                                                                                         \
                new->prev = this->back;                                                                      \
                this->back->next = new;                                                                      \
                this->back = new;                                                                            \
            }                                                                                                \
            ++this->size;                                                                                    \
            pos = new;                                                                                       \
            continue;                                                                                        \
        }                                                                                                    \
                                                                                                             \
        prev = pos->prev;                                                                                    \
        new->next = pos;                                                                                     \
        pos->prev = new;                                                                                     \
        new->prev = prev;                                                                                    \
        if (prev) {                                                                                          \
            prev->next = new;                                                                                \
        } else {                                                                                             \
            this->front = new;                                                                               \
        }                                                                                                    \
        ++this->size;                                                                                        \
        pos = new;                                                                                           \
    }                                                                                                        \
    return pos;                                                                                              \
}                                                                                                            \
                                                                                                             \
ListEntry_##id *list_insert_fromArray_##id(List_##id *this, ListEntry_##id *pos, t *arr, unsigned n) {       \
    __list_iterable_insert_body(id, this, pos, arr, end, t *end;, if (!arr || !n) return NULL;, end = &arr[n];, iter_next_ARR, iter_deref_ARR, copyValue) \
}                                                                                                            \
                                                                                                             \
ListEntry_##id *list_insert_fromList_##id(List_##id *this, ListEntry_##id *pos, ListEntry_##id *start, ListEntry_##id *end) { \
    __list_iterable_insert_body(id, this, pos, start, end, ____cds_do_nothing, if (!start || start == end) return NULL;, ____cds_do_nothing, iter_next_LIST, iter_deref_LIST, copyValue) \
}                                                                                                            \
                                                                                                             \
List_##id *list_new_fromArray_##id(t *arr, unsigned size) {                                                  \
    List_##id *l;                                                                                            \
    __ds_calloc(l, 1, sizeof(List_##id))                                                                     \
    list_insert_fromArray_##id(l, NULL, arr, size);                                                          \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
List_##id *list_new_repeatingValue_##id(unsigned n, t value) {                                               \
    List_##id *l = list_new(id);                                                                             \
    list_insert_repeatingValue_##id(l, NULL, n, value);                                                      \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
List_##id *list_createCopy_##id(List_##id *other) {                                                          \
    List_##id *l = list_new(id);                                                                             \
    list_insert_fromList_##id(l, NULL, other->front, NULL);                                                  \
    return l;                                                                                                \
}                                                                                                            \
                                                                                                             \
ListEntry_##id *list_erase_##id(List_##id *this, ListEntry_##id *first, ListEntry_##id *last) {              \
    ListEntry_##id *before, *tmp, *res;                                                                      \
    if (!first || !this->front || first == last) return NULL;                                                \
                                                                                                             \
    before = first->prev;                                                                                    \
                                                                                                             \
    while (first != last) {                                                                                  \
        tmp = first->next;                                                                                   \
        deleteValue((first->data));                                                                          \
        free(first);                                                                                         \
        first = tmp;                                                                                         \
        this->size--;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    if (before) {                                                                                            \
        before->next = last;                                                                                 \
    } else {                                                                                                 \
        this->front = last;                                                                                  \
    }                                                                                                        \
                                                                                                             \
    if (last) {                                                                                              \
        res = last;                                                                                          \
        last->prev = before;                                                                                 \
    } else {                                                                                                 \
        res = LIST_END;                                                                                      \
        this->back = before;                                                                                 \
    }                                                                                                        \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
void list_resize_usingValue_##id(List_##id *this, unsigned n, t value) {                                     \
    if (n == this->size) return;                                                                             \
    else if (n < this->size) {                                                                               \
        ListEntry_##id *first = this->front;                                                                 \
        const int toAdvance = (int) n;                                                                       \
        iter_advance_LIST(id, first, toAdvance);                                                             \
        list_erase_##id(this, first, NULL);                                                                  \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    list_insert_repeatingValue_##id(this, NULL, n - this->size, value);                                      \
}                                                                                                            \
                                                                                                             \
void list_reverse_##id(List_##id *this) {                                                                    \
    ListEntry_##id *newFront = this->back, *newBack = this->front, *curr = this->front, *prev = NULL, *next = NULL; \
                                                                                                             \
    while (curr) {                                                                                           \
        prev = curr->prev;                                                                                   \
        next = curr->next;                                                                                   \
        curr->next = prev;                                                                                   \
        curr->prev = next;                                                                                   \
        curr = next;                                                                                         \
    }                                                                                                        \
    this->front = newFront;                                                                                  \
    this->back = newBack;                                                                                    \
}                                                                                                            \
                                                                                                             \
void list_remove_if_##id(List_##id *this, int (*cond)(t*)) {                                                 \
    __list_elem_removal_body(id, this, cond(&(curr->data)), this->front, NULL, deleteValue)                  \
}                                                                                                            \
                                                                                                             \
void list_splice_range_##id(List_##id *this, ListEntry_##id *position, List_##id *other, ListEntry_##id *first, ListEntry_##id *last) { \
    ListEntry_##id *firstprev, *curr;                                                                        \
    unsigned count = 0;                                                                                      \
    if (!other->front || !first || first == last) return;                                                    \
                                                                                                             \
    /* get number of elements */                                                                             \
    firstprev = first->prev, curr = first;                                                                   \
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
 * LIST ALGORITHM/COMPARISONS SECTION
 * -------------------------------------------------------------------------- */

/**
 * Inserts @c value in sorted order.
 *
 * @param   value  Value to insert.
 *
 * @return         @c ListEntry corresponding to the inserted element.
 */
#define list_insert_sorted(id, this, value) list_insert_sorted_##id(this, value)


/**
 * Inserts @c n elements from the built-in array @c arr in sorted order.
 *
 * @param  arr  Pointer to the first element to insert.
 * @param  n    Number of elements to include.
 */
#define list_insert_fromArray_sorted(id, this, arr, n) list_insert_fromArray_sorted_##id(this, arr, n)


/**
 * Inserts elements from another @c List in the range [@c start , @c end ) into this list in sorted order.
 *
 * @param  start  First @c ListEntry to insert. Must not be NULL.
 * @param  end    @c ListEntry after the last entry to insert. If this is NULL, all elements from
 *                  @c start through the end of the other list will be inserted.
 */
#define list_insert_fromList_sorted(id, this, start, end) list_insert_fromList_sorted_##id(this, start, end)


/**
 * Scans the list and removes any adjacent elements that compare equal to the preceding element.
 * 
 * Example:
 *     Input : [1, 2, 2, 2, 3, 3]
 *     Output: [1, 2, 3]
 */
#define list_unique(id, this) list_unique_##id(this)


/**
 * Removes any elements equal to @c value .
 *
 * @param  value  Value to compare to a list element's data.
 */
#define list_remove_value(id, this, value) list_remove_value_##id(this, value)


/**
 * Finds the first instance of @c value .
 *
 * @param   value  Value to search for.
 *
 * @return         If @c value was found, returns a @c ListEntry corresponding to that element. If it
 *                 was not found, returns NULL.
 */
#define list_find(id, this, value) list_find_##id(this, value)


/**
 * Merges @c other into this list, both of which must be in sorted order prior to this operation. 
 * @c other is left with a size of 0, and this list grows by as many elements as @c other previously 
 * contained.
 *
 * @param  other  Other @c List , which will be merged with this list.
 */
#define list_merge(id, this, other) list_merge_##id(this, other)


/**
 * Sorts the list according to the @c cmp_lt macro provided in @c gen_list_source .
 * 
 * Time complexity: approx. O(n * log(n))
 */
#define list_sort(id, this) list_sort_##id(this)


/**
 * Creates a new @c List representing the union of this list and @c other (i.e. elements that are in 
 * this list, @c other , or both - all elements).
 *
 * @param   other   Second @c List .
 *
 * @return          Pointer to newly allocated list.
 */
#define set_union_list(id, this, other) __set_union_list_##id((this)->front, NULL, (other)->front, NULL)


/**
 * Creates a new @c List representing the intersection of this list and @c other (i.e. all elements 
 * that both lists have in common).
 *
 * @param   other   Second @c List .
 *
 * @return          Pointer to newly allocated list.
 */
#define set_intersection_list(id, this, other) __set_intersection_list_##id((this)->front, NULL, (other)->front, NULL)


/**
 * Creates a new @c List representing the difference of this list and @c other (i.e. all elements that 
 * are unique to this list).
 *
 * @param   other   Second @c List .
 *
 * @return          Pointer to newly allocated list.
 */
#define set_difference_list(id, this, other) __set_difference_list_##id((this)->front, NULL, (other)->front, NULL)


/**
 * Creates a new @c List representing the symmetric difference of this list and @c other (i.e. all 
 * elements that neither list has in common).
 *
 * @param   other   Second @c List .
 *
 * @return          Pointer to newly allocated list.
 */
#define set_symmetric_difference_list(id, this, other) __set_symmetric_difference_list_##id((this)->front, NULL, (other)->front, NULL)


/**
 * Determines whether this list contains each element in @c other .
 *
 * @param   other   Second @c List .
 *
 * @return          True if this list contains each element in @c other , false if not.
 */
#define includes_list(id, this, other) __includes_list_##id((this)->front, NULL, (other)->front, NULL)


/**
 * Generates @c List function definitions for the specified type and ID, including set, sorting, and comparison functions.
 *
 * @param  id  ID to be used for the @c List and @c ListEntry types (must be unique).
 * @param  t   Type to be stored in the list.
 */
#define gen_list_headers_withAlg(id, t)                                                                      \
                                                                                                             \
gen_list_headers(id, t)                                                                                      \
__gen_alg_set_func_headers(id, List_##id, list_##id, ListEntry_##id *)                                       \
                                                                                                             \
ListEntry_##id *list_insert_sorted_##id(List_##id *this, t value);                                           \
void list_insert_fromArray_sorted_##id(List_##id *this, t *arr, unsigned n);                                 \
void list_insert_fromList_sorted_##id(List_##id *this, ListEntry_##id *start, ListEntry_##id *end);          \
void list_unique_##id(List_##id *this);                                                                      \
void list_remove_value_##id(List_##id *this, t val);                                                         \
ListEntry_##id *list_find_##id(List_##id *this, t val);                                                      \
void list_merge_##id(List_##id *this, List_##id *other);                                                     \
void list_sort_##id(List_##id *this);                                                                        \


/**
 * Generates @c List function definitions for the specified type and ID, including set, sorting, and comparison functions.
 *
 * @param  id           ID used in @c gen_list_headers_withAlg .
 * @param  t            Type used in @c gen_list_headers_withAlg .
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the list.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_list_source_withAlg(id, t, cmp_lt, copyValue, deleteValue)                                       \
                                                                                                             \
gen_list_source(id, t, copyValue, deleteValue)                                                               \
__gen_alg_set_func_source(id, cmp_lt, List_##id, list_##id, list_new, ListEntry_##id *, iter_next_LIST, iter_deref_LIST, list_push_back, list_insert_fromList_##id(d_new, NULL, first1, last1), list_insert_fromList_##id(d_new, NULL, first2, last2)) \
                                                                                                             \
ListEntry_##id *list_insert_sorted_##id(List_##id *this, t value) {                                          \
    ListEntry_##id *curr = this->front, *prev;                                                               \
                                                                                                             \
    if (!curr || ds_cmp_leq(cmp_lt, value, curr->data)) {                                                    \
        list_push_front(id, this, value);                                                                    \
        return this->front;                                                                                  \
    }                                                                                                        \
    prev = this->front, curr = curr->next;                                                                   \
    while (curr != NULL) {                                                                                   \
        if (ds_cmp_eq(cmp_lt, value, curr->data) ||                                                          \
            (cmp_lt(value, curr->data) && cmp_lt(prev->data, value))) {                                      \
            return list_insert(id, this, curr, value);                                                       \
        }                                                                                                    \
        prev = prev->next, curr = curr->next;                                                                \
    }                                                                                                        \
    list_push_back(id, this, value);                                                                         \
    return this->back;                                                                                       \
}                                                                                                            \
                                                                                                             \
void list_insert_fromArray_sorted_##id(List_##id *this, t *arr, unsigned n) {                                \
    t *end;                                                                                                  \
    if (!arr || !n) return;                                                                                  \
    end = &arr[n];                                                                                           \
    for (; arr != end; ++arr) {                                                                              \
        list_insert_sorted_##id(this, *arr);                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void list_insert_fromList_sorted_##id(List_##id *this, ListEntry_##id *start, ListEntry_##id *end) {         \
    if (!start) return;                                                                                      \
    while (start != end) {                                                                                   \
        list_insert_sorted_##id(this, start->data);                                                          \
        start = start->next;                                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void list_unique_##id(List_##id *this) {                                                                     \
    __list_elem_removal_body(id, this, ds_cmp_eq(cmp_lt, prev->data, curr->data), this->front->next, this->front, deleteValue) \
}                                                                                                            \
                                                                                                             \
void list_remove_value_##id(List_##id *this, t val) {                                                        \
    __list_elem_removal_body(id, this, ds_cmp_eq(cmp_lt, val, curr->data), this->front, NULL, deleteValue)   \
}                                                                                                            \
                                                                                                             \
ListEntry_##id *list_find_##id(List_##id *this, t val) {                                                     \
    ListEntry_##id *curr = this->front;                                                                      \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->data, val)) return curr;                                                 \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
void list_merge_##id(List_##id *this, List_##id *other) {                                                    \
    ListEntry_##id *first1, *first2, *last1 = NULL, *last2 = NULL;                                           \
    if (!other->front) { /* nothing to merge */                                                              \
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
    first1 = this->front, first2 = other->front;                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(first2->data, first1->data)) {                                                            \
            ListEntry_##id *next = first2->next, *prev = first1->prev;                                       \
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
void list_sort_##id(List_##id *this) {                                                                       \
    List_##id *carry, *fill, *counter, tmp[64] = {0};                                                        \
    if (this->front == this->back) {                                                                         \
        return;                                                                                              \
    } else if (this->size == 2 && cmp_lt(this->back->data, this->front->data)) {                             \
        ListEntry_##id *temp = this->back;                                                                   \
        this->front = this->back;                                                                            \
        this->back = temp;                                                                                   \
        this->front->prev = this->back->next = NULL;                                                         \
        this->front->next = this->back;                                                                      \
        this->back->prev = this->front;                                                                      \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    carry = list_new(id);                                                                                    \
    fill = &tmp[0];                                                                                          \
                                                                                                             \
    do {                                                                                                     \
        list_splice_element(id, carry, carry->front, this, this->front);                                     \
                                                                                                             \
        for (counter = &tmp[0]; counter != fill && !(list_empty(counter)); ++counter) {                      \
            list_merge_##id(counter, carry);                                                                 \
            __list_swap(id, carry, counter)                                                                  \
        }                                                                                                    \
                                                                                                             \
        __list_swap(id, carry, counter)                                                                      \
        if (counter == fill) {                                                                               \
            ++fill;                                                                                          \
        }                                                                                                    \
    } while (!(list_empty(this)));                                                                           \
                                                                                                             \
    for (counter = &tmp[1]; counter != fill; ++counter) {                                                    \
        list_merge_##id(counter, (counter - 1));                                                             \
    }                                                                                                        \
                                                                                                             \
    __list_swap(id, this, (fill - 1))                                                                        \
    list_free(id, carry);                                                                                    \
}                                                                                                            \

#endif
