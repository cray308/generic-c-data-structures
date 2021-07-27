#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ____cds_do_nothing

#define __DS_FUNC_PREFIX static __attribute__((__unused__))
#if __STDC_VERSION__ >= 199901L
#define __DS_FUNC_PREFIX_INL __DS_FUNC_PREFIX inline
#else
#define __DS_FUNC_PREFIX_INL __DS_FUNC_PREFIX
#endif

#define DSDefault_shallowCopy(dest, src) ((dest) = (src))
#define DSDefault_shallowDelete(x) /* do nothing */
#define DSDefault_deepCopyStr(dest, src) do { __ds_malloc(dest, strlen(src) + 1); strcpy(dest, src); } while(0)
#define DSDefault_deepDelete(x) free(x)

#define DSDefault_addrOfVal(x) &(x)
#define DSDefault_addrOfRef(x) x
#define DSDefault_sizeOfVal(x) sizeof(x)
#define DSDefault_sizeOfStr(x) strlen(x)

#define __ds_adjust_index(index, size, earlyReturn) \
    if ((index) >= (size)) { \
        earlyReturn;   \
    }

#define __ds_malloc(ptr, size)                                                                               \
    ptr = malloc(size);                                                                                      \
    if (!ptr) {                                                                                              \
        write(2, "[DS] Out of memory error.\n", 26);                                                         \
        exit(1);                                                                                             \
    }                                                                                                        \

#define __ds_calloc(ptr, nmemb, size)                                                                        \
    ptr = calloc(nmemb, size);                                                                               \
    if (!ptr) {                                                                                              \
        write(2, "[DS] Out of memory error.\n", 26);                                                         \
        exit(1);                                                                                             \
    }                                                                                                        \

#define __ds_realloc(newPtr, oldPtr, size)                                                                   \
    newPtr = realloc(oldPtr, size);                                                                          \
    if (!newPtr) {                                                                                           \
        write(2, "[DS] Out of memory error.\n", 26);                                                         \
        exit(1);                                                                                             \
    }                                                                                                        \

#define min(a,b) (((a) <= (b)) ? (a) : (b))
#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define streq(a,b) (strcmp((a), (b)) == 0)


#define ds_cmp_num_lt(n1, n2) ((n1) < (n2))
#define ds_cmp_num_eq(n1, n2) ((n1) == (n2))
#define ds_cmp_str_lt(s1, s2) (strcmp((s1), (s2)) < 0)
#define ds_cmp_str_eq(s1, s2) (strcmp((s1), (s2)) == 0)

#define ds_cmp_eq(cmp_lt, x, y) (!(cmp_lt(x, y)) && !(cmp_lt(y, x)))
#define ds_cmp_neq(cmp_lt, x, y) (!(ds_cmp_eq(cmp_lt, x, y)))
#define ds_cmp_leq(cmp_lt, x, y) (cmp_lt(x, y) || ds_cmp_eq(cmp_lt, x, y))
#define ds_cmp_gt(cmp_lt, x, y) (cmp_lt(y, x))

#endif
