#ifndef PAIR_H
#define PAIR_H

#include "ds.h"


/**
 * Creates a new pair.
 *
 * @param   id      ID used with gen_pair.
 * @param   first   Value of first datatype to store in the pair.
 * @param   second  Value of second datatype to store in the pair.
 *
 * @return          Newly allocated pair.
 */
#define pair_new(id, first, second) pair_new_##id(first, second)


/**
 * Frees the memory which was allocated with array_new.
 *
 * @param   id  ID used with gen_pair.
 * @param   p   Pointer to pair.
 */
#define pair_free(id, p) pair_free_##id(p)


/**
 * Generates pair code for the specified ID and datatypes.
 *
 * @param   id  ID to be used for the pair (must be unique).
 * @param   t1  Type of first member of the pair.
 * @param   t2  Type of second member of the pair.
 */
#define gen_pair(id, t1, t2)                                                                                 \
typedef struct {                                                                                             \
    t1 first;                                                                                                \
    t2 second;                                                                                               \
} Pair_##id;                                                                                                 \
                                                                                                             \
Pair_##id *pair_new_##id(t1 first, t2 second) {                                                              \
    Pair_##id *p = malloc(sizeof(Pair_##id));                                                                \
    if (!p) DS_OOM();                                                                                        \
    p->first = first;                                                                                        \
    p->second = second;                                                                                      \
    return p;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void pair_free_##id(Pair_##id *p) {                                                     \
    if (p) {                                                                                                 \
        free(p);                                                                                             \
    }                                                                                                        \
}                                                                                                            \

#endif
