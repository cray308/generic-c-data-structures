#ifndef SET_H
#define SET_H

#include "ds.h"
#include "rbtree.h"
#include <stdbool.h>

typedef Tree Set;
typedef RBNode SetEntry;
#define SET_END (SetEntry *)(0)

typedef enum {
    SET_INIT_EMPTY,
    SET_INIT_BUILTIN, /* int nums[] = {1, 2, 3}; set_new(..., &nums, 3); */
    SET_INIT_SET /* Set *nums = ...; set_new(..., nums) */
} SetInitializer;

typedef enum {
    SET_INSERT_SINGLE, /* int num = 5; set_insert(..., &num); */
    SET_INSERT_BUILTIN, /* int nums[] = {1, 2, 3}; set_insert(..., &nums, 0, 3); */
    SET_INSERT_SET /* Set *nums = ...; set_insert(..., nums, begin, end) */
} SetInsertType;

bool _set_bool_ops(Set *this, Set *other, bool subset);
Set *_set_creation_ops(Set *this, Set *other, unsigned char op);

/**
 * The number of elements in the set.
 * 
 * @param   s  Pointer to set.
 */
#define set_len(s) ((int) (s)->size)


/**
 * Tests whether the set is empty.
 * 
 * @param   s  Pointer to set.
 */
#define set_empty(s) (!((s)->root))


/**
 * Iterates through the set in-order according to DSHelper.cmp.
 * 
 * @param   s     Pointer to set.
 * @param   eptr  Pointer which is assigned to the current set element's data.
 */
#define set_iter(s, eptr) tree_inorder((s), (eptr))


/**
 * Creates a new set.
 * In (1), an empty Set is created.
 * In (2), a Set is initialized from a built-in array data type, passed as a pointer to the
 *   function, composed of at most n elements.
 * In (3), a Set is initialized from another existing Set.
 * 
 * (1) init = SET_INIT_EMPTY:    set_new(const DSHelper *helper, SetInitializer init)
 * (2) init = SET_INIT_BUILTIN:  set_new(const DSHelper *helper, SetInitializer init, void *arr, int n)
 * (3) init = SET_INIT_SET:      set_new(const DSHelper *helper, SetInitializer init, Set *other)
 *
 * @param   helper  Pointer to DSHelper struct. Since these sets are internally organized as trees,
 *                    a comparison function MUST be provided.
 * @param   init    Type of initializer to execute.
 *
 * @return          Pointer to the newly created set. If an error occurred, returns NULL.
 */
Set *set_new(const DSHelper *helper, SetInitializer init, ...);


/**
 * Deletes all elements and frees the set.
 *
 * @param  this  Pointer to set.
 */
#define set_free(this) tree_free((this))


/**
 * Removes all elements from the set, leaving it with a size of 0.
 *
 * @param  this  Pointer to set.
 */
#define set_clear(this) tree_clear((this))


/**
 * Tests whether "value" is in the set.
 *
 * @param   this   Pointer to set.
 * @param   value  Pointer to the value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define set_in(this, value) (tree_find((this), (value)) != NULL)


/**
 * Returns the SetEntry with a key matching "value".
 *
 * @param   this   Pointer to set.
 * @param   value  Pointer to the value to be found.
 *
 * @return         Pointer to SetEntry whose data matches "value", or NULL if it was not found.
 */
#define set_find(this, value) tree_find((this), (value))


/**
 * Inserts elements into the set.
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, starting at index "start" and
 *   using "n" elements. "start" MUST be a zero-based index and "n" MUST be positive.
 * In (3), elements from another Set are inserted in the range [start, end). If "start" is NULL,
 *   it defaults to the first (smallest) element in the other set. If "end" is SET_END, then all
 *   elements from "start" through the end (greatest element) of the other set will be inserted.
 * 
 * (1) type = SET_INSERT_SINGLE:   set_insert(Set *this, SetInsertType type, void *value)
 * (2) type = SET_INSERT_BUILTIN:  set_insert(Set *this, SetInsertType type, void *arr, int start, int n)
 * (3) type = SET_INSERT_SET:      set_insert(Set *this, SetInsertType type, Set *other, SetEntry *start, SetEntry *end)
 *
 * @param  a      Pointer to set.
 * @param   type  Type of insertion to execute.
 */
void set_insert(Set *this, SetInsertType type, ...);


/**
 * Erases elements in the range [begin,end).
 *
 * @param  this   Pointer to set.
 * @param  begin  First element to erase. If this is NULL, it defaults to the smallest element
 *                  in the set.
 * @param  end    SetEntry AFTER the last element to be deleted. If this is SET_END, then all
 *                  elements from start through the greatest element in the set will be removed.
 */
void set_erase(Set *this, SetEntry *begin, SetEntry *end);


/**
 * Removes a single entry from the set whose value is equal to "value".
 *
 * @param  this   Pointer to set.
 * @param  value  Pointer to the value to be deleted.
 */
#define set_remove_value(this, value) tree_delete_by_val((this), (value))


/**
 * Returns a set with the union of "this" and "other" (i.e. elements that are in "this", "other",
 * or both - all elements).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "this" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_union(this, other) _set_creation_ops((this), (other), 0x1F)


/**
 * Returns a set with the intersection of "this" and "other" (i.e. all elements that both sets
 * have in common).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "this" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_intersection(this, other) _set_creation_ops((this), (other), 0x10)


/**
 * Returns a set with the difference of "this" and "other" (i.e. all elements that are unique to
 * "this").
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "this" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_difference(this, other) _set_creation_ops((this), (other), 0x06)


/**
 * Returns a set with the symmetric difference of "this" and "other" (i.e. all elements that
 * neither set has in common).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the symmetric difference of "this" and "other", or
 *                   NULL if "other" is NULL.
 */
#define set_symmetric_difference(this, other) _set_creation_ops((this), (other), 0x0F)


/**
 * Tests whether "this" is a subset of "other". (i.e. whether each element in "this" is in
 * other).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if each element in "this" is in "other", false if not.
 */
#define set_issubset(this, other) _set_bool_ops((this), (other), true)


/**
 * Tests whether "this" is a superset of "other". (i.e. whether "this" contains each element
 * from "other" - the opposite of a subset).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if "this" contains each element in "other", false if not.
 */
#define set_issuperset(this, other) _set_bool_ops((other), (this), true)


/**
 * Tests whether "this" is disjoint with "other". (i.e. if the sets have no elements in common).
 *
 * @param   this   Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if "this" and "other" have no common elements, false if they do.
 */
#define set_isdisjoint(this, other) _set_bool_ops((this), (other), false)

#endif
