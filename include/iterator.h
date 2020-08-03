#ifndef ITERATOR_H
#define ITERATOR_H

#include "ds.h"

typedef enum {
    ITER_ARR,
    ITER_LIST,
    ITER_TREE
} IteratorTypes;

/* --------------------------------------------------------------------------
 * Types: ARR, LIST, TREE, SET, UMAP
 * -------------------------------------------------------------------------- */

#define iter_begin(type, id, ds, n) __iter_begin_##type(id, ds, n)

#define iter_end(type, id, ds, n) __iter_end_##type(id, ds, n)

#define iter_rbegin(type, id, ds, n) __iter_rbegin_##type(id, ds, n)

#define iter_rend(type, id, ds, n) __iter_rend_##type(id, ds, n)

#define iter_next(type, id, p) __iter_next_##type(id, p)

#define iter_prev(type, id, p) __iter_prev_##type(id, p)

#define iter_deref(type, p) __iter_deref_##type(p)

#define iter_advance(type, id, p, n) __iter_advance_##type(id, p, n)

#define iter_dist(type, id, p1, p2) __iter_dist_##type(id, p1, p2)

/* --------------------------------------------------------------------------
 * Array (random access) iterators
 * -------------------------------------------------------------------------- */

#define __gen_iter_ARR(id, t)                                                                                \
typedef t* ArrayIterator_##id;                                                                               \

#define __iter_begin_ARR(id, a, n)    ((n) ? &((a)[0]) : NULL)
#define __iter_end_ARR(id, a, n)      ((n) ? &((a)[n]) : NULL)
#define __iter_rbegin_ARR(id, a, n)   (n ? &((a)[(n) - 1]) : NULL)
#define __iter_rend_ARR(id, a, n)     (n ? &((a)[-1]) : NULL)
#define __iter_next_ARR(id, p)        (++(p))
#define __iter_prev_ARR(id, p)        (--(p))
#define __iter_deref_ARR(p)       (*(p))
#define __iter_advance_ARR(id, p, n)  ((p) += n)
#define __iter_dist_ARR(id, p1, p2)   ((p2) - (p1))

/* --------------------------------------------------------------------------
 * List iterators
 * -------------------------------------------------------------------------- */

#define __gen_iter_LIST(id)                                                                                  \
typedef DLLNode_##id * ListIterator_##id;                                                                    \
                                                                                                             \
__DS_FUNC_PREFIX_INL size_t __list_iter_dist_helper_##id(DLLNode_##id *p1, DLLNode_##id *p2) {               \
    size_t list_iter_dist = 0;                                                                               \
    DLLNode_##id *list_iter_temp = p1;                                                                       \
    while (list_iter_temp != p2) {                                                                           \
        __iter_next_LIST(id, list_iter_temp);                                                                \
        ++list_iter_dist;                                                                                    \
    }                                                                                                        \
    return list_iter_dist;                                                                                   \
}                                                                                                            \

#define __iter_begin_LIST(id, l, n)    ((l)->front)
#define __iter_end_LIST(id, l, n)      NULL
#define __iter_rbegin_LIST(id, l, n)   ((l)->back)
#define __iter_rend_LIST(id, l, n)     NULL
#define __iter_next_LIST(id, p)        ((p) = (p)->next)
#define __iter_prev_LIST(id, p)        ((p) = (p)->prev)
#define __iter_deref_LIST(p)           ((p)->data)
#define __iter_advance_LIST(id, p, n)                                                                        \
do {                                                                                                         \
    int __list_iter_incr = ((n) >= 0) ? 1 : -1;                                                              \
    int __list_iter_count = 0;                                                                               \
    while (__list_iter_count != (n) && (p) != NULL) {                                                        \
        (p) = (__list_iter_incr == 1) ? (p)->next : (p)->prev;                                               \
        __list_iter_count += __list_iter_incr;                                                               \
    }                                                                                                        \
} while (0)
#define __iter_dist_LIST(id, p1, p2)   __list_iter_dist_helper_##id(p1, p2)

/* --------------------------------------------------------------------------
 * Tree iterators
 * -------------------------------------------------------------------------- */

#define __gen_iter_TREE(id)                                                                                  \
typedef RBNode_##id * TreeIterator_##id;                                                                     \
                                                                                                             \
__DS_FUNC_PREFIX_INL size_t __iter_dist_tree_helper_##id(RBNode_##id *p1, RBNode_##id *p2) {                 \
    size_t tree_iter_dist = 0;                                                                               \
    RBNode_##id *tree_iter_temp = p1;                                                                        \
    while (tree_iter_temp != p2) {                                                                           \
        __iter_next_TREE(id, tree_iter_temp);                                                                \
        ++tree_iter_dist;                                                                                    \
    }                                                                                                        \
    return tree_iter_dist;                                                                                   \
}                                                                                                            \

#define __iter_begin_TREE(id, t, n)    __rb_successor_##id((t)->root)
#define __iter_end_TREE(id, t, n)      NULL
#define __iter_rbegin_TREE(id, t, n)   __rb_predecessor_##id((t)->root)
#define __iter_rend_TREE(id, t, n)     NULL
#define __iter_next_TREE(id, p)        ((p) = __rb_inorder_successor_##id((p)))
#define __iter_prev_TREE(id, p)        ((p) = __rb_inorder_predecessor_##id((p)))
#define __iter_deref_TREE(p)           ((p)->data)
#define __iter_advance_TREE(id, p, n)                                                                        \
do {                                                                                                         \
    int __tree_iter_incr = ((n) >= 0) ? 1 : -1;                                                              \
    int __tree_iter_count = 0;                                                                               \
    while (__tree_iter_count != (n) && (p) != NULL) {                                                        \
        (p) = (__tree_iter_incr == 1) ? __iter_next_TREE(id, p) : __iter_prev_TREE(id, p);                   \
        __tree_iter_count += __tree_iter_incr;                                                               \
    }                                                                                                        \
} while (0)
#define __iter_dist_TREE(id, p1, p2)   __iter_dist_tree_helper_##id(p1, p2)

/* --------------------------------------------------------------------------
 * Set iterators
 * -------------------------------------------------------------------------- */

#define __gen_iter_SET(id)                                                                                   \
typedef RBNode_##id * SetIterator_##id;                                                                      \

#define __iter_begin_SET(id, t, n)    __rb_successor_##id((t)->root)
#define __iter_end_SET(id, t, n)      NULL
#define __iter_rbegin_SET(id, t, n)   __rb_predecessor_##id((t)->root)
#define __iter_rend_SET(id, t, n)     NULL
#define __iter_next_SET(id, p)        ((p) = __rb_inorder_successor_##id((p)))
#define __iter_prev_SET(id, p)        ((p) = __rb_inorder_predecessor_##id((p)))
#define __iter_deref_SET(p)           ((p)->data)
#define __iter_advance_SET(id, p, n)  __iter_advance_TREE(id, p, n)
#define __iter_dist_SET(id, p1, p2)   __iter_dist_tree_helper_##id(p1, p2)

/* --------------------------------------------------------------------------
 * Unordered map iterators
 * -------------------------------------------------------------------------- */

#define __gen_iter_UMAP(id)                                                                                  \
typedef Pair_##id * UMapIterator_##id;                                                                       \
__DS_FUNC_PREFIX_INL Pair_##id *__umap_iter_begin_##id(UMap_##id *m) {                                       \
    if (!(m->size)) {                                                                                        \
        m->it.curr = NULL;                                                                                   \
        m->it.idx = m->cap;                                                                                  \
    } else {                                                                                                 \
        unsigned idx = 0;                                                                                    \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = m->buckets[m->it.idx];                                                                  \
    }                                                                                                        \
    return m->it.curr ? &(m->it.curr->p) : NULL;                                                             \
}                                                                                                            \
__DS_FUNC_PREFIX_INL Pair_##id *__umap_iter_next_##id(UMap_##id *m) {                                        \
    if (m->it.curr->next) {                                                                                  \
        m->it.curr = m->it.curr->next;                                                                       \
    } else {                                                                                                 \
        unsigned idx = m->it.idx + 1;                                                                        \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = (m->it.idx >= m->cap) ? NULL : m->buckets[m->it.idx];                                   \
    }                                                                                                        \
    return m->it.curr ? &(m->it.curr->p) : NULL;                                                             \
}                                                                                                            \

#define __iter_begin_UMAP(id, m, n)  __umap_iter_begin_##id(m)
#define __iter_end_UMAP(id, m, n)    NULL
#define __iter_next_UMAP(id, m, p)   ((p) = __umap_iter_next_##id(m))
#define __iter_deref_UMAP(p)         ((p)->second)

#endif
