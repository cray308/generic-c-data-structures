#ifndef DS_UNORDERED_SET_H
#define DS_UNORDERED_SET_H

#include "hash_table.h"

#define __uset_entry_get_key(e) ((e)->data)
#define __uset_data_get_key(d)  (d)
#define __uset_copy_value(x, y)
#define __uset_delete_value(x)


/**
 * The number of entries in the set.
 */
#define uset_size(s) ((int) (s)->size)


/**
 * The current value of the set's max load factor.
 */
#define uset_max_load_factor(s) ((s)->lf)


/**
 * The total number of buckets in the set.
 */
#define uset_bucket_count(s) ((s)->cap)


/**
 * Whether the set is empty.
 */
#define uset_empty(s) (!((s)->size))


/**
 * Iterates through all entries in the set.
 *
 * @param  it  Pointer to element, which is assigned to the current element.
 */
#define uset_iter(id, s, it) for (it = iter_begin_HTABLE(id, s); it != iter_end_HTABLE(id, s); iter_next_HTABLE(id, s, it))


/**
 * Creates a new, empty set.
 *
 * @return  Pointer to newly created set.
 */
#define uset_new(id)  __htable_new_##id() 


/**
 * Creates a new set using `n` elements in a built-in array `arr`.
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define uset_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of `other`.
 *
 * @param   other  `USet` to copy.
 *
 * @return         Pointer to the newly created set.
 */
#define uset_createCopy(id, other) __htable_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 */
#define uset_free(id, s) __htable_free_##id(s)


/**
 * Inserts `value` into the set.
 *
 * @param  value  Value to insert.
 */
#define uset_insert(id, s, value) __htable_insert_##id(s, value, NULL)


/**
 * Inserts `value` into the set, and updates `inserted` with the result of insertion.
 *
 * @param  value     Value to insert.
 * @param  inserted  Set to 1 if the value was newly inserted, or 0 if the value was already a member.
 */
#define uset_insert_withResult(id, s, value, inserted) __htable_insert_##id(s, value, inserted)


/**
 * Inserts `n` elements from a built-in array `arr`.
 *
 * @param  arr  Pointer to the first element to insert.
 * @param  n    Number of elements to include.
 */
#define uset_insert_fromArray(id, s, arr, n) __htable_insert_fromArray_##id(s, arr, n)


/**
 * Tests whether `value` is in the set.
 *
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define uset_contains(id, s, value) (__htable_find_##id(s, value) != NULL)


/**
 * Removes a single element from the set whose value is equal to `value`, if it exists.
 *
 * @param  value  Value to be deleted.
 */
#define uset_remove(id, s, value) __htable_erase_##id(s, value)


/**
 * Changes the number of buckets in the set to `nbuckets`. If this is less than or equal to the 
 * current number of buckets, nothing is done.
 *
 * @param  nbuckets  New number of buckets to use in the set.
 */
#define uset_rehash(id, s, nbuckets) __htable_rehash_##id(s, nbuckets)


/**
 * If it is reasonable, sets the maximum load factor to `lf`, and may rehash the set if required.
 *
 * @param  lf  The new load factor to use.
 */
#define uset_set_load_factor(id, s, lf) __htable_set_load_factor_##id(s, lf) 


/**
 * Removes all entries from the set.
 */
#define uset_clear(id, s) __htable_clear_##id(s)


/**
 * Generates `USet` code for the given value type.
 *
 * @param  id           ID to be used for the `USet` type (must be unique).
 * @param  t            Type to be stored in the set.
 * @param  cmp_eq       Macro of the form (x, y) that returns whether x is equal to y.
 * @param  addrOfValue  Macro of the form (x) that returns a pointer to x.
 *                        - For value types (i.e. int) pass DSDefault_addrOfVal.
 *                        - For pointer types, pass DSDefault_addrOfRef.
 * @param  sizeOfValue  Macro of the form (x) that returns the number of bytes in x, where x is an element in the set.
 *                        This is necessary so that the hashing function works correctly.
 *                        - For value types (i.e. int), pass DSDefault_sizeOfVal.
 *                        - For a string (char *), pass DSDefault_sizeOfStr.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the set.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_uset(id, t, cmp_eq, addrOfValue, sizeOfValue, copyValue, deleteValue)                            \
__gen_hash_table(id, t, cmp_eq, USet_##id, t, USetEntry_##id, __uset_entry_get_key, __uset_data_get_key, addrOfValue, sizeOfValue, copyValue, deleteValue, __uset_copy_value, __uset_delete_value) \

#endif /* DS_UNORDERED_SET_H */
