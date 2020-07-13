#ifndef LIST_H
#define LIST_H

#include "ds.h"
#include <stdbool.h>

#define LIST_ERROR (DLLNode *)(-1)
#define LIST_END (DLLNode *)(0)

typedef int (*meetsCondition)(const void *_val);

typedef enum {
    LIST_INIT_EMPTY,
    LIST_INIT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    LIST_INIT_LIST /* like List *nums */
} ListInitializer;

typedef enum {
    LIST_INSERT_SINGLE, /* void */
    LIST_INSERT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    LIST_INSERT_LIST /* like List *nums */
} ListInsertType;

typedef enum {
    LIST_SPLICE_ALL, /* move entire list */
    LIST_SPLICE_SINGLE, /* move single element */
    LIST_SPLICE_RANGE /* move range of elements in [first, last) */
} ListSpliceType;

typedef struct DLLNode DLLNode;
struct DLLNode {
    DLLNode *prev;
    DLLNode *next;
    char data[];
};

typedef struct {
    size_t size;
    DSHelper helper;
    DLLNode *curr;
    DLLNode *front;
    DLLNode *back;
} List;

typedef DLLNode ListEntry;

void _list_removal_ops(List *l, void *val, meetsCondition cond);
void _list_push_val(List *l, const void *value, bool front);
void _list_pop_val(List *l, bool front);


/**
 * Pointer to the front element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_front(l) ((l)->front ? (void *)((l)->front->data) : NULL)


/**
 * Pointer to the back element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_back(l) ((l)->back ? (void *)((l)->back->data) : NULL)


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
 * @param   l     The List struct pointer.
 * @param   eptr  Pointer which is assigned to the current element's data.
 */
#define list_iter(l,eptr) \
    for ((l)->curr = (l)->front, (eptr) = (void*)((l)->curr ? (l)->curr->data : NULL); \
         (l)->curr != NULL; \
         (l)->curr = (l)->curr->next, (eptr) = (void*)((l)->curr ? (l)->curr->data : NULL))


/**
 * Macro for iterating over the list in reverse (from back to front).
 *
 * @param   l     The List struct pointer.
 * @param   eptr  Pointer which is assigned to the current element's data.
 */
#define list_riter(l,eptr) \
    for ((l)->curr = (l)->back, (eptr) = (void*)((l)->curr ? (l)->curr->data : NULL); \
         (l)->curr != NULL; \
         (l)->curr = (l)->curr->prev, (eptr) = (void*)((l)->curr ? (l)->curr->data : NULL))


/**
 * Creates a new list.
 * In (1), an empty List is created.
 * In (2), a List is initialized from a built-in array data type, passed as a pointer to the
 *   function, composed of at most n elements.
 * In (3), a List is initialized from another List.
 * 
 * (1) init = LIST_INIT_EMPTY:    list_new(const DSHelper *helper, ListInitializer init)
 * (2) init = LIST_INIT_BUILTIN:  list_new(const DSHelper *helper, ListInitializer init, void *arr, int n)
 * (3) init = LIST_INIT_LIST:     list_new(const DSHelper *helper, ListInitializer init, List *other)
 *
 * @param   helper  Pointer to DSHelper struct.
 * @param   init    Type of initializer to execute.
 *
 * @return          A pointer to the newly allocated list.
 */
List *list_new(const DSHelper *helper, ListInitializer init, ...);


/**
 * Deletes all elements and frees the list.
 *
 * @param  l  Pointer to list.
 */
void list_free(List *l);


/**
 * Prepends the provided element to the start of the list.
 *
 * @param  l      Pointer to list.
 * @param  value  Pointer to the element to be inserted.
 */
#define list_push_front(l, value) _list_push_val((l), (value), true)


/**
 * Appends the provided element to the end of the list.
 *
 * @param  l      Pointer to list.
 * @param  value  Pointer to the element to be inserted.
 */
#define list_push_back(l, value) _list_push_val((l), (value), false)


/**
 * Removes the first element from the list.
 *
 * @param  l  Pointer to list.
 */
#define list_pop_front(l) _list_pop_val((l), true)


/**
 * Removes the last element from the list.
 *
 * @param  l  Pointer to list.
 */
#define list_pop_back(l) _list_pop_val((l), false)


/**
 * Inserts new elements BEFORE the ListEntry at "pos".
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, starting at index "start" and
 *   using "n" elements. "start" MUST be a zero-based index and "n" MUST be positive.
 * In (3), elements from another List are inserted in the range [start, end). If "start" is NULL,
 *   it defaults to the front of the other list. If "end" is LIST_END, then all elements from
 *   "start" through the end of the other list will be inserted.
 * 
 * (1) type = LIST_INSERT_SINGLE:   list_insert(List *l, DLLNode *pos, int sorted, ListInsertType type, void *value)
 * (2) type = LIST_INSERT_BUILTIN:  list_insert(List *l, DLLNode *pos, int sorted, ListInsertType type, void *arr, int start, int n)
 * (3) type = LIST_INSERT_LIST:     list_insert(List *l, DLLNode *pos, int sorted, ListInsertType type, List *other, DLLNode *start, DLLNode *end)
 *
 * @param   l       Pointer to list.
 * @param   pos     ListEntry before which the element(s) should be inserted. If this is LIST_END,
 *                    it defaults to list_push_back.
 * @param   sorted  Whether elements should be inserted in sorted order. If this is true,
 *                    "pos" is ignored.
 * @param   type    Type of insertion to execute.
 *
 * @return         If successful, and "sorted" is false, returns a ListEntry corresponding to the
 *                   first inserted element. If "sorted" is true and multiple elements were
 *                   inserted, returns the front ListEntry in the list. If an error occurred,
 *                   returns LIST_ERROR.
 */
DLLNode *list_insert(List *l, DLLNode *pos, bool sorted, ListInsertType type, ...);


/**
 * Erases elements from the list within the range [first, last). To erase a single element, an
 *   example function call would be list_erase(l, e, e->next).
 *
 * @param   l      Pointer to list.
 * @param   first  The first ListEntry to be removed - must be provided.
 * @param   last   ListEntry AFTER the last element to be deleted. If this is LIST_END, then
 *                   all elements from start through the end of the list will be removed.
 *
 * @return        If successful, returns a ListEntry corresponding to the element after the
 *                   last deleted element. If an error occurred, returns LIST_ERROR. If the last
 *                   deleted element was at the end of the list, returns LIST_END.
 */
DLLNode *list_erase(List *l, DLLNode *first, DLLNode *last);


/**
 * Removes all elements from the list.
 *
 * @param  l  Pointer to list.
 */
#define list_clear(l) list_erase((l), ((l)->front), (LIST_END))


/**
 * Reverses the list; thus what was the last element will now be the first.
 *
 * @param  l  Pointer to list.
 */
void list_reverse(List *l);


/**
 * Sorts the list according to the comparison function in the provided DSHelper struct.
 * Internally, it uses an iterative version of merge sort (rather than recursive, to avoid function
 * call overhead).
 * 
 * Time complexity: approx. O(n * log(n))
 *
 * @param  l  Pointer to list.
 */
void list_sort(List *l);


/**
 * Scans the list and removes any adjacent elements that compare equal to the preceding element.
 * 
 * Example:
 *     Input : [1, 2, 2, 2, 3, 3]
 *     Output: [1, 2, 3]
 *
 * @param  l  Pointer to list.
 */
#define list_unique(l) _list_removal_ops((l), NULL, NULL)


/**
 * Removes any elements that compare equal to the provided value.
 *
 * @param  l    Pointer to list.
 * @param  val  Pointer to the value to compare to a list element's data.
 */
#define list_remove_value(l, val) _list_removal_ops((l), (val), NULL)

/**
 * Removes any elements satisfying the provided condition.
 *
 * @param  l          Pointer to list.
 * @param  condition  Function pointer to check if an element's data meets the condition.
 */
#define list_remove_if(l, condition) _list_removal_ops((l), NULL, (condition))


/**
 * Finds the first instance of the provided value.
 *
 * @param   l    Pointer to list.
 * @param   val  Pointer to the value to compare to a list element's data.
 *
 * @return       If the value was found, returns a ListEntry pointing to that element. If it was
 *                  not found, returns NULL.
 */
DLLNode *list_find(List *l, void *val);


/**
 * Creates a sublist from "this" in the range [first,last) (non-inclusive for "last").
 *
 * @param   this   Pointer to list.
 * @param   first  Element to start the sublist.
 * @param   last   Element at which the sublist will stop. If this is LIST_END, the sublist will
 *                   include all elements from "first" to the end of the list.
 *
 * @return         Newly created sublist from the list. If "this" is empty or "first" is NULL,
 *                   returns NULL.
 */
List *list_sublist(List *this, DLLNode *first, DLLNode *last);


/**
 * Merges "other" into "this", both of which must be in sorted order prior to this operation.
 *  "other" is left with a size of 0, and "this" grows by as many elements as "other" previously
 *  contained.
 *
 * @param   this   Pointer to list.
 * @param   other  Pointer to other list, which will be merged with "this".
 */
void list_merge(List *this, List *other);


/**
 * Moves elements from "other" before the ListEntry in "this" at "pos". No new elements are created;
 *  they are simply transferred from "other" into "this".
 * 
 * (1) type = LIST_SPLICE_ALL:     list_splice(List *this, DLLNode *position, List *other, LIST_SPLICE_ALL)
 * (2) type = LIST_SPLICE_SINGLE:  list_splice(List *this, DLLNode *position, List *other, LIST_SPLICE_SINGLE, DLLNode *elem)
 * (3) type = LIST_SPLICE_RANGE:   list_splice(List *this, DLLNode *position, List *other, LIST_SPLICE_RANGE, DLLNode *first, DLLNode *last)
 * 
 * @param   this      Pointer to list into which elements will be moved.
 * @param   position  Pointer to element in "this" before which elements in "other" will be
 *                      moved. If this is LIST_END, elements from "other" will be appended to
 *                      "this".
 * @param   other     Pointer to other list from which elements will be moved.
 * @param   type      Type of splice to perform.
 */
void list_splice(List *this, DLLNode *position, List *other, ListSpliceType type, ...);

#endif
