#include "array.h"
#include "list.h"
#include <time.h>
#include <limits.h>

char *ProgName = NULL;
#define MAX_NUM UINT_MAX
#define NLOOPS 1

typedef enum {
    TEST_ARRAY,
    TEST_LIST
} DSTest;

void usage(void) {
    fprintf(stderr, "Usage: %s\n", ProgName);
    fprintf(stderr, "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n");
    fprintf(stderr, "    -n NELEM            Number of elements to sort\n");
    fprintf(stderr, "    -r                  Whether to use random numbers\n");
    exit(1);
}

void test_list(unsigned n, bool r) {
    clock_t before, after;
    double total = 0;
    unsigned temp;
    List *l;
    for (int t = 0; t < NLOOPS; ++t) {
        l = list_new(&unsigned_helper, LIST_INIT_EMPTY);
        if (r) {
            for (unsigned i = 0; i < n; ++i) {
                temp = rand() % MAX_NUM;
                list_push_back(l, &temp);
            }
        } else {
            for (unsigned i = n; i > 0; --i) {
                list_push_back(l, &i);
            }
        }
        before = clock();
        list_sort(l);
        after = clock();

        total += (double) (after - before) / CLOCKS_PER_SEC;
        list_free(l);
    }
    printf("%f\n", total / (double) NLOOPS);
}

void test_arr(unsigned n, bool r) {
    clock_t before, after;
    double total = 0;
    unsigned temp;
    Array *a;
    for (int t = 0; t < NLOOPS; ++t) {
        a = array_new(&unsigned_helper, ARR_INIT_EMPTY);
        if (r) {
            for (unsigned i = 0; i < n; ++i) {
                temp = rand() % MAX_NUM;
                array_push_back(a, &temp);
            }
        } else {
            for (unsigned i = n; i > 0; --i) {
                array_push_back(a, &i);
            }
        }
        before = clock();
        array_sort(a);
        after = clock();

        total += (double) (after - before) / CLOCKS_PER_SEC;
        array_free(a);
    }
    printf("%f\n", total / (double) NLOOPS);
}

int main(int argc, char *argv[]) {
    ProgName = argv[0];
    int argind = 1;
    DSTest type = TEST_ARRAY;
    unsigned n = 0;
    char *temp;
    bool randomNums = false;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++];
        switch(arg[1]) {
            case 'd':
                temp = argv[argind++];
                if (streq(temp, "LIST")) {
                    type = TEST_LIST;
                }
                break;
            case 'n':
                n = (unsigned) atoi(argv[argind++]);
                break;
            case 'r':
                randomNums = true;
                break;
            default:
                usage();
                break;
        }
    }
    srand(time(NULL));

    switch (type) {
        case TEST_ARRAY:
            test_arr(n, randomNums);
            break;
        default:
            test_list(n, randomNums);
            break;
    }

    return 0;
}
