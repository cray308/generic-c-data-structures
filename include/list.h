#ifndef LIST_H
#define LIST_H

#include "ds.h"
#include <stdbool.h>

#define LIST_ERROR (DLLNode *)(-1)
#define LIST_END (DLLNode *)(-2)

typedef int (*meetsCondition)(const void *_val);

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


typedef DLLNode *ListEntry;

/**
 * Pointer to the front element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_front(l) (void*)(((l)->front) ? ((l)->front->data) : NULL)


/**
 * Pointer to the back element's data, or NULL if the list is empty.
 *
 * @param   l  The List struct pointer.
 */
#define list_back(l) (void*)(((l)->back) ? ((l)->back->data) : NULL)


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
#define list_size(l) ((l)->size)


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
 * Creates a new empty list.
 *
 * @param   helper  Pointer to DSHelper struct.
 *
 * @return          A pointer to the newly allocated list.
 */
List *list_new(const DSHelper *helper);


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
void list_push_front(List *l, const void *value);


/**
 * Appends the provided element to the end of the list.
 *
 * @param  l      Pointer to list.
 * @param  value  Pointer to the element to be inserted.
 */
void list_push_back(List *l, const void *value);


/**
 * Removes the first element from the list.
 *
 * @param  l  Pointer to list.
 */
void list_pop_front(List *l);


/**
 * Removes the last element from the list.
 *
 * @param  l  Pointer to list.
 */
void list_pop_back(List *l);


/**
 * Inserts the provided element before the ListEntry at pos.
 *
 * @param   l      Pointer to list.
 * @param   pos    ListEntry where the element should be inserted before. If this is NULL, it
 *                   defaults to list_push_back.
 * @param   value  Pointer to the element to be inserted.
 *
 * @return         If successful, returns a ListEntry corresponding to the inserted element. If an
 *                    error occurred, returns LIST_ERROR.
 */
DLLNode *list_insert_before(List *l, DLLNode *pos, void *value);


/**
 * Inserts the provided element after the ListEntry at pos.
 *
 * @param   l      Pointer to list.
 * @param   pos    ListEntry where the element should be inserted after. If this is NULL, it
 *                   defaults to list_push_back.
 * @param   value  Pointer to the element to be inserted.
 *
 * @return         If successful, returns a ListEntry corresponding to the inserted element. If an
 *                    error occurred, returns LIST_ERROR.
 */
DLLNode *list_insert_after(List *l, DLLNode *pos, void *value);


/**
 * Inserts the provided element into the list in sorted order, according to the comparison
 *   function in the provided DSHelper struct.
 *
 * @param   l      Pointer to list.
 * @param   value  Pointer to the element to be inserted.
 *
 * @return         If successful, returns a ListEntry corresponding to the inserted element.
 */
DLLNode *list_insert_sorted(List *l, void *value);


/**
 * Inserts elements from another list in the range [start, end) into the first list BEFORE pos.
 *   The inserted elements from are removed from the other list.
 *
 * @param   l      Pointer to list.
 * @param   pos    ListEntry in "l" before which elements should be inserted. If this is NULL,
 *                   elements will be appended.
 * @param   other  The other list from which elements will be inserted.
 * @param   start  ListEntry of the first element to insert from the other list. If this is NULL,
 *                   it defaults to the front of the other list.
 * @param   end    ListEntry AFTER the last element to be inserted. If this is NULL, then
 *                   all elements from "start" through the end of the other list will be inserted.
 *
 * @return         If successful, returns a ListEntry corresponding to the first element which
 *                    was inserted. If an error occurred, returns LIST_ERROR.
 */
DLLNode *list_insert_list(List *l, DLLNode *pos, List *other, DLLNode *start, DLLNode *end);


/**
 * Removes the provided ListEntry from the list.
 *
 * @param   l     Pointer to list.
 * @param   elem  ListEntry to be removed.
 *
 * @return        If successful, returns a ListEntry corresponding to the element after the
 *                   deleted element. If an error occurred, returns LIST_ERROR. If the deleted
 *                   element was the end of the list, returns LIST_END.
 */
//DLLNode *list_remove(List *l, DLLNode *elem);


/**
 * Erases elements from the list within the range [first, last). To erase a single element, an
 *   example function call would be list_erase(l, e, e->next).
 *
 * @param   l      Pointer to list.
 * @param   first  The first ListEntry to be removed - must be provided.
 * @param   last   ListEntry AFTER the last element to be deleted. If this is NULL, then
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
void list_clear(List *l);


/**
 * Reverses the list; thus what was the last element will now be the first.
 *
 * @param  l  Pointer to list.
 */
void list_reverse(List *l);


/**
 * Sorts the list according to the comparison function in the provided DSHelper struct.
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
void list_unique(List *l);


/**
 * Removes any elements that compare equal to the provided value.
 *
 * @param  l    Pointer to list.
 * @param  val  Pointer to the value to compare to a list element's data.
 */
void list_remove_value(List *l, void *val);


/**
 * Removes any elements satisfying the provided condition.
 *
 * @param  l          Pointer to list.
 * @param  condition  Function pointer to check if an element's data meets the condition.
 */
void list_remove_if(List *l, meetsCondition condition);


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

#endif
