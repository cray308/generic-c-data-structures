#ifndef DS_MAP_H
#define DS_MAP_H

#include "avltree.h"
#include "pair.h"

#define __map_entry_get_key(e) ((e)->data.first)
#define __map_data_get_key(d)  ((d).first)

/**
 * The number of elements in the map.
 */
#define map_size(this) ((int) (this)->size)


/**
 * Tests whether the map is empty.
 */
#define map_empty(this) (!((this)->root))


/**
 * Iterates through the map in-order.
 *
 * @param  it  `MapEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_iter(id, this, it) for (it = iter_begin(AVLTREE, id, this, 0); it != iter_end(AVLTREE, id, this, 0); iter_next(AVLTREE, id, it))

/**
 * Iterates through the map in reverse order.
 *
 * @param  it  `MapEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_riter(id, this, it) for (it = iter_rbegin(AVLTREE, id, this, 0); it != iter_rend(AVLTREE, id, this, 0); iter_prev(AVLTREE, id, it))


/**
 * Creates a new, empty map.
 *
 * @return  Pointer to the newly created map.
 */
#define map_new(id) __avltree_new_##id()


/**
 * Creates a new map using `n` key-value pairs in a built-in array `arr`.
 *
 * @param   arr  Pointer to the first element of type `Pair` to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created map.
 */
#define map_new_fromArray(id, arr, n) __avltree_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of `other`.
 *
 * @param   other  `Map` to copy.
 *
 * @return         Pointer to the newly created map.
 */
#define map_createCopy(id, other) __avltree_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 */
#define map_free(id, this) __avltree_free_##id(this)


/**
 * Removes all keys from the map, leaving it with a size of 0.
 */
#define map_clear(id, this) __avltree_clear_##id(this)


/**
 * Returns the entry with a key matching `k`.
 *
 * @param   k  Key to find.
 *
 * @return     `MapEntry` whose key matches `k`, or NULL if it was not found.
 */
#define map_find(id, this, k) __avltree_find_key_##id(this, k, 0)


/**
 * Similar to `map_find`, but returns a pointer to the pair's value rather than to the entry iterator 
 * as a whole.
 *
 * @param  k  Key to find.
 */
#define map_at(id, this, k) map_at_##id(this, k)


/**
 * Inserts `pair` into the map. If the key already exists, the value is updated to that of `pair`.
 *
 * @param   pair  Key-value pair of type `Pair` to insert.
 *
 * @return        `MapEntry` corresponding to the inserted pair.
 */
#define map_insert(id, this, pair) __avltree_insert_##id(this, pair, NULL)


/**
 * Inserts `pair` into the map, and updates `inserted` with the result of insertion. If the key already 
 * exists, the value is updated to that of `pair`.
 *
 * @param   pair      Key-value pair of type `Pair` to insert.
 * @param   inserted  Set to 1 if the pair was newly inserted, or 0 if the key already existed.
 *
 * @return            `MapEntry` corresponding to the inserted pair.
 */
#define map_insert_withResult(id, this, pair, inserted) __avltree_insert_##id(this, pair, inserted)


/**
 * Inserts `n` key-value pairs from a built-in array `arr`.
 *
 * @param  arr  Pointer to the first element of type `Pair` to insert.
 * @param  n    Number of elements to include.
 */
#define map_insert_fromArray(id, this, arr, n) __avltree_insert_fromArray_##id(this, arr, n)


/**
 * Inserts elements from another map in the range [`start`, `end`).
 *
 * @param  start  First `MapEntry` to insert. Must not be NULL.
 * @param  end    `MapEntry` after the last entry to insert. If this is NULL, all keys from `start`
 *                  through the greatest key in the other map will be inserted.
 */
#define map_insert_fromMap(id, this, start, end) __avltree_insert_fromTree_##id(this, start, end)


/**
 * Removes key-value pairs in the range [`begin`, `end`).
 *
 * @param  begin  First `MapEntry` to erase.
 * @param  end    `MapEntry` after the last entry to be deleted. If this is NULL, then all keys from
 *                  `start` through the greatest key in the map will be removed.
 */
#define map_erase(id, this, begin, end) __avltree_erase_##id(this, begin, end)


/**
 * Removes a single pair from the map whose key is equal to `key`.
 *
 * @param  key  Key to be deleted.
 */
#define map_remove_key(id, this, key) __avltree_remove_key_##id(this, key)


/**
 * Removes `entry` from the map.
 *
 * @param  entry  `MapEntry` to remove.
 */
#define map_remove_entry(id, this, entry) __avltree_remove_entry_##id(this, entry)


/**
 * Generates `Map` code for the given key type and value type.
 *
 * @param  id           ID to be used for the `Map`, `MapEntry`, and `Pair` types (must be unique).
 * @param  kt           Key type.
 * @param  vt           Value type.
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyKey      Macro of the form (x, y) which copies y into x to store the pair's key in the map.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the key is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteKey    Macro of the form (x), which is a complement to `copyKey`; if memory was dynamically allocated in `copyKey`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyKey`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyKey`, pass DSDefault_deepDelete here.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the pair's value in the map.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_map(id, kt, vt, cmp_lt, copyKey, deleteKey, copyValue, deleteValue)                              \
gen_pair(id, kt, vt)                                                                                         \
__gen_avltree(id, kt, cmp_lt, Map_##id, Pair_##id, MapEntry_##id, __map_entry_get_key, __map_data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
__DS_FUNC_PREFIX_INL vt *map_at_##id(Map_##id *this, const kt key) {                                         \
    MapEntry_##id *e = __avltree_find_key_##id(this, key, 0);                                                \
    return e ? &(e->data.second) : NULL;                                                                     \
}                                                                                                            \

#endif /* DS_MAP_H */
