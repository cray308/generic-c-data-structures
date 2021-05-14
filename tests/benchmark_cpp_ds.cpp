#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <climits>
#include <cstring>
using std::cout; using std::cerr; using std::vector; using std::list;
using std::fixed; using std::setprecision;
const char *ProgName = NULL;
clock_t before, after;

enum DSTest {
    TEST_ARRAY,
    TEST_LIST
};

void usage(void) {
    cerr << "Usage: " << ProgName << '\n';
    cerr << "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n";
    cerr << "    -n NELEM            Number of elements to sort\n";
    exit(1);
}

void test_list(unsigned n) {
    list<unsigned> *l = new list<unsigned>;
    for (unsigned i = 0; i < n; ++i) {
        l->push_back(rand() % UINT_MAX);
    }
    
    before = clock();
    l->sort();
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    delete l;
    
    cout << fixed << elapsed << '\n';
}

void test_arr(unsigned n) {
    vector<unsigned> *a = new vector<unsigned>;
    for (unsigned i = 0; i < n; ++i) {
        a->push_back(rand() % UINT_MAX);
    }
    
    before = clock();
    std::sort(a->begin(), a->end());
    after = clock();

    double elapsed = ((double) (after - before) / CLOCKS_PER_SEC) * 1000;
    delete a;
    
    cout << fixed << elapsed << '\n';
}

int main(int argc, char *argv[]) {
    ProgName = argv[0];
    int argind = 1;
    unsigned n = 100000;
    DSTest type = TEST_ARRAY;
    char *temp;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++];
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
                break;
        }
    }
    srand(time(NULL));

    switch (type) {
        case TEST_ARRAY:
            test_arr(n);
            break;
        case TEST_LIST:
            test_list(n);
            break;
    }
    return 0;
}
