#ifndef DS_UNORDERED_SET_H
#define DS_UNORDERED_SET_H

#include "hash_table.h"

#define __uset_entry_get_key(e) ((e)->data)
#define __uset_data_get_key(d)  (d)
#define __uset_copy_value(x, y)
#define __uset_delete_value(x)


/**
 * The number of entries in the map.
 *
 * @param   s  Pointer to map.
 */
#define uset_size(s) ((int) (s)->size)


/**
 * The current value of the map's max load factor.
 *
 * @param   s  Pointer to map.
 */
#define uset_max_load_factor(s) ((s)->lf)


/**
 * Whether the map is empty.
 *
 * @param   s  Pointer to map.
 */
#define uset_empty(s) (!((s)->size))


/**
 * Iterates through all entries in the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  s   Pointer to map.
 * @param  it  Pointer to pair which is assigned to the current element.
 */
#define uset_iter(id, s, it) for (it = iter_begin_HTABLE(id, s); it != iter_end_HTABLE(id, s); iter_next_HTABLE(id, s, it))


/**
 * Inserts the provided value into the map, if it does not already exist. If the key already exists, it will be updated
 *   with the provided value.
 *
 * @param  id        ID used with gen_uset.
 * @param  s         Pointer to map.
 * @param  p         Pair where the first element is the key and the second is the value.
 * @param  inserted  Pointer to int which is set to 1 if a new value was inserted, or 0 if not.
 *
 * @return           Pointer to the inserted value.
 */
#define uset_insert(id, s, value) __htable_insert_##id(s, value, NULL)


/**
 * [uset_insert_withResult description]
 *
 * @param   id        [description]
 * @param   s         [description]
 * @param   value     [description]
 * @param   inserted  [description]
 *
 * @return            [description]
 */
#define uset_insert_withResult(id, s, value, inserted) __htable_insert_##id(s, value, inserted)


/**
 * [uset_insert_fromArray description]
 *
 * @param   id   [description]
 * @param   s    [description]
 * @param   arr  [description]
 * @param   n    [description]
 */
#define uset_insert_fromArray(id, s, arr, n) __htable_insert_fromArray_##id(s, arr, n)


/**
 * Finds the provided key.
 *
 * @param  id  ID used with gen_umap.
 * @param  s   Pointer to map.
 * @param  k   Key to be used.
 *
 * @return     If the key is found, returns a pointer to that key. If the key does not exist,
 *               returns NULL.
 */
#define uset_contains(id, s, value) (__htable_find_##id(s, value) != NULL)


/**
 * Removes the key from the map, if it exists.
 *
 * @param  id     ID used with gen_umap.
 * @param  s      Pointer to map.
 * @param  value  Key to be removed.
 */
#define uset_remove(id, s, value) __htable_erase_##id(s, value)


/**
 * Creates a new, empty unordered set.
 *
 * @param  id  ID used with gen_umap.
 *
 * @return     Pointer to newly created set.
 */
#define uset_new(id)  __htable_new_##id() 


/**
 * [uset_new_fromArray description]
 *
 * @param   id   [description]
 * @param   arr  [description]
 * @param   n    [description]
 *
 * @return       Pointer to newly created set.
 */
#define uset_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * [uset_createCopy description]
 *
 * @param   id  [description]
 * @param   s   [description]
 *
 * @return      Pointer to newly created set.
 */
#define uset_createCopy(id, s) __htable_createCopy_##id(s)


/**
 * Deletes all elements and frees the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  s   Pointer to map.
 */
#define uset_free(id, s) __htable_free_##id(s) 


/**
 * Modifies the number of buckets in the map. If this is less than or equal to the current number
 *   of buckets, nothing is done.
 *
 * @param  id        ID used with gen_umap.
 * @param  s         Pointer to map.
 * @param  nbuckets  New number of buckets to use in the map.
 */
#define uset_rehash(id, s, nbuckets) __htable_rehash_##id(s, nbuckets)


/**
 * If the provided load factor is sensible, sets the maximum load factor to the provided
 *   value, and may rehash the map if required.
 *
 * @param  id  ID used with gen_umap.
 * @param  s   Pointer to map.
 * @param  lf  The new load factor to use.
 */
#define uset_set_load_factor(id, s, lf) __htable_set_load_factor_##id(s, lf) 


/**
 * Removes all entries from the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  s   Pointer to map.
 */
#define uset_clear(id, s) __htable_clear_##id(s)

/* --------------------------------------------------------------------------
 * Unordered set iterators
 * -------------------------------------------------------------------------- */

#define iter_deref_USET(p)         (*(p))

/**
 * Generates unordered set code for a specified type and ID.
 *
 * @param   id        ID to be used for the map (must be unique).
 * @param   kt        Key type to be used.
 * @param   key_decl  One of VALUE, PTR, or PTR_COPY.
 *                     - use VALUE if the key type is not a pointer (i.e. int, double, char, etc)
 *                     - use PTR if the key type is a pointer (i.e. char *) and the pointed-to value does not need to be copied.
 *                     - use PTR_COPY if the key type is a pointer and the pointed-to value *should* be copied.
 *                     - Note that PTR_COPY uses strcpy iternally, so using other types of pointers will cause undefined behavior.
 * @param   t         Value type to be stored in the map.
 * @param   cmp_eq    Macro of the form (x, y) that returns whether x is equal to than y.
 */

/**
 * [gen_uset description]
 *
 * @param   id         [description]
 * @param   t          [description]
 * @param   cmp_eq     [description]
 * @param   addrOfKey  [description]
 * @param   sizeOfKey  [description]
 * @param   copyKey    [description]
 * @param   deleteKey  [description]
 *
 * @return             [description]
 */
#define gen_uset(id, t, cmp_eq, addrOfKey, sizeOfKey, copyKey, deleteKey)                                    \
__gen_hash_table(id, t, cmp_eq, USet_##id, t, USetEntry_##id, __uset_entry_get_key, __uset_data_get_key, addrOfKey, sizeOfKey, copyKey, deleteKey, __uset_copy_value, __uset_delete_value) \

#endif /* DS_UNORDERED_SET_H */
