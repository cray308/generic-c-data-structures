#ifndef PAIR_H
#define PAIR_H

/**
 * Generates a pair type with the specified ID and datatypes.
 *
 * @param   id  ID to be used for the pair (must be unique).
 * @param   t1  Type of first member of the pair.
 * @param   t2  Type of second member of the pair.
 */
#define gen_pair(id, t1, t2)                                                                                 \
typedef struct  {                                                                                            \
    t1 first;                                                                                                \
    t2 second;                                                                                               \
} Pair_##id;                                                                                                           \

#endif
