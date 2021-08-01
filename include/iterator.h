#ifndef ITERATOR_H
#define ITERATOR_H

#include "ds.h"

/* --------------------------------------------------------------------------
 * Types: ARR, LIST, AVLTREE, STR
 * -------------------------------------------------------------------------- */

#define iter_begin(type, id, ds, n) iter_begin_##type(id, ds, n)

#define iter_end(type, id, ds, n) iter_end_##type(id, ds, n)

#define iter_rbegin(type, id, ds, n) iter_rbegin_##type(id, ds, n)

#define iter_rend(type, id, ds, n) iter_rend_##type(id, ds, n)

#define iter_next(type, id, p) iter_next_##type(id, p)

#define iter_prev(type, id, p) iter_prev_##type(id, p)

#define iter_advance(type, id, p, n) iter_advance_##type(id, p, n)

#define iter_dist(type, id, p1, p2) iter_dist_##type(id, p1, p2)

/* --------------------------------------------------------------------------
 * AVL Tree iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_AVLTREE(id, t, n)    __avl_successor_##id((t)->root)
#define iter_end_AVLTREE(id, t, n)      NULL
#define iter_rbegin_AVLTREE(id, t, n)   __avl_predecessor_##id((t)->root)
#define iter_rend_AVLTREE(id, t, n)     NULL
#define iter_next_AVLTREE(id, p)        ((p) = __avl_inorder_successor_##id((p)))
#define iter_prev_AVLTREE(id, p)        ((p) = __avl_inorder_predecessor_##id((p)))
#define iter_advance_AVLTREE(id, p, n)  iterator_advance_helper(AVLTREE, id, p, n)
#define iter_dist_AVLTREE(id, p1, p2)   __iter_dist_helper_AVLTREE_##id(p1, p2)

/* --------------------------------------------------------------------------
 * Generic helpers
 * -------------------------------------------------------------------------- */

#define iterator_advance_helper(type, id, p, n)                                                              \
do {                                                                                                         \
    int increment = ((n) >= 0) ? 1 : -1;                                                                     \
    int count = 0;                                                                                           \
    if (increment > 0) {                                                                                     \
        while (count != (n) && (p) != NULL) {                                                                \
            iter_next(type, id, p);                                                                          \
            count += increment;                                                                              \
        }                                                                                                    \
    } else {                                                                                                 \
        while (count != (n) && (p) != NULL) {                                                                \
            iter_prev(type, id, p);                                                                          \
            count += increment;                                                                              \
        }                                                                                                    \
    }                                                                                                        \
} while (0) 

#define create_iterator_distance_helper(type, id, IterType)                                                  \
__DS_FUNC_PREFIX_INL size_t __iter_dist_helper_##type_##id(IterType p1, IterType p2) {                       \
    size_t dist = 0;                                                                                         \
    IterType temp = p1;                                                                                      \
    while (temp != p2) {                                                                                     \
        iter_next(type, id, temp);                                                                           \
        ++dist;                                                                                              \
    }                                                                                                        \
    return dist;                                                                                             \
}                                                                                                            \

#endif
