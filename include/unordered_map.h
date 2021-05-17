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
 * Similar to umap_find, but returns a pointer to the pair's value rather than to the pair as a whole.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to find.
 */
#define umap_at(id, m, k) umap_at_##id(m, k)


/**
 * Iterates through all entries in the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  it  Pointer to pair which is assigned to the current element.
 */
#define umap_iter(id, m, it) for (it = iter_begin_HTABLE(id, m); it != iter_end_HTABLE(id, m); iter_next_HTABLE(id, m, it))


/**
 * Inserts the provided key-value pair into the map. If the key already exists, it will be updated
 *   with the provided value.
 *
 * @param  id        ID used with gen_umap.
 * @param  m         Pointer to map.
 * @param  p         Pair where the first element is the key and the second is the value.
 *
 * @return           Pointer to the inserted key-value pair.
 */
#define umap_insert(id, m, p) __htable_insert_##id(m, p, NULL)


/**
 * [umap_insert_withResult description]
 *
 * @param   id        [description]
 * @param   m         [description]
 * @param   p         [description]
 * @param   inserted  Pointer to int which is set to 1 if a new pair was inserted, or 0 if not.
 *
 * @return            [description]
 */
#define umap_insert_withResult(id, m, p, inserted) __htable_insert_##id(m, p, inserted)


/**
 * [umap_insert_fromArray description]
 *
 * @param   id   [description]
 * @param   m    [description]
 * @param   arr  [description]
 * @param   n    [description]
 */
#define umap_insert_fromArray(id, m, arr, n) __htable_insert_fromArray_##id(m, arr, n)


/**
 * Finds the provided key.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to be used.
 *
 * @return     If the key is found, returns a pointer to the pair (key, value) corresponding to
 *               that key. If the key does not exist, returns NULL.
 */
#define umap_find(id, m, key) __htable_find_##id(m, key)


/**
 * Removes the key and its corresponding value from the map, if it exists.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to be used.
 */
#define umap_remove_key(id, m, key) __htable_erase_##id(m, key)


/**
 * Creates a new unordered map.
 *
 * @param  id  ID used with gen_umap.
 *
 * @return     Pointer to newly created map.
 */
#define umap_new(id)  __htable_new_##id() 


/**
 * [umap_new_fromArray description]
 *
 * @param   id   [description]
 * @param   arr  [description]
 * @param   n    [description]
 *
 * @return       Pointer to newly created map.
 */
#define umap_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * Deletes all elements and frees the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 */
#define umap_free(id, m) __htable_free_##id(m) 


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

/* --------------------------------------------------------------------------
 * Unordered map iterators
 * -------------------------------------------------------------------------- */

#define iter_deref_UMAP(p)         ((p)->second)

/**
 * Generates unordered map (hash table) code for a specified key type, value type and ID.
 *
 * @param   id        ID to be used for the map (must be unique).
 * @param   kt        Key type to be used.
 * @param   key_decl  One of VALUE, PTR, or PTR_COPY.
 *                     - use VALUE if the key type is not a pointer (i.e. int, double, char, etc)
 *                     - use PTR if the key type is a pointer (i.e. char *) and the pointed-to value does not need to be copied.
 *                     - use PTR_COPY if the key type is a pointer and the pointed-to value *should* be copied.
 *                     - Note that PTR_COPY uses strcpy iternally, so using other types of pointers will cause undefined behavior.
 * @param   t         Value type to be stored in the map.
 * @param   cmp_lt    Macro of the form (x, y) that returns whether x is strictly less than y.
 */

/**
 * [gen_umap description]
 *
 * @param   id           [description]
 * @param   kt           [description]
 * @param   t            [description]
 * @param   cmp_eq       [description]
 * @param   addrOfKey    [description]
 * @param   sizeOfKey    [description]
 * @param   copyKey      [description]
 * @param   deleteKey    [description]
 * @param   copyValue    [description]
 * @param   deleteValue  [description]
 *
 * @return               [description]
 */
#define gen_umap(id, kt, t, cmp_eq, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue)                                                                \
                                                                                                             \
gen_pair(id, kt, t)                                                                                          \
__gen_hash_table(id, kt, cmp_eq, UMap_##id, Pair_##id, UMapEntry_##id, __umap_entry_get_key, __umap_data_get_key, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *umap_at_##id(UMap_##id *m, const kt key) {                                           \
    Pair_##id *p = __htable_find_##id(m, key);                                                               \
    return p ? &(p->second) : NULL;                                                                          \
}                                                                                                            \

#endif /* DS_UNORDERED_MAP_H */
