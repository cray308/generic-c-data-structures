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

#define iter_deref(type, p) iter_deref_##type(p)

#define iter_advance(type, id, p, n) iter_advance_##type(id, p, n)

#define iter_dist(type, id, p1, p2) iter_dist_##type(id, p1, p2)

/* --------------------------------------------------------------------------
 * Array (random access) iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_ARR(id, a, n)    ((n) ? &((a)[0]) : NULL)
#define iter_end_ARR(id, a, n)      ((n) ? &((a)[n]) : NULL)
#define iter_rbegin_ARR(id, a, n)   (n ? &((a)[(n) - 1]) : NULL)
#define iter_rend_ARR(id, a, n)     (n ? &((a)[-1]) : NULL)
#define iter_next_ARR(id, p)        (++(p))
#define iter_prev_ARR(id, p)        (--(p))
#define iter_deref_ARR(p)           (*(p))
#define iter_advance_ARR(id, p, n)  ((p) += n)
#define iter_dist_ARR(id, p1, p2)   ((p2) - (p1))

/* --------------------------------------------------------------------------
 * List iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_LIST(id, l, n)    ((l)->front)
#define iter_end_LIST(id, l, n)      NULL
#define iter_rbegin_LIST(id, l, n)   ((l)->back)
#define iter_rend_LIST(id, l, n)     NULL
#define iter_next_LIST(id, p)        ((p) = (p)->next)
#define iter_prev_LIST(id, p)        ((p) = (p)->prev)
#define iter_deref_LIST(p)           ((p)->data)
#define iter_advance_LIST(id, p, n)  iterator_advance_helper(LIST, id, p, n)
#define iter_dist_LIST(id, p1, p2)   __iter_dist_helper_LIST_##id(p1, p2)

/* --------------------------------------------------------------------------
 * AVL Tree iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_AVLTREE(id, t, n)    __avl_successor_##id((t)->root)
#define iter_end_AVLTREE(id, t, n)      NULL
#define iter_rbegin_AVLTREE(id, t, n)   __avl_predecessor_##id((t)->root)
#define iter_rend_AVLTREE(id, t, n)     NULL
#define iter_next_AVLTREE(id, p)        ((p) = __avl_inorder_successor_##id((p)))
#define iter_prev_AVLTREE(id, p)        ((p) = __avl_inorder_predecessor_##id((p)))
#define iter_deref_AVLTREE(p)           ((p)->data)
#define iter_advance_AVLTREE(id, p, n)  iterator_advance_helper(AVLTREE, id, p, n)
#define iter_dist_AVLTREE(id, p1, p2)   __iter_dist_helper_AVLTREE_##id(p1, p2)

/* --------------------------------------------------------------------------
 * String iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_STR(id, s, n)    ((n) ? &((s)[0]) : NULL)
#define iter_end_STR(id, s, n)      ((n) ? &((s)[n]) : NULL)
#define iter_rbegin_STR(id, s, n)   (n ? &((s)[(n) - 1]) : NULL)
#define iter_rend_STR(id, s, n)     (n ? &((s)[-1]) : NULL)
#define iter_next_STR(id, p)        (++(p))
#define iter_prev_STR(id, p)        (--(p))
#define iter_deref_STR(p)           (*(p))
#define iter_advance_STR(id, p, n)  ((p) += n)
#define iter_dist_STR(id, p1, p2)   ((p2) - (p1))

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
