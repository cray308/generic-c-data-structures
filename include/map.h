#ifndef DS_MAP_H
#define DS_MAP_H

#include "rbtree.h"
#include "pair.h"

#define __map_entry_get_key(e) ((e)->data.first)
#define __map_data_get_key(d)  ((d).first)

/**
 * The number of elements in the set.
 * 
 * @param   s  Pointer to set.
 */
#define map_size(m) ((int) (m)->size)


/**
 * Tests whether the set is empty.
 * 
 * @param   s  Pointer to set.
 */
#define map_empty(m) (!((m)->root))


/**
 * Iterates through the set in-order.
 *
 * @param   id    ID used with gen_set.
 * @param   s     Pointer to set.
 * @param   it   MapEntry which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(SET, ptr) or ptr->data.
 */
#define map_iter(id, m, it) for (it = iter_begin(TREE, id, m, 0); it != iter_end(TREE, id, m, 0); iter_next(TREE, id, it))

/**
 * Iterates through the set in reverse order.
 *
 * @param   id    ID used with gen_set.
 * @param   s     Pointer to set.
 * @param   it   MapEntry which is assigned to the current element.
 *                May be dereferenced with iter_deref(SET, ptr) or ptr->data.
 */
#define map_riter(id, m, it) for (it = iter_rbegin(TREE, id, m, 0); it != iter_rend(TREE, id, m, 0); iter_prev(TREE, id, it))


/**
 * Creates a new, empty set.
 *
 * @param   id  ID used with gen_set.
 *
 * @return      Pointer to the newly created set.
 */
#define map_new(id) __ds_calloc(1, sizeof(Map_##id))


/**
 * Creates a new set using the elements in a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define map_new_fromArray(id, arr, n) __rbtree_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of an existing Set.
 *
 * @param   id   ID used with gen_set.
 * @param   set  Set to copy.
 *
 * @return       Pointer to the newly created set.
 */
#define map_createCopy(id, other) __rbtree_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define map_free(id, m) __rbtree_free_##id(m)


/**
 * Removes all elements from the set, leaving it with a size of 0.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define map_clear(id, m) __rbtree_clear_##id(m)


/**
 * Returns the MapEntry with a key matching `k`.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be found.
 *
 * @return         Pointer to MapEntry whose key matches `k`, or NULL if it was not found.
 */
#define map_find(id, m, k) __rbtree_find_key_##id(m, k, false)


/**
 * Similar to map_find, but returns a pointer to the pair's value rather than to the entry iterator as a whole.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to find.
 */
#define map_at(id, m, k) map_at_##id(m, k)


/**
 * Inserts the value into the set.
 *
 * @param   id     ID used with gen_set.
 * @param   m      Pointer to set.
 * @param   p      Key-value pair to insert.
 */
#define map_insert(id, m, p) __rbtree_insert_##id(m, p, NULL)


/**
 * [map_insert_withResult description]
 *
 * @param   id        [description]
 * @param   m         [description]
 * @param   value     [description]
 * @param   inserted  [description]
 *
 * @return            [description]
 */
#define map_insert_withResult(id, m, p, inserted) __rbtree_insert_##id(m, p, inserted)


/**
 * Inserts elements from a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   s    Pointer to set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to insert from `arr`.
 */
#define map_insert_fromArray(id, m, arr, n) __rbtree_insert_fromArray_##id(m, arr, n)


/**
 * Inserts elements from another Set in the range [start, end).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   start  Pointer to first SetEntry to insert. Must not be NULL.
 * @param   end    Pointer after the last SetEntry to insert. If this is NULL, all elements from
 *                   `start` through the end (greatest element) of the other set will be inserted.
 */
#define map_insert_fromMap(id, m, start, end) __rbtree_insert_fromTree_##id(m, start, end)


/**
 * Erases elements in the range [begin,end).
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  begin  First element to erase. If this is NULL, it defaults to the smallest element
 *                  in the set.
 * @param  end    SetEntry AFTER the last element to be deleted. If this is NULL, then all
 *                  elements from start through the greatest element in the set will be removed.
 */
#define map_erase(id, m, begin, end) __rbtree_erase_##id(m, begin, end)


/**
 * Removes a single entry from the set whose value is equal to `value`.
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  value  Pointer to the value to be deleted.
 */
#define map_remove_key(id, m, key) __rbtree_remove_key_##id(m, key)


/**
 * [map_remove_entry description]
 *
 * @param   id  [description]
 * @param   m   [description]
 * @param   e   [description]
 *
 * @return      [description]
 */
#define map_remove_entry(id, m, e) __rbtree_remove_entry_##id(m, e)


/**
 * [gen_map description]
 *
 * @param   id           [description]
 * @param   kt           [description]
 * @param   t            [description]
 * @param   cmp_lt       [description]
 * @param   copyKey      [description]
 * @param   deleteKey    [description]
 * @param   copyValue    [description]
 * @param   deleteValue  [description]
 *
 * @return               [description]
 */
#define gen_map(id, kt, t, cmp_lt, copyKey, deleteKey, copyValue, deleteValue)                               \
gen_pair(id, kt, t)                                                                                          \
__gen_rbtree(id, kt, cmp_lt, Map_##id, Pair_##id, MapEntry_##id, __map_entry_get_key, __map_data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *map_at_##id(Map_##id *m, const kt key) {                                             \
    MapEntry_##id *e = __rbtree_find_key_##id(m, key, false);                                                \
    return e ? &(e->data.second) : NULL;                                                                     \
}                                                                                                            \

#endif /* DS_MAP_H */
