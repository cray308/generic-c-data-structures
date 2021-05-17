#ifndef SET_H
#define SET_H

#include "rbtree.h"
#include "alg_helper.h"

#define __set_entry_get_key(e) ((e)->data)
#define __set_data_get_key(d)  (d)
#define __set_copy_value(x, y)
#define __set_delete_value(x)

/**
 * The number of elements in the set.
 * 
 * @param   s  Pointer to set.
 */
#define set_size(s) ((int) (s)->size)


/**
 * Tests whether the set is empty.
 * 
 * @param   s  Pointer to set.
 */
#define set_empty(s) (!((s)->root))


/**
 * Iterates through the set in-order.
 *
 * @param   id    ID used with gen_set.
 * @param   s     Pointer to set.
 * @param   ptr   SetEntry which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(SET, ptr) or ptr->data.
 */
#define set_iter(id, s, it) for (it = iter_begin(TREE, id, s, 0); it != iter_end(TREE, id, s, 0); iter_next(TREE, id, it))

/**
 * Iterates through the set in reverse order.
 *
 * @param   id    ID used with gen_set.
 * @param   s     Pointer to set.
 * @param   ptr   SetEntry which is assigned to the current element.
 *                May be dereferenced with iter_deref(SET, ptr) or ptr->data.
 */
#define set_riter(id, s, it) for (it = iter_rbegin(TREE, id, s, 0); it != iter_rend(TREE, id, s, 0); iter_prev(TREE, id, it))


/**
 * Creates a new, empty set.
 *
 * @param   id  ID used with gen_set.
 *
 * @return      Pointer to the newly created set.
 */
#define set_new(id) __ds_calloc(1, sizeof(Set_##id))


/**
 * Creates a new set using the elements in a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define set_new_fromArray(id, arr, n) __rbtree_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of an existing Set.
 *
 * @param   id   ID used with gen_set.
 * @param   set  Set to copy.
 *
 * @return       Pointer to the newly created set.
 */
#define set_createCopy(id, other) __rbtree_createCopy_##id(other)


/**
 * Deletes all elements and frees the set.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define set_free(id, s) __rbtree_free_##id(s)


/**
 * Removes all elements from the set, leaving it with a size of 0.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define set_clear(id, s) __rbtree_clear_##id(s)


/**
 * Tests whether `value` is in the set.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define set_in(id, s, value) (__rbtree_find_key_##id(s, value, false) != NULL)


/**
 * Returns the SetEntry with a key matching `value`.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be found.
 *
 * @return         Pointer to SetEntry whose data matches `value`, or NULL if it was not found.
 */
#define set_find(id, s, value) __rbtree_find_key_##id(s, value, false)


/**
 * Inserts the value into the set.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to insert.
 */
#define set_insert(id, s, value) __rbtree_insert_##id(s, value, NULL)


/**
 * [set_insert_withResult description]
 *
 * @param   id        [description]
 * @param   s         [description]
 * @param   value     [description]
 * @param   inserted  [description]
 *
 * @return            [description]
 */
#define set_insert_withResult(id, s, value, inserted) __rbtree_insert_##id(s, value, inserted)


/**
 * Inserts elements from a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   s    Pointer to set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to insert from `arr`.
 */
#define set_insert_fromArray(id, s, arr, n) __rbtree_insert_fromArray_##id(s, arr, n)


/**
 * Inserts elements from another Set in the range [start, end).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   start  Pointer to first SetEntry to insert. Must not be NULL.
 * @param   end    Pointer after the last SetEntry to insert. If this is NULL, all elements from
 *                   `start` through the end (greatest element) of the other set will be inserted.
 */
#define set_insert_fromSet(id, s, start, end) __rbtree_insert_fromTree_##id(s, start, end)


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
#define set_erase(id, s, begin, end) __rbtree_erase_##id(s, begin, end)


/**
 * Removes a single entry from the set whose value is equal to `value`.
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  value  Pointer to the value to be deleted.
 */
#define set_remove_value(id, s, value) __rbtree_remove_key_##id(s, value)


/**
 * [set_remove_entry description]
 *
 * @param   id  [description]
 * @param   m   [description]
 * @param   e   [description]
 *
 * @return      [description]
 */
#define set_remove_entry(id, s, e) __rbtree_remove_entry_##id(s, e)


/**
 * Returns a set with the union of `s` and `other` (i.e. elements that are in `s`, `other`,
 * or both - all elements).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of `s` and `other`, or NULL if `other`
 *                   is NULL.
 */
#define set_union(id, s, other) __set_union_set_##id(iter_begin_TREE(id, s, 0), NULL, iter_begin_TREE(id, other, 0), NULL)


/**
 * Returns a set with the intersection of `s` and `other` (i.e. all elements that both sets
 * have in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of `s` and `other`, or NULL if `other`
 *                   is NULL.
 */
#define set_intersection(id, s, other) __set_intersection_set_##id(iter_begin_TREE(id, s, 0), NULL, iter_begin_TREE(id, other, 0), NULL)


/**
 * Returns a set with the difference of `s` and `other` (i.e. all elements that are unique to `s`).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of `s` and `other`, or NULL if `other`
 *                   is NULL.
 */
#define set_difference(id, s, other) __set_difference_set_##id(iter_begin_TREE(id, s, 0), NULL, iter_begin_TREE(id, other, 0), NULL)


/**
 * Returns a set with the symmetric difference of `s` and `other` (i.e. all elements that
 * neither set has in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the symmetric difference of `s` and `other`, or
 *                   NULL if `other` is NULL.
 */
#define set_symmetric_difference(id, s, other) __set_symmetric_difference_set_##id(iter_begin_TREE(id, s, 0), NULL, iter_begin_TREE(id, other, 0), NULL)


/**
 * Tests whether `s` is a subset of `other`. (i.e. whether each element in `s` is in other).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if each element in `s` is in `other`, false if not.
 */
#define set_issubset(id, s, other) __includes_set_##id(iter_begin_TREE(id, other, 0), NULL, iter_begin_TREE(id, s, 0), NULL)


/**
 * Tests whether `s` is a superset of `other`. (i.e. whether `s` contains each element
 * from `other` - the opposite of a subset).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if `s` contains each element in `other`, false if not.
 */
#define set_issuperset(id, s, other) set_issubset(id, other, s)


/**
 * Tests whether `s` is disjoint with `other`. (i.e. if the sets have no elements in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if `s` and `other` have no common elements, false if they do.
 */
#define set_isdisjoint(id, s, other) set_disjoint_##id(s, other)


/**
 * [gen_set description]
 *
 * @param   id         [description]
 * @param   t          [description]
 * @param   cmp_lt     [description]
 * @param   copyKey    [description]
 * @param   deleteKey  [description]
 *
 * @return             [description]
 */
#define gen_set(id, t, cmp_lt, copyKey, deleteKey)                                                                               \
__gen_rbtree(id, t, cmp_lt, Set_##id, t, SetEntry_##id, __set_entry_get_key, __set_data_get_key, copyKey, deleteKey, __set_copy_value, __set_delete_value) \
__gen_alg_set_funcs(id, cmp_lt, Set_##id, set_##id, set_new, SetEntry_##id *, iter_next_TREE, iter_deref_TREE, set_insert, set_insert_fromSet(id, d_new, first1, last1), set_insert_fromSet(id, d_new, first2, last2)) \
                                                                                                             \
__DS_FUNC_PREFIX bool set_disjoint_##id(Set_##id *this, Set_##id *other) {                                   \
    if (!other || !other->root) return false;                                                                \
                                                                                                             \
    SetEntry_##id *n1 = __rb_successor_##id(this->root);                                                    \
    SetEntry_##id *n2 = __rb_successor_##id(other->root);                                                   \
    while (n1 && n2) {                                                                                       \
        if (cmp_lt(n1->data, n2->data)) {                                                                    \
            n1 = __rb_inorder_successor_##id(n1);                                                            \
        } else if (cmp_lt(n2->data, n1->data)) {                                                             \
            n2 = __rb_inorder_successor_##id(n2);                                                            \
        } else {                                                                                             \
            return false;                                                                                    \
        }                                                                                                    \
    }                                                                                                        \
    return true;                                                                                             \
}                                                                                                            \

#endif
