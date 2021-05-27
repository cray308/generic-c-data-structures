#ifndef SET_H
#define SET_H

#include "avltree.h"
#include "alg_helper.h"

#define __set_entry_get_key(e) ((e)->data)
#define __set_data_get_key(d)  (d)
#define __set_copy_value(x, y)
#define __set_delete_value(x)

/**
 * The number of elements in the set.
 */
#define set_size(s) ((int) (s)->size)


/**
 * Tests whether the set is empty.
 */
#define set_empty(s) (!((s)->root))


/**
 * Iterates through the set in-order.
 *
 * @param  it  `SetEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define set_iter(id, s, it) for (it = iter_begin(AVLTREE, id, s, 0); it != iter_end(AVLTREE, id, s, 0); iter_next(AVLTREE, id, it))

/**
 * Iterates through the set in reverse order.
 *
 * @param  it  `SetEntry` which is assigned to the current element. May be dereferenced with it->data.
 */
#define set_riter(id, s, it) for (it = iter_rbegin(AVLTREE, id, s, 0); it != iter_rend(AVLTREE, id, s, 0); iter_prev(AVLTREE, id, it))


/**
 * Creates a new, empty set.
 *
 * @return  Pointer to the newly created set.
 */
#define set_new(id) __ds_calloc(1, sizeof(Set_##id))


/**
 * Creates a new set using `n` elements from a built-in array `arr`.
 *
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define set_new_fromArray(id, arr, n) __avltree_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of `other`.
 *
 * @param   other  `Set` to copy.
 *
 * @return         Pointer to the newly created set.
 */
#define set_createCopy(id, other) __avltree_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 */
#define set_free(id, s) __avltree_free_##id(s)


/**
 * Removes all elements from the set, leaving it with a size of 0.
 */
#define set_clear(id, s) __avltree_clear_##id(s)


/**
 * Tests whether `value` is in the set.
 *
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define set_contains(id, s, value) (__avltree_find_key_##id(s, value, false) != NULL)


/**
 * Returns the entry with a value of `value`.
 *
 * @param   value  Value to be found.
 *
 * @return         `SetEntry` that was found, or NULL if it was not found.
 */
#define set_find(id, s, value) __avltree_find_key_##id(s, value, false)


/**
 * Inserts `value` into the set.
 *
 * @param   value  Value to insert.
 *
 * @return         `SetEntry` corresponding to the inserted value.
 */
#define set_insert(id, s, value) __avltree_insert_##id(s, value, NULL)


/**
 * Inserts `value` into the set, and updates `inserted` with the result of insertion.
 *
 * @param   value     Value to insert.
 * @param   inserted  Set to 1 if the value was newly inserted, or 0 if the value was already a member.
 *
 * @return            `SetEntry` corresponding to the inserted value.
 */
#define set_insert_withResult(id, s, value, inserted) __avltree_insert_##id(s, value, inserted)


/**
 * Inserts `n` elements from a built-in array `arr`.
 *
 * @param  arr  Pointer to the first element to insert.
 * @param  n    Number of elements to include.
 */
#define set_insert_fromArray(id, s, arr, n) __avltree_insert_fromArray_##id(s, arr, n)


/**
 * Inserts elements from another set in the range [`start`, `end`).
 *
 * @param  start  First `SetEntry` to insert. Must not be NULL.
 * @param  end    `SetEntry` after the last entry to insert. If this is NULL, all elements from
 *                  `start` through the end (greatest element) of the other set will be inserted.
 */
#define set_insert_fromSet(id, s, start, end) __avltree_insert_fromTree_##id(s, start, end)


/**
 * Erases elements in the range [`begin`, `end`).
 *
 * @param  begin  First `SetEntry` to erase.
 * @param  end    `SetEntry` after the last entry to be deleted. If this is NULL, all elements from
 *                  `start` through the greatest element in the set will be removed.
 */
#define set_erase(id, s, begin, end) __avltree_erase_##id(s, begin, end)


/**
 * Removes a single item whose value is equal to `value`, if it exists.
 *
 * @param  value  Value to be deleted.
 */
#define set_remove_value(id, s, value) __avltree_remove_key_##id(s, value)


/**
 * Removes `entry` from the set.
 *
 * @param  entry  `SetEntry` to remove.
 */
#define set_remove_entry(id, s, entry) __avltree_remove_entry_##id(s, entry)


/**
 * Returns a `Set` with the union of this set and `other` (i.e. elements that are in this set, `other`, or both - all elements).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set, or NULL if `other` is NULL.
 */
#define set_union(id, s, other) __set_union_set_##id(iter_begin_AVLTREE(id, s, 0), NULL, iter_begin_AVLTREE(id, other, 0), NULL)


/**
 * Returns a `Set` with the intersection of this set and `other` (i.e. all elements that both sets have in common).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set, or NULL if `other` is NULL.
 */
#define set_intersection(id, s, other) __set_intersection_set_##id(iter_begin_AVLTREE(id, s, 0), NULL, iter_begin_AVLTREE(id, other, 0), NULL)


/**
 * Returns a `Set` with the difference of this set and `other` (i.e. all elements that are unique to this set).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set, or NULL if `other` is NULL.
 */
#define set_difference(id, s, other) __set_difference_set_##id(iter_begin_AVLTREE(id, s, 0), NULL, iter_begin_AVLTREE(id, other, 0), NULL)


/**
 * Returns a `Set` with the symmetric difference of this set and `other` (i.e. all elements that neither set has in common).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set, or NULL if `other` is NULL.
 */
#define set_symmetric_difference(id, s, other) __set_symmetric_difference_set_##id(iter_begin_AVLTREE(id, s, 0), NULL, iter_begin_AVLTREE(id, other, 0), NULL)


/**
 * Tests whether this set is a subset of `other` (i.e. whether each element in this subset is in `other`).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         True if each element in this set is in `other`, false if not.
 */
#define set_issubset(id, s, other) __includes_set_##id(iter_begin_AVLTREE(id, other, 0), NULL, iter_begin_AVLTREE(id, s, 0), NULL)


/**
 * Tests whether this set is a superset of `other` (i.e. whether this set contains each element from 
 * `other` - the inverse of a subset).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         True if this set contains each element in `other`, false if not.
 */
#define set_issuperset(id, s, other) set_issubset(id, other, s)


/**
 * Tests whether this set is disjoint with `other` (i.e. if the sets have no elements in common).
 *
 * @param   other  Pointer to the other set.
 *
 * @return         True if this set and `other` have no common elements, false if they do.
 */
#define set_isdisjoint(id, s, other) set_disjoint_##id(s, other)


/**
 * Generates `Set` code for the given value type.
 *
 * @param  id           ID to be used for the `Set` and `SetEntry` types (must be unique).
 * @param  t            Type to be stored in the set.
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the set.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_set(id, t, cmp_lt, copyValue, deleteValue)                                                       \
__gen_avltree(id, t, cmp_lt, Set_##id, t, SetEntry_##id, __set_entry_get_key, __set_data_get_key, copyValue, deleteValue, __set_copy_value, __set_delete_value) \
__gen_alg_set_funcs(id, cmp_lt, Set_##id, set_##id, set_new, SetEntry_##id *, iter_next_AVLTREE, iter_deref_AVLTREE, set_insert, set_insert_fromSet(id, d_new, first1, last1), set_insert_fromSet(id, d_new, first2, last2)) \
                                                                                                             \
__DS_FUNC_PREFIX bool set_disjoint_##id(Set_##id *this, Set_##id *other) {                                   \
    SetEntry_##id *n1, *n2;                                                                                  \
    if (!other || !other->root) return false;                                                                \
                                                                                                             \
    n1 = __avl_successor_##id(this->root);                                                                   \
    n2 = __avl_successor_##id(other->root);                                                                  \
    while (n1 && n2) {                                                                                       \
        if (cmp_lt(n1->data, n2->data)) {                                                                    \
            n1 = __avl_inorder_successor_##id(n1);                                                           \
        } else if (cmp_lt(n2->data, n1->data)) {                                                             \
            n2 = __avl_inorder_successor_##id(n2);                                                           \
        } else {                                                                                             \
            return false;                                                                                    \
        }                                                                                                    \
    }                                                                                                        \
    return true;                                                                                             \
}                                                                                                            \

#endif
