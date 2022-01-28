#ifndef DS_H
#define DS_H

#ifndef __CDS_SCAN
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#endif /* __CDS_SCAN */

#if DEBUG
#ifndef __CDS_SCAN
#include <assert.h>
#endif /* __CDS_SCAN */
#define customAssert(x) assert(x);
#else
#define customAssert(x)
#endif /* DEBUG */

#define DS_ARG_NOT_APPLICABLE UINT_MAX

#define DSDefault_shallowCopy(dest, src) (dest) = (src)
#define DSDefault_shallowDelete(x) /* do nothing */
#define DSDefault_deepCopyStr(dest, src) do {                                            \
    dest = malloc(strlen(src) + 1);                                                      \
    if (!dest) exit(1);                                                                  \
    strcpy(dest, src);                                                                   \
} while(0)
#define DSDefault_deepDelete(x) free(x)

#define DSDefault_addrOfVal(x) &(x)
#define DSDefault_addrOfRef(x) x
#define DSDefault_sizeOfVal(x) sizeof(x)
#define DSDefault_sizeOfStr(x) strlen(x)

#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define streq(a,b) (strcmp(a, b) == 0)

#define ds_cmp_num_lt(n1, n2) ((n1) < (n2))
#define ds_cmp_num_eq(n1, n2) ((n1) == (n2))
#define ds_cmp_num(n1, n2) ((n1) < (n2) ? -1 : ((n1) > (n2)))
#define ds_cmp_str_lt(s1, s2) (strcmp(s1, s2) < 0)
#define ds_cmp_str_eq(s1, s2) (strcmp(s1, s2) == 0)
#define ds_cmp_str(s1, s2) strcmp(s1, s2)

#define ds_cmp_eq(cmp_lt, x, y) (!cmp_lt(x, y) && !cmp_lt(y, x))
#define ds_cmp_neq(cmp_lt, x, y) !ds_cmp_eq(cmp_lt, x, y)
#define ds_cmp_leq(cmp_lt, x, y) (cmp_lt(x, y) || ds_cmp_eq(cmp_lt, x, y))
#define ds_cmp_gt(cmp_lt, x, y) cmp_lt(y, x)

#endif /* DS_H */
