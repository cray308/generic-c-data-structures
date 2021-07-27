#include "array.h"
#include "list.h"
#include "alg_helper.h"
#include <time.h>
#include <limits.h>

gen_array_headers(unsigned, unsigned)
gen_array_alg_headers(unsigned, unsigned)
gen_array_source(unsigned, unsigned, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_array_alg_source(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

gen_list_withalg(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

char *ProgName = NULL;
unsigned n = 10000;

#define runTest(append, sort) {                                                                              \
    unsigned i = 0;                                                                                          \
    double elapsed;                                                                                          \
    clock_t before, after;                                                                                   \
    for (; i < n; ++i) {                                                                                     \
        append;                                                                                              \
    }                                                                                                        \
    before = clock();                                                                                        \
    sort;                                                                                                    \
    after = clock();                                                                                         \
    elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;                                           \
    printf("%.6f\n", elapsed);                                                                               \
}

typedef enum {
    TEST_QSORT,
    TEST_ARRAY,
    TEST_LIST
} DSTest;

void usage(void) {
    char *s = "Usage: %s\n"
    "    -d DATA_STRUTURE    One of [ARRAY,LIST,QSORT]\n"
    "    -n NELEM            Number of elements to sort";
    fprintf(stderr, s, ProgName);
    exit(1);
}

void test_list(void) {
    List_unsigned *l = list_new(unsigned);
    runTest(list_push_back(unsigned, l, ((unsigned) rand()) % UINT_MAX), list_sort(unsigned, l))
    list_free(unsigned, l);
}

void test_arr(void) {
    Array_unsigned *a = array_new(unsigned);
    runTest(array_push_back(unsigned, a, ((unsigned) rand()) % UINT_MAX), array_sort(unsigned, a))
    array_free(unsigned, a);
}

int sort_compare(const void *a, const void *b) {
    return *(unsigned *)a < *(unsigned *)b ? -1 : (*(unsigned *)a > *(unsigned *)b ? 1 : 0);
}

void test_qsort(void) {
    unsigned *a = malloc(sizeof(unsigned) * n);
    runTest(a[i] = ((unsigned) rand()) % UINT_MAX, qsort(a, n, sizeof(unsigned), sort_compare))
    free(a);
}

int main(int argc, char *argv[]) {
    int argind = 1;
    DSTest type = TEST_ARRAY;
    ProgName = argv[0];

    while (argind < argc && strlen(argv[argind]) && argv[argind][0] == '-') {
        char* arg = argv[argind++], *temp;
        switch(arg[1]) {
            case 'd':
                temp = argv[argind++];
                if (streq(temp, "LIST")) {
                    type = TEST_LIST;
                } else if (streq(temp, "ARRAY")) {
                    type = TEST_ARRAY;
                } else if (streq(temp, "QSORT")) {
                    type = TEST_QSORT;
                } else {
                    usage();
                }
                break;
            case 'n':
                n = (unsigned) atoi(argv[argind++]);
                break;
            default:
                usage();
                break;
        }
    }
    srand((unsigned) time(NULL));

    switch (type) {
        case TEST_QSORT:
            test_qsort();
            break;
        case TEST_ARRAY:
            test_arr();
            break;
        default:
            test_list();
            break;
    }
    return 0;
}
