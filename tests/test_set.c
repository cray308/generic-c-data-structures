#include "set.h"
#include <assert.h>

gen_set(int, int, ds_cmp_num_lt)

void test_init_clear(void) {
    Set_int *s = set_new(int, SET_INIT_EMPTY);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_len(s) == 0);

    int i = 0;
    SetIterator_int it;
    set_iter(int, s, it) {
        i++;
    }
    assert(it == NULL);
    assert(i == 0);
    set_free(int, s);

    int vals[] = {8, 5, 3, 3, 1};
    s = set_new(int, SET_INIT_BUILTIN, vals, 5);
    assert(set_len(s) == 4);
    assert(!set_empty(s));
    assert(s->root != NULL);

    Set_int *s2 = set_new(int, SET_INIT_SET, s);
    assert(set_len(s2) == 4);
    assert(s2->root != NULL);

    set_clear(int, s);
    assert(set_len(s) == 0);
    assert(set_empty(s));
    assert(s->root == NULL);

    set_free(int, s);
    set_free(int, s2);
}

void test_membership(void) {
    int vals[] = {8, 5, 3, 3, 1};
    Set_int *s = set_new(int, SET_INIT_BUILTIN, vals, 5);
    
    int f = 4;
    assert(!set_in(int, s, f));
    f = 3;
    assert(set_in(int, s, f));
    set_free(int, s);
}

void test_remove(void) {
    int vals[] = {8, 5, 3, 3, 1};
    Set_int *s = set_new(int, SET_INIT_BUILTIN, vals, 5);
    
    int f = 4;
    set_remove_value(int, s, f);
    assert(set_len(s) == 4);

    f = 8;
    set_remove_value(int, s, f);
    assert(set_len(s) == 3);
    assert(!set_in(int, s, f));
    set_free(int, s);
}

void test_insert(void) {
    Set_int *s = set_new(int, SET_INIT_EMPTY);
    for (int i = 1; i <= 5; ++i) {
        set_insert(int, s, SET_INSERT_SINGLE, i * 10);
    }
    assert(set_len(s) == 5);

    set_insert(int, s, SET_INSERT_SINGLE, 20);
    assert(set_len(s) == 5);

    set_insert(int, s, SET_INSERT_SINGLE, 25);
    assert(set_len(s) == 6);
    set_insert(int, s, SET_INSERT_SINGLE, 24);
    assert(set_len(s) == 7);
    set_insert(int, s, SET_INSERT_SINGLE, 26);
    assert(set_len(s) == 8);

    int myints[]= {5, 10, 15};
    set_insert(int, s, SET_INSERT_BUILTIN, myints, 3);
    assert(set_len(s) == 10);

    Set_int *s2 = set_new(int, SET_INIT_EMPTY);
    SetEntry_int *begin = set_find(int, s, 10);
    SetEntry_int *end = set_find(int, s, 40);

    int comparison[] = {10, 15, 20, 24, 25, 26, 30};

    set_insert(int, s2, SET_INSERT_SET, begin, end);
    assert(set_len(s2) == 7);

    SetIterator_int it;
    int i = 0;
    set_iter(int, s2, it) {
        assert(iter_deref(SET, it) == comparison[i++]);
    }

    set_free(int, s2);
    set_free(int, s);
}

void test_erase(void) {
    int myints[] = {5, 10, 15, 20, 24, 25, 26, 30, 40, 50};
    Set_int *s = set_new(int, SET_INIT_BUILTIN, myints, 10);
    assert(set_len(s) == 10);

    set_remove_value(int, s, 20);
    assert(set_len(s) == 9);

    set_remove_value(int, s, 20);
    assert(set_len(s) == 9);

    SetEntry_int *begin = set_find(int, s, 5);
    SetEntry_int *end = set_find(int, s, 15);

    set_erase(int, s, begin, end);
    assert(set_len(s) == 7);

    begin = set_find(int, s, 26);
    end = SET_END(int);
    set_erase(int, s, begin, end);
    assert(set_len(s) == 3);

    int comparison[] = {15, 24, 25};
    SetIterator_int it;
    int i = 0;
    set_iter(int, s, it) {
        assert(iter_deref(SET, it) == comparison[i++]);
    }
    set_free(int, s);
}

void test_union(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2, 3, 4, 5};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 3);

    Set_int *s3 = set_union(int, s1, s2);
    assert(set_len(s3) == 5);

    for (int i = 0; i < 5; ++i) {
        assert(set_in(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_intersection(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {3};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 3);

    Set_int *s3 = set_intersection(int, s1, s2);
    assert(set_len(s3) == 1);
    assert(s3->root->data == 3);

    for (int i = 0; i < 1; ++i) {
        assert(set_in(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_difference(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 3);

    Set_int *s3 = set_difference(int, s1, s2);
    assert(set_len(s3) == 2);

    for (int i = 0; i < 2; ++i) {
        assert(set_in(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_symmetric_difference(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {3, 4, 5};
    int comparison[] = {1, 2, 4, 5};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 3);

    Set_int *s3 = set_symmetric_difference(int, s1, s2);
    assert(set_len(s3) == 4);

    for (int i = 0; i < 4; ++i) {
        assert(set_in(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_subset(void) {
    int v1[] = {1, 2, 3};
    int v2[] = {1, 2, 3, 4, 5};
    int v3[] = {3, 4, 5};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 5);
    Set_int *s3 = set_new(int, SET_INIT_BUILTIN, v3, 3);

    assert(set_issubset(int, s1, s1));
    assert(set_issuperset(int, s1, s1));
    assert(set_issubset(int, s2, s2));
    assert(set_issuperset(int, s2, s2));

    assert(set_issubset(int, s1, s2));
    assert(!set_issubset(int, s2, s3));
    assert(!set_issubset(int, s1, s3));

    assert(set_issuperset(int, s2, s1));
    assert(!set_issuperset(int, s1, s2));
    assert(!set_issuperset(int, s1, s3));

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_disjoint(void) {
    int v1[] = {1, 3, 5};
    int v2[] = {2, 4, 6, 8, 10};
    int v3[] = {5, 7, 8};

    Set_int *s1 = set_new(int, SET_INIT_BUILTIN, v1, 3);
    Set_int *s2 = set_new(int, SET_INIT_BUILTIN, v2, 5);
    Set_int *s3 = set_new(int, SET_INIT_BUILTIN, v3, 3);

    assert(set_isdisjoint(int, s1, s2));
    assert(!set_isdisjoint(int, s2, s3));
    assert(!set_isdisjoint(int, s2, s3));

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
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
