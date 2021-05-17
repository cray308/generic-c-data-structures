#include "unordered_set.h"
#include <assert.h>

char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

gen_uset(int, int, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_uset(str, char *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete)

void test_init_clear(void) {
    USet_int *s = uset_new(int);
    assert(uset_empty(s));
    assert(uset_size(s) == 0);

    int i = 0;
    int *it;
    uset_iter(int, s, it) {
        i++;
    }
    assert(it == NULL);
    assert(i == 0);
    uset_free(int, s);

    s = uset_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);
    assert(uset_size(s) == 4);
    assert(!uset_empty(s));

    int resultArr[4] = {-1};
    i = 0;
    uset_iter(int, s, it) {
        resultArr[i++] = *it;
    }
    assert(i == 4);
    for (int x = 0; x < 4; ++x) {
        assert(resultArr[x] != -1);
    }

    char *strResults[7] = {NULL};
    char **sptr;

    USet_str *s2 = uset_new(str);
    uset_insert_fromArray(str, s2, days, 7);
    assert(uset_size(s2) == 7);
    i = 0;
    uset_iter(str, s2, sptr) {
        strResults[i++] = *sptr;
    }
    assert(i == 7);
    for (int x = 0; x < 7; ++x) {
        assert(strResults[x]);
    }

    USet_str *s3 = uset_createCopy(str, s2);
    memset(strResults, 0, 7 * sizeof(char *));
    assert(uset_size(s2) == 7);
    i = 0;
    uset_iter(str, s3, sptr) {
        strResults[i++] = *sptr;
    }
    assert(i == 7);
    for (int x = 0; x < 7; ++x) {
        assert(strResults[x]);
    }

    uset_clear(str, s2);
    assert(uset_size(s2) == 0);
    assert(uset_empty(s2));
    assert(!uset_empty(s3));

    uset_free(int, s);
    uset_free(str, s2);
    uset_free(str, s3);
}

void test_membership(void) {
    USet_int *s = uset_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);

    assert(!uset_contains(int, s, 4));
    assert(uset_contains(int, s, 3));
    uset_free(int, s);
}

void test_remove(void) {
    USet_int *s = uset_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);

    uset_remove(int, s, 4);
    assert(uset_size(s) == 4);

    uset_remove(int, s, 8);
    assert(uset_size(s) == 3);
    assert(!uset_contains(int, s, 8));
    uset_free(int, s);
}

void test_insert(void) {
    USet_int *s = uset_new(int);
    for (int i = 1; i <= 5; ++i) {
        uset_insert(int, s, i * 10);
    }
    assert(uset_size(s) == 5);

    uset_insert(int, s, 20);
    assert(uset_size(s) == 5);

    uset_insert(int, s, 25);
    assert(uset_size(s) == 6);
    uset_insert(int, s, 24);
    assert(uset_size(s) == 7);
    uset_insert(int, s, 26);
    assert(uset_size(s) == 8);

    uset_insert_fromArray(int, s, ((int[]){5, 10, 15}), 3);
    assert(uset_size(s) == 10);

    int results[10] = {-1};
    {
        int *it;
        int i = 0;
        uset_iter(int, s, it) {
            results[i++] = *it;
        }
    }

    for (int i = 0; i < 10; ++i) {
        assert(results[i] != -1);
    }

    uset_free(int, s);
}

int main(void) {
    test_init_clear();
    test_membership();
    test_remove();
    test_insert();
    return 0;
}
