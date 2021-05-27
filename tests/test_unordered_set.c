#include "unordered_set.h"
#include <assert.h>

gen_uset(int, int, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_uset(str, char *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete)

void test_init_clear(void) {
    int i = 0, *it, resultArr[4] = {-1};
    char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    char *strResults[7] = {NULL}, **sptr;
    USet_int *s = uset_new(int);
    USet_str *s2 = uset_new(str), *s3;
    assert(uset_empty(s));
    assert(uset_size(s) == 0);

    uset_iter(int, s, it) {
        i++;
    }
    assert(it == NULL);
    assert(i == 0);
    uset_free(int, s);

    {
        int a[] = {8,5,3,3,1};
        s = uset_new_fromArray(int, a, 5);
        assert(uset_size(s) == 4);
        assert(!uset_empty(s));
    }

    i = 0;
    uset_iter(int, s, it) {
        resultArr[i++] = *it;
    }
    assert(i == 4);
    for (i = 0; i < 4; ++i) {
        assert(resultArr[i] != -1);
    }

    uset_insert_fromArray(str, s2, days, 7);
    assert(uset_size(s2) == 7);
    i = 0;
    uset_iter(str, s2, sptr) {
        strResults[i++] = *sptr;
    }
    assert(i == 7);
    for (i = 0; i < 7; ++i) {
        assert(strResults[i]);
    }

    s3 = uset_createCopy(str, s2);
    memset(strResults, 0, 7 * sizeof(char *));
    assert(uset_size(s2) == 7);
    i = 0;
    uset_iter(str, s3, sptr) {
        strResults[i++] = *sptr;
    }
    assert(i == 7);
    for (i = 0; i < 7; ++i) {
        assert(strResults[i]);
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
    int a1[] = {8,5,3,3,1};
    USet_int *s = uset_new_fromArray(int, a1, 5);
    assert(!uset_contains(int, s, 4));
    assert(uset_contains(int, s, 3));
    uset_free(int, s);
}

void test_remove(void) {
    int a1[] = {8,5,3,3,1};
    USet_int *s = uset_new_fromArray(int, a1, 5);

    uset_remove(int, s, 4);
    assert(uset_size(s) == 4);
    uset_remove(int, s, 8);
    assert(uset_size(s) == 3);
    assert(!uset_contains(int, s, 8));
    uset_free(int, s);
}

void test_resizing(void) {
    USet_int *s = uset_new(int);
    int i;
    for (i = 0; i < 100; ++i) {
        uset_insert(int, s, i);
    }

    assert(s->cap > 32);
    {
        double oldLoadFactor = uset_max_load_factor(s);
        size_t oldCap = s->cap;
        uset_rehash(int, s, oldCap);
        assert(s->cap == oldCap);
        uset_set_load_factor(int, s, 0.5);
        assert(oldLoadFactor != uset_max_load_factor(s));
    }

    for (i = 0; i < 100; ++i) {
        assert(uset_contains(int, s, i));
    }
    while (!uset_empty(s)) {
        uset_remove(int, s, i);
        assert(uset_size(s) == i);
        assert(!uset_contains(int, s, i--));
    }
    assert(uset_size(s) == 0);
    uset_free(int, s);
}

void test_insert(void) {
    USet_int *s = uset_new(int);
    int i, *it, inserted = -1, results[10] = {-1};
    for (i = 1; i <= 5; ++i) {
        uset_insert(int, s, i * 10);
    }
    assert(uset_size(s) == 5);

    uset_insert_withResult(int, s, 20, &inserted);
    assert(uset_size(s) == 5);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(int, s, 25, &inserted);
    assert(uset_size(s) == 6);
    assert(inserted);

    uset_insert(int, s, 24);
    assert(uset_size(s) == 7);
    uset_insert(int, s, 26);
    assert(uset_size(s) == 8);
    {
        int a[] = {5,10,15};
        uset_insert_fromArray(int, s, a, 3);
        assert(uset_size(s) == 10);
    }

    i = 0;
    uset_iter(int, s, it) {
        results[i++] = *it;
    }
    for (i = 0; i < 10; ++i) {
        assert(results[i] != -1);
    }
    uset_free(int, s);
}

int main(void) {
    test_init_clear();
    test_membership();
    test_remove();
    test_resizing();
    test_insert();
    return 0;
}
