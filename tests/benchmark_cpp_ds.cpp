#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <climits>
#include <cstring>

const char *ProgName = NULL;
unsigned n = 10000;

enum DSTest {
    TEST_ARRAY,
    TEST_LIST
};

static int usage(void) {
    std::cerr << "Usage: " << ProgName << '\n';
    std::cerr << "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n";
    std::cerr << "    -n NELEM            Number of elements to sort\n";
    return 1;
}

void test_list(void) {
    std::list<unsigned> *l = new std::list<unsigned>;
    clock_t before, after;
    for (unsigned i = 0; i < n; ++i) {
        l->push_back(((unsigned) rand()) % UINT_MAX);
    }
    before = clock();
    l->sort();
    after = clock();
    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    std::cout << std::fixed << elapsed << '\n';
    delete l;
}

void test_arr(void) {
    std::vector<unsigned> *a = new std::vector<unsigned>;
    clock_t before, after;
    for (unsigned i = 0; i < n; ++i) {
        a->push_back(((unsigned) rand()) % UINT_MAX);
    }
    before = clock();
    std::sort(a->begin(), a->end());
    after = clock();
    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    std::cout << std::fixed << elapsed << '\n';
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
        case TEST_ARRAY:
            test_arr();
            break;
        case TEST_LIST:
            test_list();
            break;
    }
    return 0;
}
