#ifndef DS_LIST_H
#define DS_LIST_H

#include "ds.h"

#define LIST_END ((void*)-1)

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * Returns the next entry, if it exists.
 *
 * @param   e  @c ListEntry* : Entry to use.
 *
 * @return     @c ListEntry* : Next entry.
 */
#define listEntry_getNext(e) ((e) ? (e)->next : NULL)


/**
 * Returns the previous entry, if it exists.
 *
 * @param   e  @c ListEntry* : Entry to use.
 *
 * @return     @c ListEntry* : Previous entry.
 */
#define listEntry_getPrev(e) ((e) ? (e)->prev : NULL)


/**
 * Advances the entry by @c n positions. A negative number means to move 
 * backwards.
 *
 * @param  e  @c ListEntry** : Address of entry.
 * @param  n  @c long : Number of positions to advance.
 */
#define listEntry_advance(id, e, n) listEntry_advance_##id(e, n)


/**
 * Returns the number of elements between @c first and @c last .
 *
 * @param   first  @c ListEntry* : Entry to start at.
 * @param   last   @c ListEntry* : Entry to end at. This must be reachable in
 *                  the forward direction by @c first .
 *
 * @return         @c long : Number of elements between @c first and @c last ,
 *                 or if @c last is not reachable, returns -1.
 */
#define listEntry_distance(id, first, last)                                              \
        listEntry_distance_##id(first, last)


/**
 * @brief @c ListEntry* : The starting point for iterating over elements in 
 * forward order.
 */
#define list_iterator_begin(this) (this)->front


/**
 * @brief @c ListEntry* : The ending point for iterating over elements in 
 * forward order.
 */
#define list_iterator_end() NULL


/**
 * @brief @c ListEntry* : The starting point for iterating over elements in 
 * reverse order.
 */
#define list_iterator_rbegin(this) (this)->back


/**
 * @brief @c ListEntry* : The ending point for iterating over elements in 
 * reverse order.
 */
#define list_iterator_rend() NULL


/**
 * Macro for iterating over the list from front to back.
 *
 * @param  it  @c ListEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define list_iter(this, it) for (it = (this)->front; it; it = (it)->next)


/**
 * Macro for iterating over the list in reverse (from back to front).
 *
 * @param  it  @c ListEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define list_riter(this, it) for (it = (this)->back; it; it = (it)->prev)

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * @brief @c t* : Pointer to the front element's data, if the list is not 
 * empty.
 */
#define list_front(this) ((this)->front ? &(this)->front->data : NULL)


/**
 * @brief @c t* : Pointer to the back element's data, if the list is not empty.
 */
#define list_back(this) ((this)->back ? &(this)->back->data : NULL)


/**
 * @brief @c bool : Whether the list has no elements.
 */
#define list_empty(this) !(this)->front


/**
 * @brief @c unsigned : The number of elements in the list.
 */
#define list_size(this) (this)->size

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty list.
 *
 * @return  @c List* : Newly allocated list.
 */
#define list_new(id) list_new_fromArray_##id(NULL, 0)


/**
 * Creates a new list with size @c n , where each element is set to @c value .
 *
 * @param   n      @c unsigned : Number of elements to initialize.
 * @param   value  @c t : Value to set for each of the elements.
 *
 * @return         @c List* : Newly created list.
 */
#define list_new_repeatingValue(id, n, value)                                            \
        list_new_repeatingValue_##id(n, value)


/**
 * Creates a new list using @c n elements in a built-in array @c arr .
 *
 * @param   arr  @c t* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c List* : Newly allocated list.
 */
#define list_new_fromArray(id, arr, n) list_new_fromArray_##id(arr, n)


/**
 * Creates a new list as a copy of @c other .
 *
 * @param   other  @c List* : List to copy.
 *
 * @return         @c List* : Newly allocated list.
 */
#define list_createCopy(id, other) list_createCopy_##id(other)


/**
 * Deletes all elements and frees the list.
 */
#define list_free(id, this) do {                                                         \
    list_erase(id, this, (this)->front, NULL); free(this);                               \
} while(0)


/**
 * Resizes the list to a size of @c n . If this is less than the current size, 
 * all but the first @c n elements are removed. If this is greater than the 
 * current size, elements are appended to the list with a value of 0.
 *
 * @param   n  @c unsigned : The new list size.
 *
 * @return     @c bool : Whether the operation succeeded.
 */
#define list_resize(id, this, n) list_resize_usingValue_##id(this, n, 0)


/**
 * Resizes the list to a size of @c n . If this is less than the current size, 
 * all but the first @c n elements are removed. If this is greater than the 
 * current size, elements are appended to the list with a value of @c value .
 *
 * @param   n      @c unsigned : The new list size.
 * @param   value  @c t : Value to hold in the new elements if @c n is greater
 *                  than the current size.
 *
 * @return         @c bool : Whether the operation succeeded.
 */
#define list_resize_usingValue(id, this, n, value)                                       \
        list_resize_usingValue_##id(this, n, value)


/**
 * Prepends @c value to the start of the list.
 *
 * @param  value  @c t : Value to insert.
 */
#define list_push_front(id, this, value)                                                 \
        list_insert(id, this, (this)->front, value)


/**
 * Appends @c value to the end of the list.
 *
 * @param  value  @c t : Value to insert.
 */
#define list_push_back(id, this, value) list_insert(id, this, NULL, value)


/**
 * Removes the first element from the list, if it is not empty.
 */
#define list_pop_front(id, this)                                                         \
        list_erase_##id(this, (this)->front,                                             \
            (this)->front ? (this)->front->next : NULL)


/**
 * Removes the last element from the list, if it is not empty.
 */
#define list_pop_back(id, this) list_erase_##id(this, (this)->back, NULL)


/**
 * Inserts @c value before @c pos .
 *
 * @param   pos     @c ListEntry* : Entry before which the element should be
 *                   inserted. If this is NULL, the element is appended.
 * @param   value   @c t : Value to insert.
 *
 * @return          @c ListEntry* : Entry corresponding to the inserted
 *                  element.
 */
#define list_insert(id, this, pos, value)                                                \
        list_insert_repeatingValue_##id(this, pos, 1, value)


/**
 * Inserts @c n copies of @c value before @c pos .
 *
 * @param   pos     @c ListEntry* : Entry before which the elements should be
 *                   inserted. If this is NULL, the elements are appended.
 * @param   n       @c unsigned : Number of copies of @c value to insert.
 * @param   value   @c t : Value to insert.
 *
 * @return          @c ListEntry* : If successful, returns an entry
 *                  corresponding to the first inserted element. If an error
 *                  occurred, returns NULL.
 */
#define list_insert_repeatingValue(id, this, pos, n, value)                              \
        list_insert_repeatingValue_##id(this, pos, n, value)


/**
 * Inserts @c n elements from the built-in array @c arr before @c pos .
 *
 * @param   pos  @c ListEntry* : Entry before which the elements should be
 *                inserted. If this is NULL, the elements are appended.
 * @param   arr  @c t* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c ListEntry* : If successful, returns an entry corresponding
 *               to the first inserted element. If an error occurred, returns
 *               NULL.
 */
#define list_insert_fromArray(id, this, pos, arr, n)                                     \
        list_insert_fromArray_##id(this, pos, arr, n)


/**
 * Inserts new elements from another list in the range [ @c start , @c end ) 
 * before @c pos .
 *
 * @param   pos    @c ListEntry* : Entry before which the elements should be
 *                  inserted. If this is NULL, the elements are appended.
 * @param   start  @c ListEntry* : First entry to insert. Must not be NULL.
 * @param   end    @c ListEntry* : Entry after the last entry to insert. If
 *                  this is NULL, all elements from @c start through the end of
 *                  the other list will be inserted.
 *
 * @return         @c ListEntry* : If successful, returns an entry
 *                 corresponding to the first inserted element. If an error
 *                 occurred, returns NULL.
 */
#define list_insert_fromList(id, this, pos, start, end)                                  \
        list_insert_fromList_##id(this, pos, start, end)


/**
 * Removes the element at @c pos .
 *
 * @param   pos  @c ListEntry* : Entry to be removed.
 *
 * @return       @c ListEntry* : If successful, returns an entry corresponding
 *               to the element that was after @c pos ; if @c pos was the last
 *               element in the list, this is @c LIST_END . If an error
 *               occurred, returns NULL.
 */
#define list_remove(id, this, pos)                                                       \
        list_erase_##id(this, pos, pos ? (pos)->next : NULL)


/**
 * Erases elements within the range [ @c first , @c last ).
 *
 * @param   first  @c ListEntry* : First entry to be removed - must be
 *                  provided.
 * @param   last   @c ListEntry* : Entry after the last entry to be deleted. If
 *                  this is NULL, all elements from @c first through the end of
 *                  the list will be removed.
 *
 * @return         @c ListEntry* : If successful, returns an entry
 *                 corresponding to the element after the last deleted element;
 *                 if the last deleted element was the last element in the
 *                 list, this is @c LIST_END . If an error occurred, returns
 *                 NULL.
 */
#define list_erase(id, this, first, last) list_erase_##id(this, first, last)


/**
 * Removes all elements from the list.
 */
#define list_clear(id, this) list_erase_##id(this, (this)->front, NULL)


/**
 * Reverses the list; thus what was the last element will now be the first.
 */
#define list_reverse(id, this) list_reverse_##id(this)


/**
 * Removes any elements satisfying @c condition .
 *
 * @param  condition  @c int*(t*) : Function pointer to check if an element
 *                     meets the condition.
 */
#define list_remove_if(id, this, condition)                                              \
        list_remove_if_##id(this, condition)


/**
 * Moves all elements from @c other into this list before @c pos .
 *
 * @param  pos    @c ListEntry* : Entry in this list before which elements in
 *                 @c other will be moved. If this is NULL, elements from
 *                 @c other will be appended to this list.
 * @param  other  @c List* : Other list from which elements will be moved.
 */
#define list_splice(id, this, pos, other)                                                \
        list_splice_range_##id(this, pos, other, (other)->front, NULL)


/**
 * Moves @c entry from @c other into this list before @c pos .
 *
 * @param  pos    @c ListEntry* : Entry in this list before which @c entry will
 *                 be moved. If this is NULL, @c entry is appended to this
 *                 list.
 * @param  other  @c List* : Other list from which @c entry will be moved.
 * @param  entry  @c ListEntry* : Entry to move.
 */
#define list_splice_element(id, this, pos, other, entry)                                 \
        list_splice_range_##id(this, pos, other, entry,                                  \
            entry ? (entry)->next : NULL)


/**
 * Moves elements from @c other in the range [ @c first , @c last ) into this 
 * list before @c pos .
 *
 * @param  pos    @c ListEntry* : Entry in this list before which elements in
 *                 @c other will be moved. If this is NULL, elements from
 *                 @c other will be appended to this list.
 * @param  other  @c List* : Other list from which elements will be moved.
 * @param  first  @c ListEntry* : First entry from @c other to move.
 * @param  last   @c ListEntry* : Entry after the last entry in @c other to
 *                 move. If this is NULL, all entries from @c first through the
 *                 end of @c other are moved.
 */
#define list_splice_range(id, this, pos, other, first, last)                             \
        list_splice_range_##id(this, pos, other, first, last)


/**
 * Generates @c List function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the @c List and @c ListEntry types (must be
 *              unique).
 * @param  t   Type to be stored in the list.
 */
#define gen_list_headers(id, t)                                                          \
                                                                                         \
typedef struct ListEntry_##id ListEntry_##id;                                            \
struct ListEntry_##id {                                                                  \
    ListEntry_##id *prev;                                                                \
    ListEntry_##id *next;                                                                \
    t data;                                                                              \
};                                                                                       \
                                                                                         \
typedef struct {                                                                         \
    unsigned size;                                                                       \
    ListEntry_##id *front;                                                               \
    ListEntry_##id *back;                                                                \
} List_##id;                                                                             \
                                                                                         \
void listEntry_advance_##id(ListEntry_##id **p1, long n)                                 \
  __attribute__((nonnull));                                                              \
long listEntry_distance_##id(ListEntry_##id const *p1,                                   \
                             ListEntry_##id const *p2);                                  \
                                                                                         \
ListEntry_##id *list_insert_repeatingValue_##id(List_##id *this,                         \
                                                ListEntry_##id *pos,                     \
                                                unsigned n, t const value)               \
  __attribute__((nonnull (1)));                                                          \
ListEntry_##id *list_insert_fromArray_##id(List_##id *this,                              \
                                           ListEntry_##id *pos,                          \
                                           t const *arr, unsigned n)                     \
  __attribute__((nonnull (1,3)));                                                        \
ListEntry_##id *list_insert_fromList_##id(List_##id *this,                               \
                                          ListEntry_##id *pos,                           \
                                          ListEntry_##id const *start,                   \
                                          ListEntry_##id const *end)                     \
  __attribute__((nonnull (1)));                                                          \
List_##id *list_new_fromArray_##id(t const *arr, unsigned size);                         \
List_##id *list_new_repeatingValue_##id(unsigned n, t const value)                       \
  __attribute__((nonnull));                                                              \
List_##id *list_createCopy_##id(List_##id const *other)                                  \
  __attribute__((nonnull));                                                              \
ListEntry_##id *list_erase_##id(List_##id *this,                                         \
                                ListEntry_##id *first, ListEntry_##id *last)             \
  __attribute__((nonnull (1)));                                                          \
unsigned char list_resize_usingValue_##id(List_##id *this,                               \
                                          unsigned n, t value)                           \
  __attribute__((nonnull (1)));                                                          \
void list_reverse_##id(List_##id *this) __attribute__((nonnull));                        \
void list_remove_if_##id(List_##id *this, int (*cond)(t*))                               \
  __attribute__((nonnull));                                                              \
void list_splice_range_##id(List_##id *this,                                             \
                            ListEntry_##id *position, List_##id *other,                  \
                            ListEntry_##id *first, ListEntry_##id *last)                 \
  __attribute__((nonnull (1,3)));                                                        \


/**
 * Generates @c List function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_list_headers .
 * @param  t            Type used in @c gen_list_headers .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the list.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form @c (x) which is a complement to
 *                       @c copyValue ; if memory was dynamically allocated in
 *                       @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_list_source(id, t, copyValue, deleteValue)                                   \
                                                                                         \
void listEntry_advance_##id(ListEntry_##id **p1, long n) {                               \
    long count = 0;                                                                      \
    if (n >= 0) {                                                                        \
        for (; count < n && *p1; *p1 = (*p1)->next, ++count);                            \
    } else {                                                                             \
        for (; count > n && *p1; *p1 = (*p1)->prev, --count);                            \
    }                                                                                    \
}                                                                                        \
                                                                                         \
long listEntry_distance_##id(ListEntry_##id const *p1,                                   \
                             ListEntry_##id const *p2) {                                 \
    long dist;                                                                           \
    for (dist = 0; p1 && p1 != p2; p1 = p1->next, ++dist);                               \
    return (p1 == p2) ? dist : -1;                                                       \
}                                                                                        \
                                                                                         \
ListEntry_##id *list_insert_repeatingValue_##id(List_##id *this,                         \
                                                ListEntry_##id *pos,                     \
                                                unsigned n, t const value) {             \
    unsigned i = 1;                                                                      \
    ListEntry_##id *rv, *first, *last, *curr;                                            \
    if (n + this->size <= this->size ||                                                  \
            !(first = calloc(1, sizeof(ListEntry_##id)))) return NULL;                   \
                                                                                         \
    rv = first;                                                                          \
    copyValue(first->data, value);                                                       \
    last = first;                                                                        \
    for (; i < n; ++i, curr->prev = last, last->next = curr, last = curr) {              \
        if (!(curr = calloc(1, sizeof(ListEntry_##id)))) {                               \
            rv = NULL;                                                                   \
            break;                                                                       \
        }                                                                                \
        copyValue(curr->data, value);                                                    \
    }                                                                                    \
                                                                                         \
    first->prev = pos ? pos->prev : NULL;                                                \
    last->next = pos;                                                                    \
    if (first->prev) {                                                                   \
        first->prev->next = first;                                                       \
    } else if (pos == this->front) {                                                     \
        this->front = first;                                                             \
    }                                                                                    \
    if (pos) {                                                                           \
        pos->prev = last;                                                                \
    } else {                                                                             \
        if (this->back) {                                                                \
            this->back->next = first;                                                    \
            first->prev = this->back;                                                    \
        }                                                                                \
        this->back = last;                                                               \
    }                                                                                    \
    this->size += i;                                                                     \
    return rv;                                                                           \
}                                                                                        \
                                                                                         \
ListEntry_##id *list_insert_fromArray_##id(List_##id *this,                              \
                                           ListEntry_##id *pos,                          \
                                           t const *arr, unsigned n) {                   \
    unsigned i = 1;                                                                      \
    ListEntry_##id *rv, *first, *last, *curr;                                            \
    if (n + this->size <= this->size ||                                                  \
            !(first = calloc(1, sizeof(ListEntry_##id)))) return NULL;                   \
                                                                                         \
    rv = first;                                                                          \
    copyValue(first->data, *arr);                                                        \
    last = first;                                                                        \
    for (; i < n; ++i, curr->prev = last, last->next = curr, last = curr) {              \
        if (!(curr = calloc(1, sizeof(ListEntry_##id)))) {                               \
            rv = NULL;                                                                   \
            break;                                                                       \
        }                                                                                \
        copyValue(curr->data, arr[i]);                                                   \
    }                                                                                    \
                                                                                         \
    first->prev = pos ? pos->prev : NULL;                                                \
    last->next = pos;                                                                    \
    if (first->prev) {                                                                   \
        first->prev->next = first;                                                       \
    } else if (pos == this->front) {                                                     \
        this->front = first;                                                             \
    }                                                                                    \
    if (pos) {                                                                           \
        pos->prev = last;                                                                \
    } else {                                                                             \
        if (this->back) {                                                                \
            this->back->next = first;                                                    \
            first->prev = this->back;                                                    \
        }                                                                                \
        this->back = last;                                                               \
    }                                                                                    \
    this->size += i;                                                                     \
    return rv;                                                                           \
}                                                                                        \
                                                                                         \
ListEntry_##id *list_insert_fromList_##id(List_##id *this,                               \
                                          ListEntry_##id *pos,                           \
                                          ListEntry_##id const *start,                   \
                                          ListEntry_##id const *end) {                   \
    ListEntry_##id *rv, *first, *last, *curr;                                            \
    unsigned newSize = this->size + 1;                                                   \
    if (!start || start == end || !newSize ||                                            \
            !(first = calloc(1, sizeof(ListEntry_##id)))) return NULL;                   \
                                                                                         \
    rv = first;                                                                          \
    copyValue(first->data, start->data);                                                 \
    last = first;                                                                        \
    start = start->next;                                                                 \
    for (; start != end; start = start->next, curr->prev = last,                         \
            last->next = curr, last = curr) {                                            \
        if (++newSize == 0 || !(curr = calloc(1, sizeof(ListEntry_##id)))) {             \
            if (!newSize) newSize = UINT_MAX;                                            \
            rv = NULL;                                                                   \
            break;                                                                       \
        }                                                                                \
        copyValue(curr->data, start->data);                                              \
    }                                                                                    \
                                                                                         \
    first->prev = pos ? pos->prev : NULL;                                                \
    last->next = pos;                                                                    \
    if (first->prev) {                                                                   \
        first->prev->next = first;                                                       \
    } else if (pos == this->front) {                                                     \
        this->front = first;                                                             \
    }                                                                                    \
    if (pos) {                                                                           \
        pos->prev = last;                                                                \
    } else {                                                                             \
        if (this->back) {                                                                \
            this->back->next = first;                                                    \
            first->prev = this->back;                                                    \
        }                                                                                \
        this->back = last;                                                               \
    }                                                                                    \
    this->size = newSize;                                                                \
    return rv;                                                                           \
}                                                                                        \
                                                                                         \
List_##id *list_new_fromArray_##id(t const *arr, unsigned size) {                        \
    List_##id *l = calloc(1, sizeof(List_##id));                                         \
    customAssert(l)                                                                      \
    if (l && arr && size) list_insert_fromArray_##id(l, NULL, arr, size);                \
    return l;                                                                            \
}                                                                                        \
                                                                                         \
List_##id *list_new_repeatingValue_##id(unsigned n, t const value) {                     \
    List_##id *l = list_new(id);                                                         \
    if (l && n) list_insert_repeatingValue_##id(l, NULL, n, value);                      \
    return l;                                                                            \
}                                                                                        \
                                                                                         \
List_##id *list_createCopy_##id(List_##id const *other) {                                \
    List_##id *l = list_new(id);                                                         \
    if (l) list_insert_fromList_##id(l, NULL, other->front, NULL);                       \
    return l;                                                                            \
}                                                                                        \
                                                                                         \
ListEntry_##id *list_erase_##id(List_##id *this,                                         \
                                ListEntry_##id *first, ListEntry_##id *last) {           \
    ListEntry_##id *before, *next, *res;                                                 \
    if (!first || first == last) return NULL;                                            \
                                                                                         \
    before = first->prev;                                                                \
    for (; first != last; first = next, --this->size) {                                  \
        next = first->next;                                                              \
        deleteValue(first->data);                                                        \
        free(first);                                                                     \
    }                                                                                    \
                                                                                         \
    if (before) {                                                                        \
        before->next = last;                                                             \
    } else {                                                                             \
        this->front = last;                                                              \
    }                                                                                    \
                                                                                         \
    if (last) {                                                                          \
        res = last;                                                                      \
        last->prev = before;                                                             \
    } else {                                                                             \
        res = LIST_END;                                                                  \
        this->back = before;                                                             \
    }                                                                                    \
    return res;                                                                          \
}                                                                                        \
                                                                                         \
unsigned char list_resize_usingValue_##id(List_##id *this,                               \
                                          unsigned n, t value) {                         \
    ListEntry_##id *first = this->front;                                                 \
    if (n == this->size) return 1;                                                       \
    else if (n < this->size) {                                                           \
        listEntry_advance_##id(&first, n);                                               \
        list_erase_##id(this, first, NULL);                                              \
        return 1;                                                                        \
    }                                                                                    \
    return !list_insert_repeatingValue_##id(this, NULL, n - this->size, value);          \
}                                                                                        \
                                                                                         \
void list_reverse_##id(List_##id *this) {                                                \
    ListEntry_##id *newFront = this->back, *newBack = this->front, *next, *curr;         \
    for (curr = this->front; curr; curr = next) {                                        \
        next = curr->next;                                                               \
        curr->next = curr->prev;                                                         \
        curr->prev = next;                                                               \
    }                                                                                    \
    this->front = newFront;                                                              \
    this->back = newBack;                                                                \
}                                                                                        \
                                                                                         \
void list_remove_if_##id(List_##id *this, int (*cond)(t*)) {                             \
    ListEntry_##id *curr, *prev = NULL, *next;                                           \
    for (curr = this->front; curr; curr = next) {                                        \
        next = curr->next;                                                               \
        if (cond(&curr->data)) {                                                         \
            if (prev) {                                                                  \
                prev->next = next;                                                       \
            } else {                                                                     \
                this->front = next;                                                      \
            }                                                                            \
                                                                                         \
            if (next) {                                                                  \
                next->prev = prev;                                                       \
            } else {                                                                     \
                this->back = prev;                                                       \
            }                                                                            \
                                                                                         \
            deleteValue(curr->data);                                                     \
            free(curr);                                                                  \
            this->size--;                                                                \
        } else {                                                                         \
            prev = curr;                                                                 \
        }                                                                                \
    }                                                                                    \
    this->back = prev;                                                                   \
}                                                                                        \
                                                                                         \
void list_splice_range_##id(List_##id *this,                                             \
                            ListEntry_##id *position, List_##id *other,                  \
                            ListEntry_##id *first, ListEntry_##id *last) {               \
    ListEntry_##id *firstprev, *curr;                                                    \
    unsigned count = 0;                                                                  \
    if (!first || first == last) return;                                                 \
                                                                                         \
    /* get number of elements */                                                         \
    firstprev = first->prev;                                                             \
    for (curr = first; curr != last; ++count, curr = curr->next);                        \
                                                                                         \
    if (!this->front) {                                                                  \
        this->front = first;                                                             \
        this->back = last ? last->prev : other->back;                                    \
        this->front->prev = this->back->next = NULL;                                     \
    } else if (!position) { /* append */                                                 \
        this->back->next = first;                                                        \
        first->prev = this->back;                                                        \
        this->back = last ? last->prev : other->back;                                    \
        this->back->next = NULL;                                                         \
    } else {                                                                             \
        ListEntry_##id *prev = position->prev;                                           \
        first->prev = prev;                                                              \
        position->prev = last ? last->prev : other->back;                                \
        position->prev->next = position;                                                 \
        if (prev) {                                                                      \
            prev->next = first;                                                          \
        } else {                                                                         \
            this->front = first;                                                         \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    if (firstprev) {                                                                     \
        firstprev->next = last;                                                          \
    } else {                                                                             \
        other->front = last;                                                             \
    }                                                                                    \
                                                                                         \
    if (last) {                                                                          \
        last->prev = firstprev;                                                          \
    } else {                                                                             \
        other->back = firstprev;                                                         \
    }                                                                                    \
    this->size += count;                                                                 \
    other->size -= count;                                                                \
}                                                                                        \

/* --------------------------------------------------------------------------
 * LIST ALGORITHM/COMPARISONS SECTION
 * -------------------------------------------------------------------------- */


/**
 * Scans the list and removes any adjacent elements that compare equal to the 
 * preceding element.
 * 
 * Example:
 *     Input : [1, 2, 2, 2, 3, 3]
 *     Output: [1, 2, 3]
 */
#define list_unique(id, this) list_unique_##id(this)


/**
 * Removes any elements equal to @c value .
 *
 * @param  value  @c t : Value to remove.
 */
#define list_remove_value(id, this, value) list_remove_value_##id(this, value)


/**
 * Finds the first instance of @c value .
 *
 * @param   value  @c t : Value to search for.
 *
 * @return         @c ListEntry* : If @c value was found, returns an entry
 *                 corresponding to that element. If it was not found, returns
 *                 NULL.
 */
#define list_find(id, this, value) list_find_##id(this, value)


/**
 * Merges @c other into this list, both of which must be in sorted order prior 
 * to this operation. @c other is left with a size of 0, and this list grows 
 * by as many elements as @c other previously contained.
 *
 * @param  other  @c List* : Other list.
 */
#define list_merge(id, this, other) list_merge_##id(this, other)


/**
 * @brief Sorts the list according to the @c cmp_lt macro provided in 
 * @c gen_list_source . Time complexity: approx. O(n * log(n)).
 */
#define list_sort(id, this) list_sort_##id(this)


/**
 * Creates a new list representing the union of this list and @c other 
 * (i.e. elements that are in this list, @c other , or both - all elements).
 *
 * @param   other   @c List* : Second list.
 *
 * @return          @c List* : Newly allocated list.
 */
#define list_union(id, this, other) list_union_##id(this, other)


/**
 * Creates a new list representing the intersection of this list and 
 * @c other (i.e. all elements that both lists have in common).
 *
 * @param   other   @c List* : Second list.
 *
 * @return          @c List* : Newly allocated list.
 */
#define list_intersection(id, this, other) list_intersection_##id(this, other)


/**
 * Creates a new list representing the difference of this list and @c other 
 * (i.e. all elements that are unique to this list).
 *
 * @param   other   @c List* : Second list.
 *
 * @return          @c List* : Newly allocated list.
 */
#define list_difference(id, this, other) list_difference_##id(this, other)


/**
 * Creates a new list representing the symmetric difference of this list 
 * and @c other (i.e. all elements that neither list has in common).
 *
 * @param   other   @c List* : Second list.
 *
 * @return          @c List* : Newly allocated list.
 */
#define list_symmetric_difference(id, this, other)                                       \
        list_symmetric_difference_##id(this, other)


/**
 * Determines whether this list contains each element in @c other .
 *
 * @param   other   @c List* : Second list.
 *
 * @return          @c bool : Whether this list contains each element in
 *                  @c other .
 */
#define list_includes(id, this, other) list_includes_##id(this, other)


/**
 * Generates @c List function definitions for the specified type and ID, 
 * including set, sorting, and comparison functions.
 *
 * @param  id  ID to be used for the @c List and @c ListEntry types (must be
 *              unique).
 * @param  t   Type to be stored in the list.
 */
#define gen_list_headers_withAlg(id, t)                                                  \
                                                                                         \
gen_list_headers(id, t)                                                                  \
                                                                                         \
List_##id *list_union_##id(List_##id const *this, List_##id const *other)                \
  __attribute__((nonnull));                                                              \
List_##id *list_intersection_##id(List_##id const *this,                                 \
                                  List_##id const *other)                                \
  __attribute__((nonnull));                                                              \
List_##id *list_difference_##id(List_##id const *this,                                   \
                                List_##id const *other)                                  \
  __attribute__((nonnull));                                                              \
List_##id *list_symmetric_difference_##id(List_##id const *this,                         \
                                          List_##id const *other)                        \
  __attribute__((nonnull));                                                              \
unsigned char list_includes_##id(List_##id const *this,                                  \
                                 List_##id const *other)                                 \
  __attribute__((nonnull));                                                              \
void list_unique_##id(List_##id *this) __attribute__((nonnull));                         \
void list_remove_value_##id(List_##id *this, const t val)                                \
  __attribute__((nonnull));                                                              \
ListEntry_##id *list_find_##id(List_##id *this, const t val)                             \
  __attribute__((nonnull));                                                              \
void list_merge_##id(List_##id *this, List_##id *other)                                  \
  __attribute__((nonnull));                                                              \
void list_sort_##id(List_##id *this) __attribute__((nonnull));                           \


/**
 * Generates @c List function definitions for the specified type and ID, 
 * including set, sorting, and comparison functions.
 *
 * @param  id           ID used in @c gen_list_headers_withAlg .
 * @param  t            Type used in @c gen_list_headers_withAlg .
 * @param  cmp_lt       Macro of the form @c (x,y) that returns whether @c x is
 *                       strictly less than @c y .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the list.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form @c (x) which is a complement to
 *                       @c copyValue ; if memory was dynamically allocated in
 *                       @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_list_source_withAlg(id, t, cmp_lt, copyValue, deleteValue)                   \
                                                                                         \
gen_list_source(id, t, copyValue, deleteValue)                                           \
                                                                                         \
List_##id *list_union_##id(List_##id const *this, List_##id const *other) {              \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                        \
    List_##id *d_new = list_new(id);                                                     \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            list_insert_fromList_##id(d_new, NULL, first1, NULL);                        \
        } else if (first2) {                                                             \
            list_insert_fromList_##id(d_new, NULL, first2, NULL);                        \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            list_push_back(id, d_new, first1->data);                                     \
            first1 = first1->next;                                                       \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            list_push_back(id, d_new, first2->data);                                     \
            first2 = first2->next;                                                       \
        } else {                                                                         \
            list_push_back(id, d_new, first1->data);                                     \
            first1 = first1->next;                                                       \
            first2 = first2->next;                                                       \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        list_insert_fromList_##id(d_new, NULL, first1, NULL);                            \
    } else if (first2) {                                                                 \
        list_insert_fromList_##id(d_new, NULL, first2, NULL);                            \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
List_##id *list_intersection_##id(List_##id const *this,                                 \
                                  List_##id const *other) {                              \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                        \
    List_##id *d_new = list_new(id);                                                     \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) return d_new;                                          \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            first1 = first1->next;                                                       \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            first2 = first2->next;                                                       \
        } else {                                                                         \
            list_push_back(id, d_new, first1->data);                                     \
            first1 = first1->next;                                                       \
            first2 = first2->next;                                                       \
        }                                                                                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
List_##id *list_difference_##id(List_##id const *this,                                   \
                                List_##id const *other) {                                \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                        \
    List_##id *d_new = list_new(id);                                                     \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            list_insert_fromList_##id(d_new, NULL, first1, NULL);                        \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            list_push_back(id, d_new, first1->data);                                     \
            first1 = first1->next;                                                       \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            first2 = first2->next;                                                       \
        } else {                                                                         \
            first1 = first1->next;                                                       \
            first2 = first2->next;                                                       \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        list_insert_fromList_##id(d_new, NULL, first1, NULL);                            \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
List_##id *list_symmetric_difference_##id(List_##id const *this,                         \
                                          List_##id const *other) {                      \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                        \
    List_##id *d_new = list_new(id);                                                     \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            list_insert_fromList_##id(d_new, NULL, first1, NULL);                        \
        } else if (first2) {                                                             \
            list_insert_fromList_##id(d_new, NULL, first2, NULL);                        \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            list_push_back(id, d_new, first1->data);                                     \
            first1 = first1->next;                                                       \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            list_push_back(id, d_new, first2->data);                                     \
            first2 = first2->next;                                                       \
        } else {                                                                         \
            first1 = first1->next;                                                       \
            first2 = first2->next;                                                       \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        list_insert_fromList_##id(d_new, NULL, first1, NULL);                            \
    } else if (first2) {                                                                 \
        list_insert_fromList_##id(d_new, NULL, first2, NULL);                            \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
unsigned char list_includes_##id(List_##id const *this,                                  \
                                 List_##id const *other) {                               \
    ListEntry_##id *first1 = this->front, *first2 = other->front;                        \
    if (!(first1 && first2)) return first2 ? 0 : 1;                                      \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            first1 = first1->next;                                                       \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            return 0;                                                                    \
        } else {                                                                         \
            first1 = first1->next;                                                       \
            first2 = first2->next;                                                       \
        }                                                                                \
    }                                                                                    \
    return first2 == NULL;                                                               \
}                                                                                        \
                                                                                         \
void list_unique_##id(List_##id *this) {                                                 \
    ListEntry_##id *curr, *prev = this->front, *next;                                    \
    if (!prev) return;                                                                   \
                                                                                         \
    for (curr = prev->next; curr; curr = next) {                                         \
        next = curr->next;                                                               \
        if (ds_cmp_eq(cmp_lt, prev->data, curr->data)) {                                 \
            prev->next = next;                                                           \
            if (next) {                                                                  \
                next->prev = prev;                                                       \
            } else {                                                                     \
                this->back = prev;                                                       \
            }                                                                            \
                                                                                         \
            deleteValue(curr->data);                                                     \
            free(curr);                                                                  \
            this->size--;                                                                \
        } else {                                                                         \
            prev = curr;                                                                 \
        }                                                                                \
    }                                                                                    \
    this->back = prev;                                                                   \
}                                                                                        \
                                                                                         \
void list_remove_value_##id(List_##id *this, const t val) {                              \
    ListEntry_##id *curr, *prev = NULL, *next;                                           \
    for (curr = this->front; curr; curr = next) {                                        \
        next = curr->next;                                                               \
        if (ds_cmp_eq(cmp_lt, val, curr->data)) {                                        \
            if (prev) {                                                                  \
                prev->next = next;                                                       \
            } else {                                                                     \
                this->front = next;                                                      \
            }                                                                            \
                                                                                         \
            if (next) {                                                                  \
                next->prev = prev;                                                       \
            } else {                                                                     \
                this->back = prev;                                                       \
            }                                                                            \
                                                                                         \
            deleteValue(curr->data);                                                     \
            free(curr);                                                                  \
            this->size--;                                                                \
        } else {                                                                         \
            prev = curr;                                                                 \
        }                                                                                \
    }                                                                                    \
    this->back = prev;                                                                   \
}                                                                                        \
                                                                                         \
ListEntry_##id *list_find_##id(List_##id *this, const t val) {                           \
    ListEntry_##id *curr;                                                                \
    for (curr = this->front; curr; curr = curr->next) {                                  \
        if (ds_cmp_eq(cmp_lt, curr->data, val)) return curr;                             \
    }                                                                                    \
    return NULL;                                                                         \
}                                                                                        \
                                                                                         \
void list_merge_##id(List_##id *this, List_##id *other) {                                \
    ListEntry_##id *first1 = this->front, *first2 = other->front, *next, *prev;          \
    if (!first2) return;                                                                 \
    else if (!first1) {                                                                  \
        this->front = first2;                                                            \
        this->back = other->back;                                                        \
        this->size = other->size;                                                        \
        other->front = other->back = NULL;                                               \
        other->size = 0;                                                                 \
        return;                                                                          \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first2->data, first1->data)) {                                        \
            next = first2->next;                                                         \
            if ((prev = first1->prev)) {                                                 \
                prev->next = first2;                                                     \
            } else {                                                                     \
                this->front = first2;                                                    \
            }                                                                            \
            first2->prev = prev;                                                         \
            first2->next = first1;                                                       \
            first1->prev = first2;                                                       \
            first2 = next;                                                               \
        } else {                                                                         \
            first1 = first1->next;                                                       \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    if (first2) { /* still elements in first2, but have reached end of first1 */         \
        first2->prev = this->back;                                                       \
        this->back->next = first2;                                                       \
        this->back = other->back;                                                        \
    }                                                                                    \
    this->size += other->size;                                                           \
    other->front = other->back = NULL;                                                   \
    other->size = 0;                                                                     \
}                                                                                        \
                                                                                         \
void list_sort_##id(List_##id *this) {                                                   \
    List_##id tmp[64] = {0};                                                             \
    List_##id carry = {0};                                                               \
    List_##id *fill, *counter;                                                           \
    register ListEntry_##id *ltemp_front, *ltemp_back;                                   \
    register unsigned ltemp_size;                                                        \
    if (this->front == this->back) return;                                               \
    else if (this->size == 2 && cmp_lt(this->back->data, this->front->data)) {           \
        ltemp_back = this->back;                                                         \
        this->front = this->back;                                                        \
        this->back = ltemp_back;                                                         \
        this->front->prev = this->back->next = NULL;                                     \
        this->front->next = this->back;                                                  \
        this->back->prev = this->front;                                                  \
        return;                                                                          \
    }                                                                                    \
                                                                                         \
    fill = &tmp[0];                                                                      \
    do {                                                                                 \
        list_splice_element(id, &carry, carry.front, this, this->front);                 \
                                                                                         \
        for (counter = &tmp[0]; counter != fill && counter->front; ++counter) {          \
            list_merge_##id(counter, &carry);                                            \
                                                                                         \
            ltemp_front = carry.front;                                                   \
            ltemp_back = carry.back;                                                     \
            ltemp_size = carry.size;                                                     \
            carry.front = counter->front;                                                \
            carry.back = counter->back;                                                  \
            carry.size = counter->size;                                                  \
            counter->front = ltemp_front;                                                \
            counter->back = ltemp_back;                                                  \
            counter->size = ltemp_size;                                                  \
        }                                                                                \
                                                                                         \
        ltemp_front = carry.front;                                                       \
        ltemp_back = carry.back;                                                         \
        ltemp_size = carry.size;                                                         \
        carry.front = counter->front;                                                    \
        carry.back = counter->back;                                                      \
        carry.size = counter->size;                                                      \
        counter->front = ltemp_front;                                                    \
        counter->back = ltemp_back;                                                      \
        counter->size = ltemp_size;                                                      \
                                                                                         \
        if (counter == fill) ++fill;                                                     \
    } while (this->front);                                                               \
                                                                                         \
    for (counter = &tmp[1]; counter != fill; ++counter) {                                \
        list_merge_##id(counter, counter - 1);                                           \
    }                                                                                    \
                                                                                         \
    ltemp_front = this->front;                                                           \
    ltemp_back = this->back;                                                             \
    ltemp_size = this->size;                                                             \
    this->front = (fill - 1)->front;                                                     \
    this->back = (fill - 1)->back;                                                       \
    this->size = (fill - 1)->size;                                                       \
    (fill - 1)->front = ltemp_front;                                                     \
    (fill - 1)->back = ltemp_back;                                                       \
    (fill - 1)->size = ltemp_size;                                                       \
}                                                                                        \

#endif /* DS_LIST_H */
