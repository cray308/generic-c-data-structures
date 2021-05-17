#ifndef DS_UNORDERED_MAP_H
#define DS_UNORDERED_MAP_H

#include "hash_table.h"

#define __umap_entry_get_key(e) ((e)->data.first)
#define __umap_data_get_key(d)  ((d).first)

/**
 * The number of entries in the map.
 *
 * @param   m  Pointer to map.
 */
#define umap_size(m) ((int) (m)->size)


/**
 * The current value of the map's max load factor.
 *
 * @param   m  Pointer to map.
 */
#define umap_max_load_factor(m) ((m)->lf)


/**
 * Whether the map is empty.
 *
 * @param   m  Pointer to map.
 */
#define umap_empty(m) (!((m)->size))


/**
 * Iterates through all entries in the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  it  Pointer to pair which is assigned to the current element.
 */
#define umap_iter(id, m, it) for (it = iter_begin_HTABLE(id, m); it != iter_end_HTABLE(id, m); iter_next_HTABLE(id, m, it))


/**
 * Creates a new, empty map.
 *
 * @param  id  ID used with gen_umap.
 *
 * @return     Pointer to newly created map.
 */
#define umap_new(id)  __htable_new_##id() 


/**
 * Creates a new map using the key-value pairs in a built-in array data type.
 *
 * @param   id   ID used with gen_umap.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created map.
 */
#define umap_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of an existing UMap.
 *
 * @param   id     ID used with gen_umap.
 * @param   other  Map to copy.
 *
 * @return         Pointer to the newly created map.
 */
#define umap_createCopy(id, other) __htable_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 */
#define umap_free(id, m) __htable_free_##id(m)


/**
 * Inserts the provided key-value pair into the map. If the key already exists, it will be updated
 *   with the provided value.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  p   Key-value pair to insert.
 *
 * @return     Pointer to the inserted key-value pair.
 */
#define umap_insert(id, m, p) __htable_insert_##id(m, p, NULL)


/**
 * Inserts the key-value pair into the map, and updates `inserted` with the result of insertion.
 *
 * @param   id        ID used with gen_umap.
 * @param   m         Pointer to map.
 * @param   p         Key-value pair to insert.
 * @param   inserted  Pointer to int which is set to 1 if a new pair was inserted, or 0 if not.
 *
 * @return            Pointer to the inserted key-value pair.
 */
#define umap_insert_withResult(id, m, p, inserted) __htable_insert_##id(m, p, inserted)


/**
 * Inserts multiple key-value pairs from a built-in array data type.
 *
 * @param   id   ID used with gen_umap.
 * @param   m    Pointer to map.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to insert from `arr`.
 */
#define umap_insert_fromArray(id, m, arr, n) __htable_insert_fromArray_##id(m, arr, n)


/**
 * Finds the provided key.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to find.
 *
 * @return     Pointer to the pair (key, value) whose key matches `k`, or NULL if it was not found.
 */
#define umap_find(id, m, k) __htable_find_##id(m, k)


/**
 * Similar to umap_find, but returns a pointer to the pair's value rather than to the pair as a whole.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to find.
 */
#define umap_at(id, m, k) umap_at_##id(m, k)


/**
 * Removes a single pair from the map whose key is equal to `k`.
 *
 * @param  id   ID used with gen_umap.
 * @param  m    Pointer to map.
 * @param  k    Key to be deleted.
 */
#define umap_remove_key(id, m, k) __htable_erase_##id(m, k)


/**
 * Modifies the number of buckets in the map. If this is less than or equal to the current number
 *   of buckets, nothing is done.
 *
 * @param  id        ID used with gen_umap.
 * @param  m         Pointer to map.
 * @param  nbuckets  New number of buckets to use in the map.
 */
#define umap_rehash(id, m, nbuckets) __htable_rehash_##id(m, nbuckets)


/**
 * If the provided load factor is sensible, sets the maximum load factor to the provided
 *   value, and may rehash the map if required.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  lf  The new load factor to use.
 */
#define umap_set_load_factor(id, m, lf) __htable_set_load_factor_##id(m, lf) 


/**
 * Removes all entries from the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 */
#define umap_clear(id, m) __htable_clear_##id(m)

/**
 * Generates unordered map code for the given key type and value type.
 *
 * @param   id           ID to be used for the UMap and Pair types (must be unique).
 * @param   kt           Key type.
 * @param   vt           Value type.
 * @param   cmp_eq       Macro of the form (x, y) that returns whether x is equal to y.
 * @param   addrOfKey    Macro of the form (x) that returns a pointer to x.
 *                         - For value types (i.e. int) pass DSDefault_addrOfVal.
 *                         - For pointer types, pass DSDefault_addrOfRef.
 * @param   sizeOfKey    Macro of the form (x) that returns the number of bytes in x, where x is a key for the map.
 *                         This is necessary so that the hashing function works correctly.
 *                         - For value types (i.e. int), pass DSDefault_sizeOfVal.
 *                         - For a string (char *), pass DSDefault_sizeOfStr.
 * @param   copyKey      Macro of the form (x, y) which copies y into x to store the pair's key in the map.
 *                         - If no special copying is required, pass DSDefault_shallowCopy.
 *                         - If the key is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param   deleteKey    Macro of the form (x), which is a complement to `copyKey`; if memory was dynamically allocated in `copyKey`, it should be freed here.
 *                         - If DSDefault_shallowCopy was used in `copyKey`, pass DSDefault_shallowDelete here.
 *                         - If DSDefault_deepCopyStr was used in `copyKey`, pass DSDefault_deepDelete here.
 * @param   copyValue    Macro of the form (x, y) which copies y into x to store the pair's value in the map.
 *                         - If no special copying is required, pass DSDefault_shallowCopy.
 *                         - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param   deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                         - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                         - If DSDefault_deepCopyStr  was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_umap(id, kt, vt, cmp_eq, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue)       \
                                                                                                             \
gen_pair(id, kt, vt)                                                                                         \
__gen_hash_table(id, kt, cmp_eq, UMap_##id, Pair_##id, UMapEntry_##id, __umap_entry_get_key, __umap_data_get_key, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
__DS_FUNC_PREFIX_INL vt *umap_at_##id(UMap_##id *m, const kt key) {                                          \
    Pair_##id *p = __htable_find_##id(m, key);                                                               \
    return p ? &(p->second) : NULL;                                                                          \
}                                                                                                            \

#endif /* DS_UNORDERED_MAP_H */
