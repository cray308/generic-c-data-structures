#include "set.h"
#include <assert.h>

void test_init_clear(void) {
    Set *s = set_new(&int_helper, SET_INIT_EMPTY);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_len(s) == 0);

    int i = 0;
    int *iptr;
    set_iter(s, iptr) {
        i++;
    }
    assert(iptr == NULL);
    assert(i == 0);
    set_free(s);

    int vals[] = {8, 5, 3, 3, 1};
    s = set_new(&int_helper, SET_INIT_BUILTIN, &vals, 5);
    assert(set_len(s) == 4);
    assert(!set_empty(s));
    assert(s->root != NULL);

    Set *s2 = set_new(&int_helper, SET_INIT_SET, s);
    assert(set_len(s2) == 4);
    assert(s2->root != NULL);

    set_clear(s);
    assert(set_len(s) == 0);
    assert(set_empty(s));
    assert(s->root == NULL);

    set_free(s);
    set_free(s2);
}

void test_membership(void) {
    int vals[] = {8, 5, 3, 3, 1};
    Set *s = set_new(&int_helper, SET_INIT_BUILTIN, &vals, 5);
    
    int f = 4;
    assert(!set_in(s, &f));
    f = 3;
    assert(set_in(s, &f));
    set_free(s);
}

void test_remove(void) {
    int vals[] = {8, 5, 3, 3, 1};
    Set *s = set_new(&int_helper, SET_INIT_BUILTIN, &vals, 5);
    
    int f = 4;
    set_remove_value(s, &f);
    assert(set_len(s) == 4);

    f = 8;
    set_remove_value(s, &f);
    assert(set_len(s) == 3);
    assert(!set_in(s, &f));
    set_free(s);
}

void test_insert(void) {
    Set *s = set_new(&int_helper, SET_INIT_EMPTY);
    for (int i = 1; i <= 5; ++i) {
        set_insert(s, SET_INSERT_SINGLE, &(int){i * 10});
    }
    assert(set_len(s) == 5);

    set_insert(s, SET_INSERT_SINGLE, &(int){20});
    assert(set_len(s) == 5);

    set_insert(s, SET_INSERT_SINGLE, &(int){25});
    assert(set_len(s) == 6);
    set_insert(s, SET_INSERT_SINGLE, &(int){24});
    assert(set_len(s) == 7);
    set_insert(s, SET_INSERT_SINGLE, &(int){26});
    assert(set_len(s) == 8);

    int myints[]= {5, 10, 15};
    set_insert(s, SET_INSERT_BUILTIN, &myints, 0, 3);
    assert(set_len(s) == 10);

    Set *s2 = set_new(&int_helper, SET_INIT_EMPTY);
    SetEntry *begin = set_find(s, &(int){10});
    SetEntry *end = set_find(s, &(int){40});

    int comparison[] = {10, 15, 20, 24, 25, 26, 30};

    set_insert(s2, SET_INSERT_SET, s, begin, end);
    assert(set_len(s2) == 7);

    int *ptr;
    int i = 0;
    set_iter(s2, ptr) {
        assert(*ptr == comparison[i++]);
    }

    set_free(s2);
    set_free(s);
}

void test_erase(void) {
    int myints[] = {5, 10, 15, 20, 24, 25, 26, 30, 40, 50};
    Set *s = set_new(&int_helper, SET_INIT_BUILTIN, &myints, 10);
    assert(set_len(s) == 10);

    set_remove_value(s, &(int){20});
    assert(set_len(s) == 9);

    set_remove_value(s, &(int){20});
    assert(set_len(s) == 9);

    SetEntry *begin = set_find(s, &(int){5});
    SetEntry *end = set_find(s, &(int){15});

    set_erase(s, begin, end);
    assert(set_len(s) == 7);

    begin = set_find(s, &(int){26});
    end = SET_END;
    set_erase(s, begin, end);
    assert(set_len(s) == 3);

    int comparison[] = {15, 24, 25};
    int *ptr;
    int i = 0;
    set_iter(s, ptr) {
        assert(*ptr == comparison[i++]);
    }
    set_free(s);
}

void test_union(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2, 3, 4, 5};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 3);

    Set *s3 = set_union(s1, s2);
    assert(set_len(s3) == 5);

    for (int i = 0; i < 5; ++i) {
        assert(set_in(s3, &comparison[i]));
    }

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

void test_intersection(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {3};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 3);

    Set *s3 = set_intersection(s1, s2);
    assert(set_len(s3) == 1);
    assert(*((int *) s3->root->data) == 3);

    for (int i = 0; i < 1; ++i) {
        assert(set_in(s3, &comparison[i]));
    }

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

void test_difference(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 3);

    Set *s3 = set_difference(s1, s2);
    assert(set_len(s3) == 2);

    for (int i = 0; i < 2; ++i) {
        assert(set_in(s3, &comparison[i]));
    }

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

void test_symmetric_difference(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2, 4, 5};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 3);

    Set *s3 = set_symmetric_difference(s1, s2);
    assert(set_len(s3) == 4);

    for (int i = 0; i < 4; ++i) {
        assert(set_in(s3, &comparison[i]));
    }

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

void test_subset(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {1, 2, 3, 4, 5};
    int v3[] = {3, 4, 5};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 5);
    Set *s3 = set_new(&int_helper, SET_INIT_BUILTIN, &v3, 3);

    assert(set_issubset(s1, s1));
    assert(set_issuperset(s1, s1));
    assert(set_issubset(s2, s2));
    assert(set_issuperset(s2, s2));

    assert(set_issubset(s1, s2));
    assert(!set_issubset(s2, s3));
    assert(!set_issubset(s1, s3));

    assert(set_issuperset(s2, s1));
    assert(!set_issuperset(s1, s2));
    assert(!set_issuperset(s1, s3));

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

void test_disjoint(void) {
    int v1[] = {1, 3, 5};
    int v2[] = {2, 4, 6, 8, 10};
    int v3[] = {5, 7, 8};

    Set *s1 = set_new(&int_helper, SET_INIT_BUILTIN, &v1, 3);
    Set *s2 = set_new(&int_helper, SET_INIT_BUILTIN, &v2, 5);
    Set *s3 = set_new(&int_helper, SET_INIT_BUILTIN, &v3, 3);

    assert(set_isdisjoint(s1, s2));
    assert(!set_isdisjoint(s1, s3));
    assert(!set_isdisjoint(s2, s3));

    set_free(s1);
    set_free(s2);
    set_free(s3);
}

int main(void) {
    test_init_clear();
    test_membership();
    test_remove();
    test_insert();
    test_erase();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_subset();
    test_disjoint();
    return 0;
}
