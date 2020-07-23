#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define DS_UNUSED __attribute__((__unused__))
#define __DS_FUNC_PREFIX static DS_UNUSED
#define __DS_FUNC_PREFIX_INL __DS_FUNC_PREFIX inline


/**
 * Checks whether an index is reasonable.
 
 * @param   index  Index in data structure.
 * @param   size   Size of data structure to be indexed.
 * 
 * @return         -If the index is positive, whether it is less than the size.
 *                 -If the index is negative, whether subtracting it from the size is at least 0.
 */
#define check_index(index, size)                                                                             \
    (((index) < 0) ? ((int) (size) + (index) >= 0) : ((index) < (int) (size)))


/**
 * If successful, returns the positive modulus.
 *
 * @param   index  Index in data structure.
 * @param   size   Size of data structure to be indexed.
 *
 * @return         If the index is valid, returns the positive modulus. Otherwise, returns -1.
 */
static inline DS_UNUSED int modulo(int index, size_t size) {
    if (!check_index(index, size)) {
        return -1;
    }

    int m = index % (int) size;
    return (m < 0) ? (m + (int) size) : m;
}

#ifndef DS_OOM
#define DS_OOM()                                                                                             \
    do {                                                                                                     \
        fprintf(stderr, "Out of memory error.\n");                                                           \
        exit(1);                                                                                             \
    } while(0)
#endif

#define min(a,b) (((a) <= (b)) ? (a) : (b))
#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define streq(a,b) (strcmp((a), (b)) == 0)


#define ds_cmp_num_lt(n1, n2) ((n1) < (n2))
#define ds_cmp_str_lt(s1, s2) (strcmp((s1), (s2)) < 0)

#define ds_cmp_eq(cmp_lt, x, y) (!(cmp_lt(x, y)) && !(cmp_lt(y, x)))
#define ds_cmp_neq(cmp_lt, x, y) (!(ds_cmp_eq(cmp_lt, x, y)))
#define ds_cmp_leq(cmp_lt, x, y) (cmp_lt(x, y) || ds_cmp_eq(cmp_lt, x, y))
#define ds_cmp_gt(cmp_lt, x, y) (cmp_lt(y, x))

#define __gen_node(id, t)                                                                                    \
typedef struct Node_##id Node_##id;                                                                          \
struct Node_##id {                                                                                           \
    Node_##id *next;                                                                                         \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
Node_##id *node_new_##id(void) {                                                                             \
    Node_##id *node = malloc(sizeof(Node_##id));                                                             \
    if (!node) {                                                                                             \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(node, 0, sizeof(Node_##id));                                                                      \
    return node;                                                                                             \
}                                                                                                            \

#endif
