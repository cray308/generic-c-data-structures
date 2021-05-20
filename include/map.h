#ifndef DS_MAP_H
#define DS_MAP_H

#include "rbtree.h"
#include "pair.h"

#define __map_entry_get_key(e) ((e)->data.first)
#define __map_data_get_key(d)  ((d).first)

/**
 * The number of elements in the map.
 */
#define map_size(m) ((int) (m)->size)


/**
 * Tests whether the map is empty.
 */
#define map_empty(m) (!((m)->root))


/**
 * Iterates through the map in-order.
 *
 * @param  it  `MapEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_iter(id, m, it) for (it = iter_begin(TREE, id, m, 0); it != iter_end(TREE, id, m, 0); iter_next(TREE, id, it))

/**
 * Iterates through the map in reverse order.
 *
 * @param  it  `MapEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_riter(id, m, it) for (it = iter_rbegin(TREE, id, m, 0); it != iter_rend(TREE, id, m, 0); iter_prev(TREE, id, it))


/**
 * Creates a new, empty map.
 *
 * @return   Pointer to the newly created map.
 */
#define map_new(id) __ds_calloc(1, sizeof(Map_##id))


/**
 * Creates a new map using `n` key-value pairs in a built-in array `arr`.
 *
 * @param   arr  Pointer to the first element of type `Pair` to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created map.
 */
#define map_new_fromArray(id, arr, n) __rbtree_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of `other`.
 *
 * @param   other  `Map` to copy.
 *
 * @return         Pointer to the newly created map.
 */
#define map_createCopy(id, other) __rbtree_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 */
#define map_free(id, m) __rbtree_free_##id(m)


/**
 * Removes all keys from the map, leaving it with a size of 0.
 */
#define map_clear(id, m) __rbtree_clear_##id(m)


/**
 * Returns the entry with a key matching `k`.
 *
 * @param   k  Key to find.
 *
 * @return     `MapEntry` whose key matches `k`, or NULL if it was not found.
 */
#define map_find(id, m, k) __rbtree_find_key_##id(m, k, false)


/**
 * Similar to `map_find`, but returns a pointer to the pair's value rather than to the entry iterator 
 * as a whole.
 *
 * @param  k  Key to find.
 */
#define map_at(id, m, k) map_at_##id(m, k)


/**
 * Inserts `pair` into the map. If the key already exists, the value is updated to that of `pair`.
 *
 * @param   pair  Key-value pair of type `Pair` to insert.
 *
 * @return        `MapEntry` corresponding to the inserted pair.
 */
#define map_insert(id, m, pair) __rbtree_insert_##id(m, pair, NULL)


/**
 * Inserts `pair` into the map, and updates `inserted` with the result of insertion. If the key already 
 * exists, the value is updated to that of `pair`.
 *
 * @param   pair      Key-value pair of type `Pair` to insert.
 * @param   inserted  Set to 1 if the pair was newly inserted, or 0 if the key already existed.
 *
 * @return            `MapEntry` corresponding to the inserted pair.
 */
#define map_insert_withResult(id, m, pair, inserted) __rbtree_insert_##id(m, pair, inserted)


/**
 * Inserts `n` key-value pairs from a built-in array `arr`.
 *
 * @param  arr  Pointer to the first element of type `Pair` to insert.
 * @param  n    Number of elements to include.
 */
#define map_insert_fromArray(id, m, arr, n) __rbtree_insert_fromArray_##id(m, arr, n)


/**
 * Inserts elements from another map in the range [`start`, `end`).
 *
 * @param  start  First `MapEntry` to insert. Must not be NULL.
 * @param  end    `MapEntry` after the last entry to insert. If this is NULL, all keys from `start`
 *                  through the greatest key in the other map will be inserted.
 */
#define map_insert_fromMap(id, m, start, end) __rbtree_insert_fromTree_##id(m, start, end)


/**
 * Removes key-value pairs in the range [`begin`, `end`).
 *
 * @param  begin  First `MapEntry` to erase.
 * @param  end    `MapEntry` after the last entry to be deleted. If this is NULL, then all keys from
 *                  `start` through the greatest key in the map will be removed.
 */
#define map_erase(id, m, begin, end) __rbtree_erase_##id(m, begin, end)


/**
 * Removes a single pair from the map whose key is equal to `key`.
 *
 * @param  key  Key to be deleted.
 */
#define map_remove_key(id, m, key) __rbtree_remove_key_##id(m, key)


/**
 * Removes `entry` from the map.
 *
 * @param  entry  `MapEntry` to remove.
 */
#define map_remove_entry(id, m, entry) __rbtree_remove_entry_##id(m, entry)


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
__gen_rbtree(id, kt, cmp_lt, Map_##id, Pair_##id, MapEntry_##id, __map_entry_get_key, __map_data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
__DS_FUNC_PREFIX_INL vt *map_at_##id(Map_##id *m, const kt key) {                                            \
    MapEntry_##id *e = __rbtree_find_key_##id(m, key, false);                                                \
    return e ? &(e->data.second) : NULL;                                                                     \
}                                                                                                            \

#endif /* DS_MAP_H */
