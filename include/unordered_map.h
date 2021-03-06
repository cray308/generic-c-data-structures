#ifndef DS_UNORDERED_MAP_H
#define DS_UNORDERED_MAP_H

#include "hash_table.h"

#define __umap_entry_get_key(e) (e)->data.first
#define __umap_data_get_key(d)  (d).first

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * Iterates through all entries in the map.
 *
 * @param  it  @c Pair* : Assigned to the current element.
 */
#define umap_iter(id, this, it)                                                          \
        for (it = __htable_iter_begin_##id(this); it;                                    \
             it = __htable_iter_next_##id(this))

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * @brief @c unsigned : The number of entries in the map.
 */
#define umap_size(this) (this)->size


/**
 * @brief @c unsigned : The current value of the map's max load factor.
 */
#define umap_max_load_factor(this) (this)->lf


/**
 * @brief @c unsigned : The total number of buckets in the map.
 */
#define umap_bucket_count(this) (this)->cap


/**
 * @brief @c bool : Whether the map is empty.
 */
#define umap_empty(this) !(this)->size

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty map.
 *
 * @return  @c UMap* : Newly created map.
 */
#define umap_new(id) __htable_new_fromArray_##id(NULL, 0)


/**
 * Creates a new map using @c n key-value pairs in a built-in array @c arr .
 *
 * @param   arr  @c Pair* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c UMap* : Newly created map.
 */
#define umap_new_fromArray(id, arr, n) __htable_new_fromArray_##id(arr, n)


/**
 * Creates a new map as a copy of @c other .
 *
 * @param   other  @c UMap* : Map to copy.
 *
 * @return         @c UMap* : Newly created map.
 */
#define umap_createCopy(id, other) __htable_createCopy_##id(other)


/**
 * Deletes all elements and frees the map.
 */
#define umap_free(id, this) do {                                                         \
    __htable_clear_##id(this); free((this)->buckets); free(this);                        \
} while(0)


/**
 * Inserts @c pair into the map. If the key already exists, the value is 
 * updated to that of @c pair .
 *
 * @param  pair  @c Pair : Key-value pair to insert.
 *
 * @return       @c Pair* : Pointer to the inserted pair.
 */
#define umap_insert(id, this, pair) __htable_insert_##id(this, pair, NULL)


/**
 * Inserts @c pair into the map, and updates @c inserted with the result of 
 * insertion. If the key already exists, the value is updated to that of 
 * @c pair .
 *
 * @param   pair      @c Pair : Key-value pair to insert.
 * @param   inserted  @c int* : Set to 1 if a new pair was inserted, or 0 if
 *                     not.
 *
 * @return            @c Pair* : Pointer to the inserted pair.
 */
#define umap_insert_withResult(id, this, pair, inserted)                                 \
        __htable_insert_##id(this, pair, inserted)


/**
 * Inserts @c n key-value pairs from a built-in array @c arr .
 *
 * @param   arr  @c Pair* : Pointer to the first element to insert.
 * @param   n    @c unsigned : Number of elements to include.
 *
 * @return       @c bool : Whether the operation succeeded.
 */
#define umap_insert_fromArray(id, this, arr, n)                                          \
        __htable_insert_fromArray_##id(this, arr, n)


/**
 * Finds the entry with a key matching @c k .
 *
 * @param  k   @c kt : Key to find.
 *
 * @return     @c Pair* : Pointer to pair whose key matches @c k , or NULL if
 *             it was not found.
 */
#define umap_find(id, this, k) __htable_find_##id(this, k)


/**
 * Similar to @c umap_find , but returns a pointer to the pair's value rather 
 * than to the pair as a whole.
 *
 * @param   k  @c kt : Key to find.
 *
 * @return     @c vt* : Pointer to the value whose key matches @c k , or NULL
 *             if it was not found.
 */
#define umap_at(id, this, k) umap_at_##id(this, k)


/**
 * Removes a single pair from the map whose key is equal to @c k .
 *
 * @param  k  @c kt : Key to be deleted.
 *
 * @return    @c bool : Whether an entry was found and deleted.
 */
#define umap_remove_key(id, this, k) __htable_erase_##id(this, k)


/**
 * Changes the number of buckets in the map to @c nbuckets . If this is less 
 * than or equal to the current number of buckets, nothing is done.
 *
 * @param   nbuckets  @c unsigned : New number of buckets to use in the map.
 *
 * @return            @c bool : Whether the operation succeeded.
 */
#define umap_rehash(id, this, nbuckets) __htable_rehash_##id(this, nbuckets)


/**
 * If it is reasonable, sets the maximum load factor to @c lf , and may rehash 
 * the map if required.
 *
 * @param   lf  @c unsigned : The new load factor to use.
 *
 * @return      @c bool : Whether the operation succeeded.
 */
#define umap_set_load_factor(id, this, lf)                                               \
        __htable_set_load_factor_##id(this, lf)


/**
 * Removes all entries from the map.
 */
#define umap_clear(id, this) __htable_clear_##id(this)


/**
 * Generates @c UMap function declarations for the given key type and value 
 * type.
 *
 * @param  id  ID to be used for the @c UMap and @c Pair types (must be
 *              unique).
 * @param  kt  Key type.
 * @param  vt  Value type.
 */
#define gen_umap_headers(id, kt, vt)                                                     \
                                                                                         \
typedef struct {                                                                         \
    kt first;                                                                            \
    vt second;                                                                           \
} Pair_##id;                                                                             \
                                                                                         \
__setup_hash_table_headers(id, kt, UMap_##id, Pair_##id, UMapEntry_##id)                 \
                                                                                         \
vt* umap_at_##id(UMap_##id const *this, kt const key) __attribute__((nonnull));          \


/**
 * Generates @c UMap function definitions for the given key type and value type.
 *
 * @param  id           ID used in @c gen_umap_headers .
 * @param  kt           Key type used in @c gen_umap_headers .
 * @param  vt           Value type used in @c gen_umap_headers .
 * @param  cmp_eq       Macro of the form @c (x,y) that returns whether @c x is
 *                       equal to @c y .
 * @param  addrOfKey    Macro of the form @c (x) that returns a pointer to
 *                       @c x .
 *                        - For value types (i.e. int) pass
 *                         @c DSDefault_addrOfVal .
 *                        - For pointer types, pass @c DSDefault_addrOfRef .
 * @param  sizeOfKey    Macro of the form (x) that returns the number of bytes
 *                       in @c x , a key for the map. This is necessary so that
 *                       the hashing function works correctly.
 *                        - For value types (i.e. int), pass
 *                         @c DSDefault_sizeOfVal .
 *                        - For a string (char*), pass @c DSDefault_sizeOfStr .
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
 *                        @c copyKey , pass @c DSDefault_shallowDelete here.
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
 *                        - If @c DSDefault_deepCopyStr  was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_umap_source(id, kt, vt, cmp_eq, addrOfKey, sizeOfKey,                        \
                        copyKey, deleteKey, copyValue, deleteValue)                      \
                                                                                         \
__setup_hash_table_source(id, kt, cmp_eq, UMap_##id, Pair_##id, UMapEntry_##id,          \
    __umap_entry_get_key, __umap_data_get_key, addrOfKey, sizeOfKey, copyKey,            \
    deleteKey, copyValue, deleteValue)                                                   \
                                                                                         \
vt* umap_at_##id(UMap_##id const *this, kt const key) {                                  \
    Pair_##id *p = __htable_find_##id(this, key);                                        \
    return p ? &(p->second) : NULL;                                                      \
}                                                                                        \

#endif /* DS_UNORDERED_MAP_H */
