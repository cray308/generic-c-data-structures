#ifndef DS_MAP_H
#define DS_MAP_H

#include "avltree.h"

#define __map_entry_get_key(e) (e)->data.first
#define __map_data_get_key(d)  (d).first

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * Returns the next entry in terms of increasing keys, if it exists.
 *
 * @param   e  @c MapEntry* : Entry to use.
 *
 * @return     @c MapEntry* : Next entry.
 */
#define mapEntry_getNext(id, e) __avl_inorder_successor_##id(e)


/**
 * Returns the previous entry in terms of increasing keys, if it exists.
 *
 * @param   e  @c MapEntry* : Entry to use.
 *
 * @return     @c MapEntry* : Previous entry.
 */
#define mapEntry_getPrev(id, e) __avl_inorder_predecessor_##id(e)


/**
 * Advances the entry by @c n positions. A negative number means to move 
 * backwards.
 *
 * @param  e  @c MapEntry** : Address of entry.
 * @param  n  @c long : Number of positions to advance.
 */
#define mapEntry_advance(id, e, n) __avlEntry_advance_##id(e, n)


/**
 * Returns the number of elements between @c first and @c last .
 *
 * @param   first  @c MapEntry* : Entry to start at.
 * @param   last   @c MapEntry* : Entry to end at. This must be reachable in
 *                  the forward direction by @c first .
 *
 * @return         @c long : Number of elements between @c first and @c last ,
 *                 or if @c last is not reachable, returns -1.
 */
#define mapEntry_distance(id, first, last)                                               \
        __avlEntry_distance_##id(first, last)


/**
 * @brief @c MapEntry* : The starting point for iterating over keys in 
 * increasing order.
 */
#define map_iterator_begin(id, this) __avl_successor_##id((this)->root)


/**
 * @brief @c MapEntry* : The ending point for iterating over keys in 
 * increasing order.
 */
#define map_iterator_end() NULL


/**
 * @brief @c MapEntry* : The starting point for iterating over keys in 
 * decreasing order.
 */
#define map_iterator_rbegin(id, this) __avl_predecessor_##id((this)->root)


/**
 * @brief @c MapEntry* : The ending point for iterating over keys in 
 * decreasing order.
 */
#define map_iterator_rend() NULL


/**
 * Iterates through the map in-order.
 *
 * @param  it  @c MapEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define map_iter(id, this, it)                                                           \
        for (it = map_iterator_begin(id, this); it;                                      \
             it = mapEntry_getNext(id, it))

/**
 * Iterates through the map in reverse order.
 *
 * @param  it  @c MapEntry* : Assigned to the current element. May be
 *              dereferenced with @c it->data .
 */
#define map_riter(id, this, it)                                                          \
        for (it = map_iterator_rbegin(id, this); it;                                     \
             it = mapEntry_getPrev(id, it))

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * @brief @c unsigned : The number of elements in the map.
 */
#define map_size(this) (this)->size


/**
 * @brief @c bool : Whether the map is empty.
 */
#define map_empty(this) !(this)->root

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty map.
 *
 * @return  @c Map* : Newly created map.
 */
#define map_new(id) __avltree_new_fromArray_##id(NULL, 0)


/**
 * Creates a new map using @c n key-value pairs in a built-in array @c arr .
 *
 * @param   arr  @c Pair* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c Map* : Newly created map.
 */
#define map_new_fromArray(id, arr, n) __avltree_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of @c other .
 *
 * @param   other  @c Map* : Map to copy.
 *
 * @return         @c Map* : Newly created map.
 */
#define map_createCopy(id, other) __avltree_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 */
#define map_free(id, this) do { map_clear(id, this); free(this); } while(0)


/**
 * Removes all keys from the map, leaving it with a size of 0.
 */
#define map_clear(id, this)                                                              \
        __avltree_erase_##id(this, __avl_successor_##id((this)->root), NULL)


/**
 * Returns the entry with a key matching @c k .
 *
 * @param   k  @c kt : Key to find.
 *
 * @return     @c MapEntry* : Entry whose key matches @c k , or NULL if it was
 *             not found.
 */
#define map_find(id, this, k) __avltree_find_key_##id(this, k, 0)


/**
 * Similar to @c map_find , but returns a pointer to the pair's value rather 
 * than to the entry iterator as a whole.
 *
 * @param   k  @c kt : Key to find.
 *
 * @return     @c vt* : Pointer to the value whose key matches @c k , or NULL
 *             if it was not found.
 */
#define map_at(id, this, k) map_at_##id(this, k)


/**
 * Inserts @c pair into the map. If the key already exists, the value is 
 * updated to that of @c pair .
 *
 * @param   pair  @c Pair : Key-value pair to insert.
 *
 * @return        @c MapEntry* : Entry corresponding to the inserted pair.
 */
#define map_insert(id, this, pair) __avltree_insert_##id(this, pair, NULL)


/**
 * Inserts @c pair into the map, and updates @c inserted with the result of 
 * insertion. If the key already exists, the value is updated to that of 
 * @c pair .
 *
 * @param   pair      @c Pair : Key-value pair to insert.
 * @param   inserted  @c int* : Set to 1 if the pair was newly inserted, or 0
 *                     if the key already existed.
 *
 * @return            @c MapEntry* : Entry corresponding to the inserted pair.
 */
#define map_insert_withResult(id, this, pair, inserted)                                  \
        __avltree_insert_##id(this, pair, inserted)


/**
 * Inserts @c n key-value pairs from a built-in array @c arr .
 *
 * @param   arr  @c Pair* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c bool : Whether the operation succeeded.
 */
#define map_insert_fromArray(id, this, arr, n)                                           \
        __avltree_insert_fromArray_##id(this, arr, n)


/**
 * Inserts elements from another map in the range [ @c start , @c end ).
 *
 * @param   start  @c MapEntry* : First entry to insert. Must not be NULL.
 * @param   end    @c MapEntry* : Entry after the last entry to insert. If this
 *                  is NULL, all keys from @c start through the greatest key in
 *                  the other map will be inserted.
 *
 * @return         @c bool : Whether the operation succeeded.
 */
#define map_insert_fromMap(id, this, start, end)                                         \
        __avltree_insert_fromTree_##id(this, start, end)


/**
 * Removes key-value pairs in the range [ @c begin , @c end ).
 *
 * @param  begin  @c MapEntry* : First entry to erase.
 * @param  end    @c MapEntry* : Entry after the last entry to be deleted. If
 *                 this is NULL, all keys from @c begin through the greatest
 *                 key in the map will be removed.
 */
#define map_erase(id, this, begin, end) __avltree_erase_##id(this, begin, end)


/**
 * Removes a single pair from the map whose key is equal to @c key .
 *
 * @param  key  @c kt : Key to be deleted.
 */
#define map_remove_key(id, this, key)                                                    \
        __avltree_remove_entry_##id(this,                                                \
            __avltree_find_key_##id(this, key, 0))


/**
 * Removes @c entry from the map.
 *
 * @param  entry  @c MapEntry* : Entry to remove.
 */
#define map_remove_entry(id, this, entry)                                                \
        __avltree_remove_entry_##id(this, entry)


/**
 * Generates @c Map function declarations for the given key type and value 
 * type.
 *
 * @param  id  ID to be used for the @c Map , @c MapEntry , and @c Pair types
 *              (must be unique).
 * @param  kt  Key type.
 * @param  vt  Value type.
 */
#define gen_map_headers(id, kt, vt)                                                      \
                                                                                         \
typedef struct {                                                                         \
    kt first;                                                                            \
    vt second;                                                                           \
} Pair_##id;                                                                             \
                                                                                         \
__setup_avltree_headers(id, kt, Map_##id, Pair_##id, MapEntry_##id)                      \
                                                                                         \
vt* map_at_##id(Map_##id const *this, kt const key) __attribute__((nonnull));            \


/**
 * Generates @c Map function definitions for the given key type and value type.
 *
 * @param  id           ID used in @c gen_map_headers .
 * @param  kt           Key type used in @c gen_map_headers .
 * @param  vt           Value type used in @c gen_map_headers .
 * @param  cmp_lt       Macro of the form @c (x,y) that returns whether @c x is
 *                       strictly less than @c y .
 * @param  copyKey      Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the pair's key in the map.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the key is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteKey    Macro of the form @c (x) which is a complement to
 *                       @c copyKey ; if memory was dynamically allocated in
 *                       @c copyKey , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyKey , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyKey , pass @c DSDefault_deepDelete here.
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the pair's value in the map.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form @c (x) which is a complement to
 *                       @c copyValue ; if memory was dynamically allocated in
 *                       @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_map_source(id, kt, vt, cmp_lt, copyKey, deleteKey, copyValue,                \
                       deleteValue)                                                      \
                                                                                         \
__setup_avltree_source(id, kt, Map_##id, Pair_##id, MapEntry_##id, cmp_lt,               \
    __map_entry_get_key, __map_data_get_key, copyKey, deleteKey, copyValue,              \
    deleteValue)                                                                         \
                                                                                         \
vt* map_at_##id(Map_##id const *this, kt const key) {                                    \
    MapEntry_##id *e = __avltree_find_key_##id(this, key, 0);                            \
    return e ? &(e->data.second) : NULL;                                                 \
}                                                                                        \

#endif /* DS_MAP_H */
