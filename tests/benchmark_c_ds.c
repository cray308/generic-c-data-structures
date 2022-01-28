#include "array.h"
#include "list.h"
#include <stdio.h>
#include <time.h>

gen_array_headers_withAlg(unsigned, unsigned)
gen_array_source_withAlg(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

gen_list_headers_withAlg(unsigned, unsigned)
gen_list_source_withAlg(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

char *ProgName = NULL;
unsigned n = 10000;

typedef enum {
    TEST_QSORT,
    TEST_ARRAY,
    TEST_LIST
} DSTest;

static int usage(void) {
    char *s = "Usage: %s\n"
    "    -d DATA_STRUTURE    One of [ARRAY,LIST,QSORT]\n"
    "    -n NELEM            Number of elements to sort\n";
    fprintf(stderr, s, ProgName);
    return 1;
}

void test_list(void) {
    List_unsigned *l = list_new(unsigned);
    unsigned i = 0;
    double elapsed;
    clock_t before, after;
    for (; i < n; ++i) {
        list_push_back(unsigned, l, ((unsigned) rand()) % UINT_MAX);
    }
    before = clock();
    list_sort(unsigned, l);
    after = clock();
    elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    printf("%.6f\n", elapsed);
    list_free(unsigned, l);
}

void test_arr(void) {
    Array_unsigned *a = array_new(unsigned);
    unsigned i = 0;
    double elapsed;
    clock_t before, after;
    for (; i < n; ++i) {
        array_push_back(unsigned, a, ((unsigned) rand()) % UINT_MAX);
    }
    before = clock();
    array_sort(unsigned, a);
    after = clock();
    elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    printf("%.6f\n", elapsed);
    array_free(unsigned, a);
}

int sort_compare(const void *a, const void *b) {
    return *(unsigned *)a < *(unsigned *)b ? -1 : (*(unsigned *)a > *(unsigned *)b ? 1 : 0);
}

void test_qsort(void) {
    unsigned *a = malloc(sizeof(unsigned) * n);
    unsigned i = 0;
    double elapsed;
    clock_t before, after;
    for (; i < n; ++i) {
        a[i] = ((unsigned) rand()) % UINT_MAX;
    }
    before = clock();
    qsort(a, n, sizeof(unsigned), sort_compare);
    after = clock();
    elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    printf("%.6f\n", elapsed);
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
                    return usage();
                }
                break;
            case 'n':
                n = (unsigned) atoi(argv[argind++]);
                break;
            default:
                return usage();
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
