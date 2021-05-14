#ifndef SET_H
#define SET_H

#include "rbtree.h"
#include "alg_helper.h"

/**
 * The number of elements in the set.
 * 
 * @param   s  Pointer to set.
 */
#define set_len(s) ((int) (s)->size)


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
#define set_iter(id, s, ptr) tree_iter(id, s, ptr)

/**
 * Iterates through the set in reverse order.
 *
 * @param   id    ID used with gen_set.
 * @param   s     Pointer to set.
 * @param   ptr   SetEntry which is assigned to the current element.
 *                May be dereferenced with iter_deref(SET, ptr) or ptr->data.
 */
#define set_riter(id, s, ptr) tree_riter(id, s, ptr)


/**
 * Creates a new, empty set.
 *
 * @param   id  ID used with gen_set.
 *
 * @return      Pointer to the newly created set.
 */
#define set_new(id) tree_new(id)


/**
 * Creates a new set using the elements in a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to include.
 *
 * @return       Pointer to the newly created set.
 */
#define set_new_fromArray(id, arr, n) set_new_fromArray_##id(arr, n)


/**
 * Creates a new set as a copy of an existing Set.
 *
 * @param   id   ID used with gen_set.
 * @param   set  Set to copy.
 *
 * @return       Pointer to the newly created set.
 */
#define set_createCopy(id, set) set_createCopy_##id(set)


/**
 * Deletes all elements and frees the set.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define set_free(id, s) tree_free_##id(s)


/**
 * Removes all elements from the set, leaving it with a size of 0.
 *
 * @param  id  ID used with gen_set.
 * @param  s   Pointer to set.
 */
#define set_clear(id, s) tree_clear_##id(s)


/**
 * Tests whether `value` is in the set.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define set_in(id, s, value) (tree_find(id, s, value) != NULL)


/**
 * Returns the SetEntry with a key matching `value`.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be found.
 *
 * @return         Pointer to SetEntry whose data matches `value`, or NULL if it was not found.
 */
#define set_find(id, s, value) tree_find(id, s, value)


/**
 * Inserts the value into the set.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to insert.
 */
#define set_insert(id, s, value) tree_insert_##id(s, value)


/**
 * Inserts elements from a built-in array data type.
 *
 * @param   id   ID used with gen_set.
 * @param   s    Pointer to set.
 * @param   arr  Pointer to the first element to insert.
 * @param   n    Number of elements to insert from `arr`.
 */
#define set_insert_fromArray(id, s, arr, n) set_insert_fromArray_##id(s, arr, n)


/**
 * Inserts elements from another Set in the range [start, end).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   start  Pointer to first SetEntry to insert. Must not be NULL.
 * @param   end    Pointer after the last SetEntry to insert. If this is NULL, all elements from
 *                   `start` through the end (greatest element) of the other set will be inserted.
 */
#define set_insert_fromSet(id, s, start, end) set_insert_fromSet_##id(s, start, end)


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
#define set_erase(id, s, begin, end) set_erase_##id(s, begin, end)


/**
 * Removes a single entry from the set whose value is equal to `value`.
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  value  Pointer to the value to be deleted.
 */
#define set_remove_value(id, s, value) tree_delete_by_val_##id(s, value)


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
#define set_union(id, s, other) __set_union_set_##id(iter_begin_SET(id, s, 0), NULL, iter_begin_SET(id, other, 0), NULL)


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
#define set_intersection(id, s, other) __set_intersection_set_##id(iter_begin_SET(id, s, 0), NULL, iter_begin_SET(id, other, 0), NULL)


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
#define set_difference(id, s, other) __set_difference_set_##id(iter_begin_SET(id, s, 0), NULL, iter_begin_SET(id, other, 0), NULL)


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
#define set_symmetric_difference(id, s, other) __set_symmetric_difference_set_##id(iter_begin_SET(id, s, 0), NULL, iter_begin_SET(id, other, 0), NULL)


/**
 * Tests whether `s` is a subset of `other`. (i.e. whether each element in `s` is in other).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if each element in `s` is in `other`, false if not.
 */
#define set_issubset(id, s, other) __includes_set_##id(iter_begin_SET(id, other, 0), NULL, iter_begin_SET(id, s, 0), NULL)


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

/* --------------------------------------------------------------------------
 * Set iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_SET(id, t, n)   iter_begin_TREE(id, t, n)
#define iter_end_SET(id, t, n)     iter_end_TREE(id, t, n)
#define iter_rbegin_SET(id, t, n)  iter_rbegin_TREE(id, t, n)
#define iter_rend_SET(id, t, n)    iter_rend_TREE(id, t, n)
#define iter_next_SET(id, p)       iter_next_TREE(id, p)
#define iter_prev_SET(id, p)       iter_prev_TREE(id, p)
#define iter_deref_SET(p)          iter_deref_TREE(p)
#define iter_advance_SET(id, p, n) iter_advance_TREE(id, p, n)
#define iter_dist_SET(id, p1, p2)  iter_dist_TREE(id, p1, p2)

/**
 * Generates set code for a specified type and ID.
 *
 * @param   id      ID to be used for the type stored in the set (must be unique).
 * @param   t       Type to be stored in the set.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether (x < y).
 */
#define gen_set(id, t, cmp_lt)                                                                               \
gen_rbtree(id, t, cmp_lt)                                                                                    \
                                                                                                             \
typedef Tree_##id Set_##id;                                                                                  \
typedef TreeEntry_##id SetEntry_##id;                                                                        \
__DS_FUNC_PREFIX void set_insert_fromSet_##id(Set_##id *this, SetEntry_##id *start, SetEntry_##id *end);     \
__DS_FUNC_PREFIX void set_insert_fromArray_##id(Set_##id *this, t *arr, size_t n);                           \
                                                                                                             \
__DS_FUNC_PREFIX Set_##id *set_new_fromArray_##id(t *arr, size_t size) {                                     \
    Set_##id *s = set_new(id);                                                                               \
    set_insert_fromArray_##id(s, arr, size);                                                                 \
    return s;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Set_##id *set_createCopy_##id(Set_##id *set) {                                              \
    Set_##id *s = set_new(id);                                                                               \
    set_insert_fromSet_##id(s, __rb_successor_##id(set->root), NULL);                                        \
    return s;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void set_insert_fromArray_##id(Set_##id *this, t *arr, size_t n) {                          \
    if (!arr || !n) return;                                                                                  \
    t *end = &arr[n];                                                                                        \
    for (; arr != end; ++arr) {                                                                              \
        tree_insert_##id(this, *arr);                                                                        \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void set_insert_fromSet_##id(Set_##id *this, SetEntry_##id *start, SetEntry_##id *end) {    \
    while (start != end) {                                                                                   \
        tree_insert_##id(this, start->data);                                                                 \
        start = __rb_inorder_successor_##id(start);                                                          \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void set_erase_##id(Set_##id *this, SetEntry_##id *begin, SetEntry_##id *end) {             \
    if (!begin) {                                                                                            \
        begin = __rb_successor_##id(this->root);                                                             \
    }                                                                                                        \
                                                                                                             \
    /* store values in an array since RB tree deletions involve swapping values
     * and thus it's not reliable to use RBNode pointers in a bulk delete operation */                       \
    t vals[this->size];                                                                                      \
    int count = 0;                                                                                           \
    t *c = vals;                                                                                             \
    TreeEntry_##id *curr = begin;                                                                            \
    while (curr != end) {                                                                                    \
        *c = curr->data;                                                                                     \
        ++c, ++count;                                                                                        \
        curr = __rb_inorder_successor_##id(curr);                                                            \
    }                                                                                                        \
                                                                                                             \
    for (int i = 0; i < count; ++i) {                                                                        \
        tree_delete_by_val_##id(this, vals[i]);                                                              \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX bool set_disjoint_##id(Set_##id *this, Set_##id *other) {                                   \
    if (!other || !other->root) return false;                                                                \
                                                                                                             \
    TreeEntry_##id *n1 = __rb_successor_##id(this->root);                                                    \
    TreeEntry_##id *n2 = __rb_successor_##id(other->root);                                                   \
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
                                                                                                             \
__gen_alg_set_funcs(id, cmp_lt, Set_##id, set_##id, tree_new, TreeEntry_##id *, iter_next_TREE, iter_deref_TREE, tree_insert, set_insert_fromSet_##id(d_new, first1, last1), set_insert_fromSet_##id(d_new, first2, last2)) \

#endif
