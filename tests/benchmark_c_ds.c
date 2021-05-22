#include "array.h"
#include "list.h"
#include "alg_helper.h"
#include <time.h>
#include <limits.h>

gen_array_withalg(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

gen_list_withalg(unsigned, unsigned, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

char *ProgName = NULL;
clock_t before, after;

typedef enum {
    TEST_QSORT,
    TEST_ARRAY,
    TEST_LIST
} DSTest;

void usage(void) {
    fprintf(stderr, "Usage: %s\n", ProgName);
    fprintf(stderr, "    -d DATA_STRUTURE    One of [ARRAY,LIST,QSORT]\n");
    fprintf(stderr, "    -n NELEM            Number of elements to sort\n");
    exit(1);
}

void test_list(unsigned n) {
    List_unsigned *l = list_new(unsigned);
    for (unsigned i = 0; i < n; ++i) {
        list_push_back(unsigned, l, rand() % UINT_MAX);
    }

    before = clock();
    list_sort(unsigned, l);
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    list_free(unsigned, l);
    
    printf("%.6f\n", elapsed);
}

void test_arr(unsigned n) {
    Array_unsigned *a = array_new_unsigned();
    for (unsigned i = 0; i < n; ++i) {
        array_push_back_unsigned(a, rand() % UINT_MAX);
    }
    
    before = clock();
    array_sort(unsigned, a);
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;

    array_free(unsigned, a);
    
    printf("%.6f\n", elapsed);
}

int sort_compare(const void *a, const void *b) {
    return *(unsigned *)a < *(unsigned *)b ? -1 : (*(unsigned *)a > *(unsigned *)b ? 1 : 0);
}

void test_qsort(unsigned n) {
    unsigned *arr = malloc(sizeof(unsigned) * n);
    for (unsigned i = 0; i < n; ++i) {
        arr[i] = rand() % UINT_MAX;
    }
    
    before = clock();
    qsort(arr, n, sizeof(unsigned), sort_compare);
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;

    free(arr);
    
    printf("%.6f\n", elapsed);
}

int main(int argc, char *argv[]) {
    ProgName = argv[0];
    int argind = 1;
    DSTest type = TEST_ARRAY;
    unsigned n = 10000;
    char *temp;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++];
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
    srand(time(NULL));

    switch (type) {
        case TEST_QSORT:
            test_qsort(n);
            break;
        case TEST_ARRAY:
            test_arr(n);
            break;
        default:
            test_list(n);
            break;
    }

    return 0;
}
