#include <list>
#include <iostream>
#include <ctime>
#include <climits>
#include <cstring>

#define MAX_NUM UINT_MAX
#define NLOOPS 1

void usage(void) {
    std::cerr << "Usage: \n";
    std::cerr << "    -d DATA_STRUTURE    One of [ARRAY,LIST]  \n";
    std::cerr << "    -n NELEM            Number of elements to sort\n";
    std::cerr << "    -r                  Whether to use random numbers\n";
    exit(1);
}

void test_list(unsigned n, bool r) {
    std::cout << std::fixed;
    clock_t before, after;
    double total = 0;
    unsigned temp;
    std::list<unsigned> *l;

    for (int t = 0; t < NLOOPS; ++t) {
        l = new std::list<unsigned>;
        if (r) {
            for (unsigned i = 0; i < n; ++i) {
                temp = rand() % MAX_NUM;
                l->push_back(temp);
            }
        } else {
            for (unsigned i = n; i > 0; --i) {
                l->push_back(i);
            }
        }
        before = clock();
        l->sort();
        after = clock();

        total += (double) (after - before) / CLOCKS_PER_SEC;
        delete l;
    }
    std::cout << (total / (double) NLOOPS) << '\n';
}

int main(int argc, char *argv[]) {
    int argind = 1;
    unsigned n = 0;
    bool randomNums = false;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++];
        switch(arg[1]) {
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

    test_list(n, randomNums);
    return 0;
}
