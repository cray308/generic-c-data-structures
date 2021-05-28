#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
//#include <iomanip>
#include <ctime>
#include <climits>
#include <cstring>

const char *ProgName = NULL;
unsigned n = 10000;

#define runTest(append, sort) {                                                                              \
    clock_t before, after;                                                                                   \
    for (unsigned i = 0; i < n; ++i) {                                                                       \
        append;                                                                                              \
    }                                                                                                        \
    before = clock();                                                                                        \
    sort;                                                                                                    \
    after = clock();                                                                                         \
    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;                                    \
    std::cout << std::fixed << elapsed << '\n';                                                              \
}

enum DSTest {
    TEST_ARRAY,
    TEST_LIST
};

#define usage()                                                                                              \
    std::cerr << "Usage: " << ProgName << '\n';                                                              \
    std::cerr << "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n";                                          \
    std::cerr << "    -n NELEM            Number of elements to sort\n";                                     \
    exit(1);

void test_list(void) {
    std::list<unsigned> *l = new std::list<unsigned>;
    runTest(l->push_back(rand() % UINT_MAX), l->sort())
    delete l;
}

void test_arr(void) {
    std::vector<unsigned> *a = new std::vector<unsigned>;
    runTest(a->push_back(rand() % UINT_MAX), std::sort(a->begin(), a->end()))
    delete a;
}

int main(int argc, char *argv[]) {
    int argind = 1;
    DSTest type = TEST_ARRAY;
    ProgName = argv[0];

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++], *temp;
        switch(arg[1]) {
            case 'd':
                temp = argv[argind++];
                if (strcmp(temp, "LIST") == 0) {
                    type = TEST_LIST;
                } else if (strcmp(temp, "ARRAY") == 0) {
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
        }
    }
    srand(time(NULL));

    switch (type) {
        case TEST_ARRAY:
            test_arr();
            break;
        case TEST_LIST:
            test_list();
            break;
    }
    return 0;
}
