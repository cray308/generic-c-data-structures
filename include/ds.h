#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define __DS_FUNC_PREFIX static __attribute__((__unused__))
#define __DS_FUNC_PREFIX_INL __DS_FUNC_PREFIX inline


/**
 * If successful, returns the positive modulus.
 *
 * @param   index  Index in data structure.
 * @param   size   Size of data structure to be indexed.
 *
 * @return         If the index is valid, returns the positive modulus. Otherwise, returns -1.
 */
__DS_FUNC_PREFIX_INL int modulo(int index, size_t size) {
    bool valid = index < 0 ? ((int) size + index >= 0) : (index < (int) size);
    if (!valid) return -1;

    int m = index % (int) size;
    return (m < 0) ? (m + (int) size) : m;
}

__DS_FUNC_PREFIX_INL void *__ds_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Out of memory error.\n");
        exit(1);  
    }
    return ptr;
}

__DS_FUNC_PREFIX_INL void *__ds_calloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    if (!ptr) {
        fprintf(stderr, "Out of memory error.\n");
        exit(1);  
    }
    return ptr;
}

__DS_FUNC_PREFIX_INL void *__ds_realloc(void *ptr, size_t size) {
    void *new = realloc(ptr, size);
    if (!new) {
        fprintf(stderr, "Out of memory error.\n");
        exit(1);  
    }
    return new;
}

#define min(a,b) (((a) <= (b)) ? (a) : (b))
#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define streq(a,b) (strcmp((a), (b)) == 0)


#define ds_cmp_num_lt(n1, n2) ((n1) < (n2))
#define ds_cmp_str_lt(s1, s2) (strcmp((s1), (s2)) < 0)

#define ds_cmp_eq(cmp_lt, x, y) (!(cmp_lt(x, y)) && !(cmp_lt(y, x)))
#define ds_cmp_neq(cmp_lt, x, y) (!(ds_cmp_eq(cmp_lt, x, y)))
#define ds_cmp_leq(cmp_lt, x, y) (cmp_lt(x, y) || ds_cmp_eq(cmp_lt, x, y))
#define ds_cmp_gt(cmp_lt, x, y) (cmp_lt(y, x))

#define gen_node(id, t)                                                                                      \
typedef struct id id;                                                                                        \
struct id {                                                                                                  \
    id *next;                                                                                                \
    t data;                                                                                                  \
};                                                                                                           \

#endif
