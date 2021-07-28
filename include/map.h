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
 * @param  it  @c MapEntry which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_iter(id, this, it) for (it = iter_begin(AVLTREE, id, this, 0); it != iter_end(AVLTREE, id, this, 0); iter_next(AVLTREE, id, it))

/**
 * Iterates through the map in reverse order.
 *
 * @param  it  @c MapEntry which is assigned to the current element. May be dereferenced with it->data.
 */
#define map_riter(id, this, it) for (it = iter_rbegin(AVLTREE, id, this, 0); it != iter_rend(AVLTREE, id, this, 0); iter_prev(AVLTREE, id, it))


/**
 * Creates a new, empty map.
 *
 * @return  Pointer to the newly created map.
 */
#define map_new(id) __avltree_new_fromArray_##id(NULL, 0)


/**
 * Creates a new map using @c n key-value pairs in a built-in array @c arr .
 *
 * @param   arr  Pointer to the first element of type @c Pair to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created map.
 */
#define map_new_fromArray(id, arr, n) __avltree_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of @c other .
 *
 * @param   other  @c Map to copy.
 *
 * @return         Pointer to the newly created map.
 */
#define map_createCopy(id, other) __avltree_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 */
#define map_free(id, this) do { map_clear(id, this); free(this); } while(0)


/**
 * Removes all keys from the map, leaving it with a size of 0.
 */
#define map_clear(id, this) __avltree_erase_##id(this, __avl_successor_##id((this)->root), NULL)


/**
 * Returns the entry with a key matching @c k .
 *
 * @param   k  Key to find.
 *
 * @return     @c MapEntry whose key matches @c k , or NULL if it was not found.
 */
#define map_find(id, this, k) __avltree_find_key_##id(this, k, 0)


/**
 * Similar to @c map_find , but returns a pointer to the pair's value rather than to the entry iterator 
 * as a whole.
 *
 * @param  k  Key to find.
 */
#define map_at(id, this, k) map_at_##id(this, k)


/**
 * Inserts @c pair into the map. If the key already exists, the value is updated to that of @c pair .
 *
 * @param   pair  Key-value pair of type @c Pair to insert.
 *
 * @return        @c MapEntry corresponding to the inserted pair.
 */
#define map_insert(id, this, pair) __avltree_insert_##id(this, pair, NULL)


/**
 * Inserts @c pair into the map, and updates @c inserted with the result of insertion. If the key already 
 * exists, the value is updated to that of @c pair .
 *
 * @param   pair      Key-value pair of type @c Pair to insert.
 * @param   inserted  Set to 1 if the pair was newly inserted, or 0 if the key already existed.
 *
 * @return            @c MapEntry corresponding to the inserted pair.
 */
#define map_insert_withResult(id, this, pair, inserted) __avltree_insert_##id(this, pair, inserted)


/**
 * Inserts @c n key-value pairs from a built-in array @c arr .
 *
 * @param  arr  Pointer to the first element of type @c Pair to insert.
 * @param  n    Number of elements to include.
 */
#define map_insert_fromArray(id, this, arr, n) __avltree_insert_fromArray_##id(this, arr, n)


/**
 * Inserts elements from another map in the range [@c start , @c end ).
 *
 * @param  start  First @c MapEntry to insert. Must not be NULL.
 * @param  end    @c MapEntry after the last entry to insert. If this is NULL, all keys from @c start
 *                  through the greatest key in the other map will be inserted.
 */
#define map_insert_fromMap(id, this, start, end) __avltree_insert_fromTree_##id(this, start, end)


/**
 * Removes key-value pairs in the range [@c begin , @c end ).
 *
 * @param  begin  First @c MapEntry to erase.
 * @param  end    @c MapEntry after the last entry to be deleted. If this is NULL, then all keys from
 *                  @c begin through the greatest key in the map will be removed.
 */
#define map_erase(id, this, begin, end) __avltree_erase_##id(this, begin, end)


/**
 * Removes a single pair from the map whose key is equal to @c key .
 *
 * @param  key  Key to be deleted.
 */
#define map_remove_key(id, this, key) __avltree_remove_entry_##id(this, __avltree_find_key_##id(this, key, 0))


/**
 * Removes @c entry from the map.
 *
 * @param  entry  @c MapEntry to remove.
 */
#define map_remove_entry(id, this, entry) __avltree_remove_entry_##id(this, entry)


/**
 * Generates @c Map function declarations for the given key type and value type.
 *
 * @param  id  ID to be used for the @c Map , @c MapEntry , and @c Pair types (must be unique).
 * @param  kt  Key type.
 * @param  vt  Value type.
 */
#define gen_map_headers(id, kt, vt)                                                                          \
                                                                                                             \
gen_pair(id, kt, vt)                                                                                         \
__setup_avltree_headers(id, kt, Map_##id, Pair_##id, MapEntry_##id)                                          \
                                                                                                             \
vt *map_at_##id(Map_##id *this, const kt key);                                                               \


/**
 * Generates @c Map function definitions for the given key type and value type.
 *
 * @param  id           ID used in @ gen_map_headers .
 * @param  kt           Key type used in @ gen_map_headers .
 * @param  vt           Value type used in @ gen_map_headers .
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyKey      Macro of the form (x, y) which copies y into x to store the pair's key in the map.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the key is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteKey    Macro of the form (x), which is a complement to @c copyKey ; if memory was dynamically allocated in @c copyKey , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyKey , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyKey , pass @c DSDefault_deepDelete here.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the pair's value in the map.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_map_source(id, kt, vt, cmp_lt, copyKey, deleteKey, copyValue, deleteValue)                       \
                                                                                                             \
__setup_avltree_source(id, kt, Map_##id, Pair_##id, MapEntry_##id, cmp_lt, __map_entry_get_key, __map_data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
vt* map_at_##id(Map_##id *this, const kt key) {                                                              \
    MapEntry_##id *e = __avltree_find_key_##id(this, key, 0);                                                \
    return e ? &(e->data.second) : NULL;                                                                     \
}                                                                                                            \

#endif /* DS_MAP_H */
