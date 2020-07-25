#ifndef SET_H
#define SET_H

#include "rbtree.h"
#include "alg_helper.h"

#define SET_END(id) (SetEntry_##id *)(0)

typedef enum {
    SET_INIT_EMPTY,
    SET_INIT_BUILTIN, /* int nums[] = {1, 2, 3}; set_new(..., &nums, 3); */
    SET_INIT_SET /* Set *nums = ...; set_new(..., nums) */
} SetInitializer;

typedef enum {
    SET_INSERT_SINGLE, /* int num = 5; set_insert(..., num); */
    SET_INSERT_BUILTIN, /* int nums[] = {1, 2, 3}; set_insert(..., &nums, 0, 3); */
    SET_INSERT_SET /* Set *nums = ...; set_insert(..., nums, begin, end) */
} SetInsertType;

#define set_begin(id, s) ((s)->root ? __rb_successor_##id((s)->root) : NULL)
#define __init_set(id) set_new_##id(SET_INIT_EMPTY)
#define __iter_next_set(id, p) (p = __rb_inorder_successor_##id(p))
#define __deref_set(p) (p->data)
#define __insert_single_set tree_insert
#define __insert_multi_1_set(id) __set_insert_set_##id(d_new, first1, last1)
#define __insert_multi_2_set(id) __set_insert_set_##id(d_new, first2, last2)

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
 * @param   eptr  Pointer which is assigned to the current set element's data.
 */
#define set_iter(id, s, eptr) tree_inorder(id, s, eptr)


/**
 * Creates a new set.
 * In (1), an empty Set is created.
 * In (2), a Set is initialized from a built-in array data type, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total.
 * In (3), a Set is initialized from another existing Set.
 * 
 * (1) init = SET_INIT_EMPTY:    set_new(id, SetInitializer init)
 * (2) init = SET_INIT_BUILTIN:  set_new(id, SetInitializer init, t arr[], int n)
 * (3) init = SET_INIT_SET:      set_new(id, SetInitializer init, Set_id *other)
 *
 * @param   id      ID used with gen_set.
 * @param   init    Type of initializer to execute.
 *
 * @return          Pointer to the newly created set. If an error occurred, returns NULL.
 */
#define set_new(id, init, ...) set_new_##id(init, ##__VA_ARGS__)


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
 * Tests whether "value" is in the set.
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be checked for membership.
 *
 * @return         True if the value was found, false if not.
 */
#define set_in(id, s, value) (tree_find(id, s, value) != NULL)


/**
 * Returns the SetEntry with a key matching "value".
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   value  Value to be found.
 *
 * @return         Pointer to SetEntry whose data matches "value", or NULL if it was not found.
 */
#define set_find(id, s, value) tree_find(id, s, value)


/**
 * Inserts elements into the set.
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, where "arr" is a pointer to the
 *   first element to insert, inserting "n" elements total.
 * In (3), elements from another Set are inserted in the range [start, end). "start" must not be
 *   NULL. If "end" is SET_END, then all elements from "start" through the end (greatest element)
 *   of the other set will be inserted.
 * 
 * (1) type = SET_INSERT_SINGLE:   set_insert(id, Set_id *s, SetInsertType type, t value)
 * (2) type = SET_INSERT_BUILTIN:  set_insert(id, Set_id *s, SetInsertType type, t arr[], int n)
 * (3) type = SET_INSERT_SET:      set_insert(id, Set_id *s, SetInsertType type, SetEntry_id *start, SetEntry_id *end)
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  type   Type of insertion to execute.
 */
#define set_insert(id, s, type, ...) set_insert_##id(s, type, ##__VA_ARGS__)


/**
 * Erases elements in the range [begin,end).
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  begin  First element to erase. If this is NULL, it defaults to the smallest element
 *                  in the set.
 * @param  end    SetEntry AFTER the last element to be deleted. If this is SET_END, then all
 *                  elements from start through the greatest element in the set will be removed.
 */
#define set_erase(id, s, begin, end) set_erase_##id(s, begin, end)


/**
 * Removes a single entry from the set whose value is equal to "value".
 *
 * @param  id     ID used with gen_set.
 * @param  s      Pointer to set.
 * @param  value  Pointer to the value to be deleted.
 */
#define set_remove_value(id, s, value) tree_delete_by_val_##id(s, value)


/**
 * Returns a set with the union of "s" and "other" (i.e. elements that are in "s", "other",
 * or both - all elements).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "s" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_union(id, s, other) __set_union_set_##id(set_begin(id, s), NULL, set_begin(id, other), NULL)


/**
 * Returns a set with the intersection of "s" and "other" (i.e. all elements that both sets
 * have in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "s" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_intersection(id, s, other) __set_intersection_set_##id(set_begin(id, s), NULL, set_begin(id, other), NULL)


/**
 * Returns a set with the difference of "s" and "other" (i.e. all elements that are unique to
 * "s").
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the union of "s" and "other", or NULL if "other"
 *                   is NULL.
 */
#define set_difference(id, s, other) __set_difference_set_##id(set_begin(id, s), NULL, set_begin(id, other), NULL)


/**
 * Returns a set with the symmetric difference of "s" and "other" (i.e. all elements that
 * neither set has in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         Newly created set which is the symmetric difference of "s" and "other", or
 *                   NULL if "other" is NULL.
 */
#define set_symmetric_difference(id, s, other) __set_symmetric_difference_set_##id(set_begin(id, s), NULL, set_begin(id, other), NULL)


/**
 * Tests whether "s" is a subset of "other". (i.e. whether each element in "s" is in
 * other).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if each element in "s" is in "other", false if not.
 */
#define set_issubset(id, s, other) __includes_set_##id(set_begin(id, other), NULL, set_begin(id, s), NULL)


/**
 * Tests whether "s" is a superset of "other". (i.e. whether "s" contains each element
 * from "other" - the opposite of a subset).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if "s" contains each element in "other", false if not.
 */
#define set_issuperset(id, s, other) set_issubset(id, other, s)


/**
 * Tests whether "s" is disjoint with "other". (i.e. if the sets have no elements in common).
 *
 * @param   id     ID used with gen_set.
 * @param   s      Pointer to set.
 * @param   other  Pointer to the other set.
 *
 * @return         True if "s" and "other" have no common elements, false if they do.
 */
#define set_isdisjoint(id, s, other) __set_disjoint_##id(s, other)


/**
 * Generates set code for a specified type and ID.
 *
 * @param   id      ID to be used for the type stored in the set (must be unique).
 * @param   t       Type to be stored in the set.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether (x < y).
 */
#define gen_set(id, t, cmp_lt)                                                                               \
gen_rbtree(id, t, cmp_lt)                                                                                    \
__gen_set_declarations(id, t)                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX Set_##id *set_new_##id(SetInitializer init, ...) {                                          \
    Set_##id *s = tree_new_##id();                                                                           \
    if (!s) {                                                                                                \
        return NULL;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    if (init == SET_INIT_EMPTY) { /* nothing more to do in this case */                                      \
        return s;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    int n;                                                                                                   \
    void *other;                                                                                             \
                                                                                                             \
    /* parse arguments */                                                                                    \
    va_list args;                                                                                            \
    va_start(args, init);                                                                                    \
                                                                                                             \
    other = va_arg(args, void *);                                                                            \
                                                                                                             \
    if (init == SET_INIT_BUILTIN) {                                                                          \
        n = va_arg(args, int);                                                                               \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
    if (!other) {                                                                                            \
        return s;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    if (init == SET_INIT_BUILTIN) {                                                                          \
        __set_insert_builtin_##id(s, (t *) other, n);                                                        \
    } else {                                                                                                 \
        __set_insert_set_##id(s, set_begin(id, ((Set_##id *) other)), NULL);                                 \
    }                                                                                                        \
    return s;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void set_insert_##id(Set_##id *this, SetInsertType type, ...) {                             \
    int n;                                                                                                   \
    t value;                                                                                                 \
    void *begin;                                                                                             \
    void *end;                                                                                               \
                                                                                                             \
    /* parse arguments */                                                                                    \
    va_list args;                                                                                            \
    va_start(args, type);                                                                                    \
                                                                                                             \
    if (type == SET_INSERT_SINGLE) {                                                                         \
        value = (t) (long) va_arg(args, void *);                                                             \
    } else if (type == SET_INSERT_BUILTIN) {                                                                 \
        begin = va_arg(args, void *);                                                                        \
        n = va_arg(args, int);                                                                               \
    } else {                                                                                                 \
        begin = va_arg(args, void *);                                                                        \
        end = va_arg(args, void *);                                                                          \
    }                                                                                                        \
                                                                                                             \
    va_end(args);                                                                                            \
                                                                                                             \
    switch (type) {                                                                                          \
        case SET_INSERT_SINGLE:                                                                              \
            tree_insert_##id(this, value);                                                                   \
            break;                                                                                           \
        case SET_INSERT_BUILTIN:                                                                             \
            __set_insert_builtin_##id(this, (t *) begin, n);                                                 \
            break;                                                                                           \
        case SET_INSERT_SET:                                                                                 \
            __set_insert_set_##id(this, (SetEntry_##id *) begin, (SetEntry_##id *) end);                     \
            break;                                                                                           \
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
    RBNode_##id *curr = begin;                                                                               \
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
__DS_FUNC_PREFIX bool __set_disjoint_##id(Set_##id *this, Set_##id *other) {                                 \
    if (!other || !other->root) {                                                                            \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    RBNode_##id *n1 = __rb_successor_##id(this->root);                                                       \
    RBNode_##id *n2 = __rb_successor_##id(other->root);                                                      \
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
__gen_set_helper_funcs(id, t)                                                                                \
__gen_alg_set_funcs(id, cmp_lt, Set_##id, set_##id, __init_set, RBNode_##id *, __iter_next_set, __deref_set, __insert_single_set, __insert_multi_1_set, __insert_multi_2_set) \

#define __gen_set_declarations(id, t)                                                                        \
typedef Tree_##id Set_##id;                                                                                  \
typedef RBNode_##id SetEntry_##id;                                                                           \
__DS_FUNC_PREFIX void __set_insert_set_##id(Set_##id *this, SetEntry_##id *start, SetEntry_##id *end);       \
__DS_FUNC_PREFIX void __set_insert_builtin_##id(Set_##id *this, t *arr, int n);                              \

#define __gen_set_helper_funcs(id, t)                                                                        \
__DS_FUNC_PREFIX void __set_insert_set_##id(Set_##id *this, SetEntry_##id *start, SetEntry_##id *end) {      \
    while (start != end) {                                                                                   \
        tree_insert_##id(this, start->data);                                                                 \
        start = __rb_inorder_successor_##id(start);                                                          \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __set_insert_builtin_##id(Set_##id *this, t *arr, int n) {                             \
    if (!arr || !n) {                                                                                        \
        return;                                                                                              \
    }                                                                                                        \
    t *end = &arr[n];                                                                                        \
    for (; arr != end; ++arr) {                                                                              \
        tree_insert_##id(this, *arr);                                                                        \
    }                                                                                                        \
}                                                                                                            \

#endif
