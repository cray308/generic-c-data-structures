#ifndef DS_SET_H
#define DS_SET_H

#include "avltree.h"

#define __set_entry_get_key(e) (e)->data
#define __set_data_get_key(d) d
#define __set_copy_value(x, y)
#define __set_delete_value(x)

#define SET_END ((void *)-1)

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * Returns the next entry in terms of increasing values, if it exists.
 *
 * @param   e  @c SetEntry* : Entry to use.
 *
 * @return     @c SetEntry* : Next entry.
 */
#define setEntry_getNext(id, e) __avl_inorder_successor_##id(e)


/**
 * Returns the previous entry in terms of increasing values, if it exists.
 *
 * @param   e  @c SetEntry* : Entry to use.
 *
 * @return     @c SetEntry* : Previous entry.
 */
#define setEntry_getPrev(id, e) __avl_inorder_predecessor_##id(e)


/**
 * Advances the entry by @c n positions. A negative number means to move 
 * backwards.
 *
 * @param  e  @c SetEntry** : Address of entry.
 * @param  n  @c long : Number of positions to advance.
 */
#define setEntry_advance(id, e, n) __avlEntry_advance_##id(e, n)


/**
 * Returns the number of elements between @c first and @c last .
 *
 * @param   first  @c SetEntry* : Entry to start at.
 * @param   last   @c SetEntry* : Entry to end at. This must be reachable in
 *                  the forward direction by @c first .
 *
 * @return         @c long : Number of elements between @c first and @c last ,
 *                 or if @c last is not reachable, returns -1.
 */
#define setEntry_distance(id, first, last)                                               \
        __avlEntry_distance_##id(first, last)


/**
 * @brief @c SetEntry* : The starting point for iterating over elements in 
 * increasing order.
 */
#define set_iterator_begin(id, this) __avl_successor_##id((this)->root)


/**
 * @brief @c SetEntry* : The ending point for iterating over elements in 
 * increasing order.
 */
#define set_iterator_end() NULL


/**
 * @brief @c SetEntry* : The starting point for iterating over elements in 
 * decreasing order.
 */
#define set_iterator_rbegin(id, this) __avl_predecessor_##id((this)->root)


/**
 * @brief @c SetEntry* : The ending point for iterating over elements in 
 * decreasing order.
 */
#define set_iterator_rend() NULL


/**
 * Iterates through the set in-order.
 *
 * @param  it  @c SetEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define set_iter(id, this, it)                                                           \
        for (it = set_iterator_begin(id, this); it != set_iterator_end();                \
             it = setEntry_getNext(id, it))


/**
 * Iterates through the set in reverse order.
 *
 * @param  it  @c SetEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define set_riter(id, this, it)                                                          \
        for (it = set_iterator_rbegin(id, this); it;                                     \
             it = setEntry_getPrev(id, it))

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * @brief @c unsigned : The number of elements in the set.
 */
#define set_size(this) (this)->size


/**
 * @brief @c bool : Whether the set is empty.
 */
#define set_empty(this) !(this)->root

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty set.
 *
 * @return  @c Set* : Newly created set.
 */
#define set_new(id) __avltree_new_fromArray_##id(NULL, 0)


/**
 * Creates a new set using @c n elements from a built-in array @c arr .
 *
 * @param   arr  @c t* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c Set* : Newly created set.
 */
#define set_new_fromArray(id, arr, n) __avltree_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of @c other .
 *
 * @param   other  @c Set* : Set to copy.
 *
 * @return         @c Set* : Newly created set.
 */
#define set_createCopy(id, other) __avltree_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 */
#define set_free(id, this) do { set_clear(id, this); free(this); } while(0)


/**
 * Removes all elements from the set, leaving it with a size of 0.
 */
#define set_clear(id, this)                                                              \
        __avltree_erase_##id(this, __avl_successor_##id((this)->root), NULL)


/**
 * Tests whether @c value is in the set.
 *
 * @param   value  @c t : Value to be checked for membership.
 *
 * @return         @c bool : Whether the value was found.
 */
#define set_contains(id, this, value)                                                    \
        (__avltree_find_key_##id(this, value, 0) != NULL)


/**
 * Returns the entry with a value of @c value .
 *
 * @param   value  @c t : Value to be found.
 *
 * @return         @c SetEntry* : Entry that was found, or NULL if it was not
 *                 found.
 */
#define set_find(id, this, value) __avltree_find_key_##id(this, value, 0)


/**
 * Inserts @c value into the set.
 *
 * @param   value  @c t : Value to insert.
 *
 * @return         @c SetEntry* : Entry corresponding to the inserted value.
 */
#define set_insert(id, this, value) __avltree_insert_##id(this, value, NULL)


/**
 * Inserts @c value into the set, and updates @c inserted with the result of 
 * insertion.
 *
 * @param   value     @c t : Value to insert.
 * @param   inserted  @c int* : Set to 1 if the value was newly inserted, or 0
 *                     if the value was already a member.
 *
 * @return            @c SetEntry* : Entry corresponding to the inserted value.
 */
#define set_insert_withResult(id, this, value, inserted)                                 \
        __avltree_insert_##id(this, value, inserted)


/**
 * Inserts @c n elements from a built-in array @c arr .
 *
 * @param   arr  @c t* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c bool : Whether the operation succeeded.
 */
#define set_insert_fromArray(id, this, arr, n)                                           \
        __avltree_insert_fromArray_##id(this, arr, n)


/**
 * Inserts elements from another set in the range [ @c start , @c end ).
 *
 * @param   start  @c SetEntry* : First entry to insert. Must not be NULL.
 * @param   end    @c SetEntry* : Entry after the last entry to insert. If this
 *                  is NULL, all elements from @c start through the end
 *                  (greatest element) of the other set will be inserted.
 *
 * @return         @c bool : Whether the operation succeeded.
 */
#define set_insert_fromSet(id, this, start, end)                                         \
        __avltree_insert_fromTree_##id(this, start, end)


/**
 * Erases elements in the range [ @c begin , @c end ).
 *
 * @param  begin  @c SetEntry* : First entry to erase.
 * @param  end    @c SetEntry* : Entry after the last entry to be deleted. If
 *                 this is NULL, all elements from @c begin through the
 *                 greatest element in the set will be removed.
 *
 * @return        @c SetEntry* : If successful, returns an entry
 *                corresponding to the element after the last deleted element;
 *                if the last deleted element was the last element in the
 *                set, this is @c SET_END . If an error occurred, returns NULL.
 */
#define set_erase(id, this, begin, end) __avltree_erase_##id(this, begin, end)


/**
 * Removes a single item whose value is equal to @c value , if it exists.
 *
 * @param  value  @c t : Value to be deleted.
 *
 * @return        @c SetEntry* : If successful, returns the entry after the
 *                entry corresponding to @c value ; if @c value was the last
 *                element in the set, this is @c SET_END . If an error
 *                occurred, returns NULL.
 */
#define set_remove_value(id, this, value)                                                \
        __avltree_remove_entry_##id(this,                                                \
            __avltree_find_key_##id(this, value, 0))


/**
 * Removes @c entry from the set.
 *
 * @param  entry  @c SetEntry* : Entry to remove.
 *
 * @return        @c SetEntry* : If successful, returns the entry after
 *                @c entry ; if @c entry was the last element in the set, this
 *                is @c SET_END . If an error occurred, returns NULL.
 */
#define set_remove_entry(id, this, entry)                                                \
        __avltree_remove_entry_##id(this, entry)


/**
 * Returns a set with the union of this set and @c other (i.e. elements 
 * that are in this set, @c other , or both - all elements).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c Set* : Newly created set.
 */
#define set_union(id, this, other) set_union_##id(this, other)


/**
 * Returns a set with the intersection of this set and @c other (i.e. all 
 * elements that both sets have in common).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c Set* : Newly created set.
 */
#define set_intersection(id, this, other) set_intersection_##id(this, other)


/**
 * Returns a set with the difference of this set and @c other (i.e. all 
 * elements that are unique to this set).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c Set* : Newly created set.
 */
#define set_difference(id, this, other) set_difference_##id(this, other)


/**
 * Returns a set with the symmetric difference of this set and @c other 
 * (i.e. all elements that neither set has in common).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c Set* : Newly created set.
 */
#define set_symmetric_difference(id, this, other)                                        \
        set_symmetric_difference_##id(this, other)


/**
 * Tests whether this set is a subset of @c other (i.e. whether each element 
 * in this subset is in @c other ).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c bool : Whether each element in this set is in @c other .
 */
#define set_issubset(id, this, other) set_includes_##id(other, this)


/**
 * Tests whether this set is a superset of @c other (i.e. whether this set 
 * contains each element from @c other - the inverse of a subset).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c bool : Whether this set contains each element in
 *                 @c other .
 */
#define set_issuperset(id, this, other) set_issubset(id, other, this)


/**
 * Tests whether this set is disjoint with @c other (i.e. if the sets have no 
 * elements in common).
 *
 * @param   other  @c Set* : Other set.
 *
 * @return         @c bool : Whether this set and @c other have no common
 *                 elements.
 */
#define set_isdisjoint(id, this, other) set_disjoint_##id(this, other)


/**
 * Generates @c Set function declarations for the given value type.
 *
 * @param  id           ID to be used for the @c Set and @c SetEntry types (must be
 *                       unique).
 * @param  t            Type to be stored in the set.
 */
#define gen_set_headers(id, t)                                                           \
                                                                                         \
__setup_avltree_headers(id, t, Set_##id, t, SetEntry_##id)                               \
                                                                                         \
Set_##id *set_union_##id(Set_##id const *this, Set_##id const *other)                    \
  __attribute__((nonnull));                                                              \
Set_##id *set_intersection_##id(Set_##id const *this, Set_##id const *other)             \
  __attribute__((nonnull));                                                              \
Set_##id *set_difference_##id(Set_##id const *this, Set_##id const *other)               \
  __attribute__((nonnull));                                                              \
Set_##id *set_symmetric_difference_##id(Set_##id const *this,                            \
                                        Set_##id const *other)                           \
  __attribute__((nonnull));                                                              \
unsigned char set_includes_##id(Set_##id const *this, Set_##id const *other)             \
  __attribute__((nonnull));                                                              \
unsigned char set_disjoint_##id(Set_##id const *this, Set_##id const *other)             \
  __attribute__((nonnull));                                                              \


/**
 * Generates @c Set function definitions for the given value type.
 *
 * @param  id           ID used in @c gen_set_headers .
 * @param  t            Type used in @c gen_set_headers .
 * @param  cmp_lt       Macro of the form @c (x,y) that returns whether @c x is
 *                       strictly less than @c y .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the set.
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
#define gen_set_source(id, t, cmp_lt, copyValue, deleteValue)                            \
                                                                                         \
__setup_avltree_source(id, t, Set_##id, t, SetEntry_##id, cmp_lt,                        \
    __set_entry_get_key, __set_data_get_key, copyValue, deleteValue,                     \
    __set_copy_value, __set_delete_value)                                                \
                                                                                         \
Set_##id *set_union_##id(Set_##id const *this, Set_##id const *other) {                  \
    SetEntry_##id *first1, *first2;                                                      \
    Set_##id *d_new = set_new(id);                                                       \
    first1 = __avl_successor_##id(this->root);                                           \
    first2 = __avl_successor_##id(other->root);                                          \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            set_insert_fromSet(id, d_new, first1, NULL);                                 \
        } else if (first2) {                                                             \
            set_insert_fromSet(id, d_new, first2, NULL);                                 \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            set_insert(id, d_new, first1->data);                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            set_insert(id, d_new, first2->data);                                         \
            first2 = __avl_inorder_successor_##id(first2);                               \
        } else {                                                                         \
            set_insert(id, d_new, first1->data);                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
            first2 = __avl_inorder_successor_##id(first2);                               \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        set_insert_fromSet(id, d_new, first1, NULL);                                     \
    } else if (first2) {                                                                 \
        set_insert_fromSet(id, d_new, first2, NULL);                                     \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Set_##id *set_intersection_##id(Set_##id const *this, Set_##id const *other) {           \
    SetEntry_##id *first1, *first2;                                                      \
    Set_##id *d_new = set_new(id);                                                       \
    first1 = __avl_successor_##id(this->root);                                           \
    first2 = __avl_successor_##id(other->root);                                          \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) return d_new;                                          \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            first1 = __avl_inorder_successor_##id(first1);                               \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            first2 = __avl_inorder_successor_##id(first2);                               \
        } else {                                                                         \
            set_insert(id, d_new, first1->data);                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
            first2 = __avl_inorder_successor_##id(first2);                               \
        }                                                                                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Set_##id *set_difference_##id(Set_##id const *this, Set_##id const *other) {             \
    SetEntry_##id *first1, *first2;                                                      \
    Set_##id *d_new = set_new(id);                                                       \
    first1 = __avl_successor_##id(this->root);                                           \
    first2 = __avl_successor_##id(other->root);                                          \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            set_insert_fromSet(id, d_new, first1, NULL);                                 \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            set_insert(id, d_new, first1->data);                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            first2 = __avl_inorder_successor_##id(first2);                               \
        } else {                                                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
            first2 = __avl_inorder_successor_##id(first2);                               \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        set_insert_fromSet(id, d_new, first1, NULL);                                     \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Set_##id *set_symmetric_difference_##id(Set_##id const *this,                            \
                                        Set_##id const *other) {                         \
    SetEntry_##id *first1, *first2;                                                      \
    Set_##id *d_new = set_new(id);                                                       \
    first1 = __avl_successor_##id(this->root);                                           \
    first2 = __avl_successor_##id(other->root);                                          \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            set_insert_fromSet(id, d_new, first1, NULL);                                 \
        } else if (first2) {                                                             \
            set_insert_fromSet(id, d_new, first2, NULL);                                 \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            set_insert(id, d_new, first1->data);                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            set_insert(id, d_new, first2->data);                                         \
            first2 = __avl_inorder_successor_##id(first2);                               \
        } else {                                                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
            first2 = __avl_inorder_successor_##id(first2);                               \
        }                                                                                \
    }                                                                                    \
    if (first1) {                                                                        \
        set_insert_fromSet(id, d_new, first1, NULL);                                     \
    } else if (first2) {                                                                 \
        set_insert_fromSet(id, d_new, first2, NULL);                                     \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
unsigned char set_includes_##id(Set_##id const *this, Set_##id const *other) {           \
    SetEntry_##id *first1, *first2;                                                      \
    first1 = __avl_successor_##id(this->root);                                           \
    first2 = __avl_successor_##id(other->root);                                          \
    if (!(first1 && first2)) return first2 ? 0 : 1;                                      \
                                                                                         \
    while (first1 && first2) {                                                           \
        if (cmp_lt(first1->data, first2->data)) {                                        \
            first1 = __avl_inorder_successor_##id(first1);                               \
        } else if (cmp_lt(first2->data, first1->data)) {                                 \
            return 0;                                                                    \
        } else {                                                                         \
            first1 = __avl_inorder_successor_##id(first1);                               \
            first2 = __avl_inorder_successor_##id(first2);                               \
        }                                                                                \
    }                                                                                    \
    return first2 == NULL;                                                               \
}                                                                                        \
                                                                                         \
unsigned char set_disjoint_##id(Set_##id const *this, Set_##id const *other) {           \
    SetEntry_##id *n1, *n2;                                                              \
    n1 = __avl_successor_##id(this->root);                                               \
    n2 = __avl_successor_##id(other->root);                                              \
    while (n1 && n2) {                                                                   \
        if (cmp_lt(n1->data, n2->data)) {                                                \
            n1 = __avl_inorder_successor_##id(n1);                                       \
        } else if (cmp_lt(n2->data, n1->data)) {                                         \
            n2 = __avl_inorder_successor_##id(n2);                                       \
        } else {                                                                         \
            return 0;                                                                    \
        }                                                                                \
    }                                                                                    \
    return 1;                                                                            \
}                                                                                        \

#endif /* DS_SET_H */
