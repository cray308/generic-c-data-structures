#include "array.h"
#include "list.h"
#include "alg_helper.h"
#include <time.h>
#include <limits.h>

gen_array_withalg(unsigned, unsigned, ds_cmp_num_lt)

gen_list(unsigned, unsigned, ds_cmp_num_lt)

char *ProgName = NULL;

typedef enum {
    TEST_ARRAY,
    TEST_LIST
} DSTest;

void usage(void) {
    fprintf(stderr, "Usage: %s\n", ProgName);
    fprintf(stderr, "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n");
    fprintf(stderr, "    -n NELEM            Number of elements to sort\n");
    exit(1);
}

void test_list(unsigned n) {
    clock_t before, after;
    unsigned temp;
    List_unsigned *l = list_new(unsigned, LIST_INIT_EMPTY);
    for (unsigned i = 0; i < n; ++i) {
        temp = rand() % UINT_MAX;
        list_push_back(unsigned, l, temp);
    }

    before = clock();
    list_sort(unsigned, l);
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    list_free(unsigned, l);
    
    printf("%.6f\n", elapsed);
}

void test_arr(unsigned n) {
    clock_t before, after;
    unsigned temp;
    Array_unsigned *a = array_new_unsigned(ARR_INIT_EMPTY);
    for (unsigned i = 0; i < n; ++i) {
        temp = rand() % UINT_MAX;
        array_push_back_unsigned(a, temp);
    }
    
    before = clock();
    array_sort(unsigned, a);
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;

    array_free(unsigned, a);
    
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
        case TEST_ARRAY:
            test_arr(n);
            break;
        default:
            test_list(n);
            break;
    }

    return 0;
}
