#ifndef DS_UNORDERED_SET_H
#define DS_UNORDERED_SET_H

#include "hash_table.h"

#define __uset_entry_get_key(e) (e)->data
#define __uset_data_get_key(d)  d
#define __uset_copy_value(x, y)
#define __uset_delete_value(x)

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * Iterates through all entries in the set.
 *
 * @param  it  Pointer to element, which is assigned to the current element.
 */
#define uset_iter(id, this, it)                                                                              \
        for (it = __htable_iter_begin_##id(this); it;                                                        \
             it = __htable_iter_next_##id(this))

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * The number of entries in the set.
 */
#define uset_size(this) (this)->size


/**
 * The current value of the set's max load factor.
 */
#define uset_max_load_factor(this) (this)->lf


/**
 * The total number of buckets in the set.
 */
#define uset_bucket_count(this) (this)->cap


/**
 * Whether the set is empty.
 */
#define uset_empty(this) !(this)->size

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty set.
 *
 * @return  Pointer to newly created set.
 */
#define uset_new(id) __htable_new_fromArray_##id(NULL, 0)


/**
 * Creates a new set using @c n elements in a built-in array @c arr .
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define uset_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of @c other .
 *
 * @param   other  @c USet to copy.
 *
 * @return         Pointer to the newly created set.
 */
#define uset_createCopy(id, other) __htable_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 */
#define uset_free(id, this) do {                                                                             \
    __htable_clear_##id(this); free((this)->buckets); free(this);                                            \
} while(0)


/**
 * Inserts @c value into the set.
 *
 * @param  value  Value to insert.
 */
#define uset_insert(id, this, value) __htable_insert_##id(this, value, NULL)


/**
 * Inserts @c value into the set, and updates @c inserted with the result of insertion.
 *
 * @param  value     Value to insert.
 * @param  inserted  Set to 1 if the value was newly inserted, or 0 if the value was already a member.
 */
#define uset_insert_withResult(id, this, value, inserted)                                                    \
        __htable_insert_##id(this, value, inserted)


/**
 * Inserts @c n elements from a built-in array @c arr .
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Whether the operation succeeded.
 */
#define uset_insert_fromArray(id, this, arr, n)                                                              \
        __htable_insert_fromArray_##id(this, arr, n)


/**
 * Tests whether @c value is in the set.
 *
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define uset_contains(id, this, value)                                                                       \
        (__htable_find_##id(this, value) != NULL)


/**
 * Removes a single element from the set whose value is equal to @c value , if it exists.
 *
 * @param  value  Value to be deleted.
 */
#define uset_remove(id, this, value) __htable_erase_##id(this, value)


/**
 * Changes the number of buckets in the set to @c nbuckets . If this is less than or equal to the current 
 * number of buckets, nothing is done.
 *
 * @param   nbuckets  New number of buckets to use in the set.
 *
 * @return            Whether the operation succeeded.
 */
#define uset_rehash(id, this, nbuckets) __htable_rehash_##id(this, nbuckets)


/**
 * If it is reasonable, sets the maximum load factor to @c lf , and may rehash the set if required.
 *
 * @param   lf  The new load factor to use.
 *
 * @return      Whether the operation succeeded.
 */
#define uset_set_load_factor(id, this, lf)                                                                   \
        __htable_set_load_factor_##id(this, lf) 


/**
 * Removes all entries from the set.
 */
#define uset_clear(id, this) __htable_clear_##id(this)


/**
 * Generates @c USet function declarations for the given value type.
 *
 * @param  id  ID to be used for the @c USet type (must be unique).
 * @param  t   Type to be stored in the set.
 */
#define gen_uset_headers(id, t)                                                                              \
        __setup_hash_table_headers(id, t, USet_##id, t, USetEntry_##id)


/**
 * Generates @c USet function definitions for the given value type.
 *
 * @param  id           ID used in @c gen_uset_headers .
 * @param  t            Type used in @c gen_uset_headers .
 * @param  cmp_eq       Macro of the form (x, y) that returns whether x is equal to y.
 * @param  addrOfValue  Macro of the form (x) that returns a pointer to x.
 *                        - For value types (i.e. int) pass @c DSDefault_addrOfVal .
 *                        - For pointer types, pass @c DSDefault_addrOfRef .
 * @param  sizeOfValue  Macro of the form (x) that returns the number of bytes in x, where x is an element
 *                       in the set. This is necessary so that the hashing function works correctly.
 *                        - For value types (i.e. int), pass @c DSDefault_sizeOfVal .
 *                        - For a string (char *), pass @c DSDefault_sizeOfStr .
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the set.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass
 *                         @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was
 *                       dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass
 *                         @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass
 *                         @c DSDefault_deepDelete here.
 */
#define gen_uset_source(id, t, cmp_eq, addrOfValue, sizeOfValue,                                             \
                        copyValue, deleteValue)                                                              \
        __setup_hash_table_source(id, t, cmp_eq, USet_##id, t, USetEntry_##id,                               \
            __uset_entry_get_key, __uset_data_get_key, addrOfValue,                                          \
            sizeOfValue, copyValue, deleteValue, __uset_copy_value,                                          \
            __uset_delete_value)

#endif /* DS_UNORDERED_SET_H */
