#include "set.h"
#include <assert.h>

#define LEN_INTS 30
#define LEN_STRS 23

gen_set(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_set(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

void test_basic_rbtree_ints(void) {
    Set_int *s = set_new(int);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_size(s) == 0);

    {
        int i = 0;
        SetEntry_int *it;
        set_iter(int, s, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }

    int ints[] = {58490, 13829, 44828, 35725, 20384, 46283, 56369, 21807, 15590, 1520, 60285, 11387,
    34372, 49312, 52170, 30455, 52223, 41564, 53018, 3098, 25875, 45077, 36617, 58448, 42128, 13655,
    7077, 62936, 46937, 14427};
    int ints_sorted[] = {1520, 3098, 7077, 11387, 13655, 13829, 14427, 15590, 20384, 21807, 25875, 30455,
    34372, 35725, 36617, 41564, 42128, 44828, 45077, 46283, 46937, 49312, 52170, 52223, 53018, 56369,
    58448, 58490, 60285, 62936};

    {
        int counter = 0;
        SetEntry_int *ptr;

        for (int i = 0; i < LEN_INTS; ++i) {
            set_insert(int, s, ints[i]);
        }
        set_iter(int, s, ptr) {
            assert(ptr->data == ints_sorted[counter++]);
        }
        assert(counter == LEN_INTS);
        assert(counter == set_size(s));

        counter = LEN_INTS - 1;
        set_riter(int, s, ptr) {
            assert(ptr->data == ints_sorted[counter--]);
        }
    }

    {
        int test_int = 20384;

        SetEntry_int *e = set_find(int, s, test_int);
        assert(e != NULL);
        set_remove_entry(int, s, e);
        e = set_find(int, s, test_int);
        assert(e == NULL);

        test_int = 3098;
        set_remove_value(int, s, test_int);
        e = set_find(int, s, test_int);
        assert(e == NULL);
    }
    set_free(int, s);
}

void test_basic_rbtree_strs(void) {
    Set_str *s = set_new(str);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_size(s) == 0);

    {
        int i = 0;
        SetEntry_str *it;
        set_iter(str, s, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }

    char strs[][64] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
    "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd","polkitd","chrisray",
    "avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};
    char strs_sorted[][64] = {"avahi","bin","chrisray","colord","daemon","dbus","ftp","geoclue","git",
    "http","lightdm","mail","nobody","polkitd","root","rtkit","systemd-coredump","systemd-journal-remote",
    "systemd-network","systemd-resolve","systemd-timesync","usbmux","uuidd"};

    {
        int counter = 0;
        SetEntry_str *ptr;

        for (int i = 0; i < LEN_STRS; ++i) {
            set_insert(str, s, strs[i]);
        }
        set_iter(str, s, ptr) {
            assert(streq(iter_deref(TREE, ptr), strs_sorted[counter++]));
        }
        assert(counter == LEN_STRS);
        assert(counter == set_size(s));

        counter = LEN_STRS - 1;
        set_riter(str, s, ptr) {
            assert(streq(iter_deref(TREE, ptr), strs_sorted[counter--]));
        }
    }

    {
        char *test_str = "mail";

        SetEntry_str *e = set_find(str, s, test_str);
        assert(e != NULL);
        set_remove_entry(str, s, e);
        e = set_find(str, s, test_str);
        assert(e == NULL);

        test_str = "git";
        set_remove_value(str, s, test_str);
        e = set_find(str, s, test_str);
        assert(e == NULL);
    }
    set_free(str, s);
}

void test_init_clear(void) {
    Set_int *s = set_new(int);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_size(s) == 0);

    {
        int i = 0;
        SetEntry_int *it;
        set_iter(int, s, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }
    set_free(int, s);

    s = set_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);
    assert(set_size(s) == 4);
    assert(!set_empty(s));
    assert(s->root != NULL);

    Set_int *s2 = set_createCopy(int, s);
    assert(set_size(s2) == 4);
    assert(s2->root != NULL);

    set_clear(int, s);
    assert(set_size(s) == 0);
    assert(set_empty(s));
    assert(s->root == NULL);

    set_free(int, s);
    set_free(int, s2);
}

void test_membership(void) {
    Set_int *s = set_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);

    assert(!set_contains(int, s, 4));
    assert(set_contains(int, s, 3));
    set_free(int, s);
}

void test_strings(void) {
    Set_str *s = set_new(str);
    assert(s->root == NULL);
    assert(set_empty(s));
    assert(set_size(s) == 0);

    {
        int i = 0;
        SetEntry_str *it;
        set_iter(str, s, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }
    set_free(str, s);

    s = set_new_fromArray(str, ((char *[]){"treatment","tread","court","court","animal"}), 5);
    assert(set_size(s) == 4);
    assert(!set_empty(s));
    assert(s->root != NULL);

    Set_str *s2 = set_createCopy(str, s);
    assert(set_size(s2) == 4);
    assert(s2->root != NULL);

    assert(!set_contains(str, s2, "cour"));
    assert(set_contains(str, s2, "court"));

    set_clear(str, s);
    assert(set_size(s) == 0);
    assert(set_empty(s));
    assert(s->root == NULL);

    set_free(str, s);
    set_free(str, s2);
}

void test_remove(void) {
    Set_int *s = set_new_fromArray(int, ((int[]){8, 5, 3, 3, 1}), 5);

    set_remove_value(int, s, 4);
    assert(set_size(s) == 4);

    set_remove_value(int, s, 8);
    assert(set_size(s) == 3);
    assert(!set_contains(int, s, 8));
    set_free(int, s);

    Set_str *s2 = set_new_fromArray(str, ((char *[]){"treatment","tread","court","court","animal"}), 5);
    set_remove_value(str, s2, "cour");
    assert(set_size(s2) == 4);

    set_remove_value(str, s2, "treatment");
    assert(set_size(s2) == 3);
    assert(!set_contains(str, s2, ""));
    set_free(str, s2);
}

void test_insert(void) {
    Set_int *s = set_new(int);
    for (int i = 1; i <= 5; ++i) {
        set_insert(int, s, i * 10);
    }
    assert(set_size(s) == 5);

    {
        int inserted = -1;
        set_insert_withResult(int, s, 20, &inserted);
        assert(set_size(s) == 5);
        assert(!inserted);

        inserted = -1;
        set_insert_withResult(int, s, 25, &inserted);
        assert(set_size(s) == 6);
        assert(inserted);
    }

    set_insert(int, s, 24);
    assert(set_size(s) == 7);
    set_insert(int, s, 26);
    assert(set_size(s) == 8);

    set_insert_fromArray(int, s, ((int[]){5, 10, 15}), 3);
    assert(set_size(s) == 10);

    {
        Set_int *s2 = set_new(int);
        SetEntry_int *begin = set_find(int, s, 10);
        SetEntry_int *end = set_find(int, s, 40);

        int comparison[] = {10, 15, 20, 24, 25, 26, 30};

        set_insert_fromSet(int, s2, begin, end);
        assert(set_size(s2) == 7);

        SetEntry_int *it;
        int i = 0;
        set_iter(int, s2, it) {
            assert(it->data == comparison[i++]);
        }
        set_free(int, s2);
    }

    set_free(int, s);
}

void test_insert_strs(void) {
    char buf[16];
    Set_str *s = set_new(str);
    for (int i = 1; i <= 5; ++i) {
        snprintf(buf, 16, "%d", i * 10);
        set_insert(str, s, buf);
    }
    assert(set_size(s) == 5);

    {
        int inserted = -1;
        set_insert_withResult(str, s, "20", &inserted);
        assert(set_size(s) == 5);
        assert(!inserted);
    }

    set_insert(str, s, "25");
    assert(set_size(s) == 6);
    set_insert(str, s, "24");
    assert(set_size(s) == 7);
    set_insert(str, s, "26");
    assert(set_size(s) == 8);

    set_insert_fromArray(str, s, ((char *[]){"5","10","15"}), 3);
    assert(set_size(s) == 10);

    {
        Set_str *s2 = set_new(str);
        SetEntry_str *begin = set_find(str, s, "10");
        SetEntry_str *end = set_find(str, s, "40");

        char *comparison[] = {"10","15","20","24","25","26","30"};

        set_insert_fromSet(str, s2, begin, end);
        assert(set_size(s2) == 7);

        SetEntry_str *it;
        int i = 0;
        set_iter(str, s2, it) {
            assert(streq(it->data, comparison[i++]));
        }
        set_free(str, s2);
    }

    set_free(str, s);
}

void test_erase(void) {
    Set_int *s = set_new_fromArray(int, ((int[]){5, 10, 15, 20, 24, 25, 26, 30, 40, 50}), 10);
    assert(set_size(s) == 10);

    set_remove_value(int, s, 20);
    assert(set_size(s) == 9);

    set_remove_value(int, s, 20);
    assert(set_size(s) == 9);

    {
        SetEntry_int *begin = set_find(int, s, 5);
        SetEntry_int *end = set_find(int, s, 15);

        set_erase(int, s, begin, end);
        assert(set_size(s) == 7);

        begin = set_find(int, s, 26);
        end = NULL;
        set_erase(int, s, begin, end);
        assert(set_size(s) == 3);
    }

    {
        int comparison[] = {15, 24, 25};
        SetEntry_int *it;
        int i = 0;
        set_iter(int, s, it) {
            assert(it->data == comparison[i++]);
        }
    }
    set_free(int, s);
}

void test_erase_strs(void) {
    Set_str *s = set_new_fromArray(str, ((char *[]){"a","ba","bb","ca","cd","ce","cf","da","ea","fa"}), 10);
    assert(set_size(s) == 10);

    set_remove_value(str, s, "ca");
    assert(set_size(s) == 9);

    set_remove_value(str, s, "ca");
    assert(set_size(s) == 9);

    {
        SetEntry_str *begin = set_find(str, s, "a");
        SetEntry_str *end = set_find(str, s, "bb");

        set_erase(str, s, begin, end);
        assert(set_size(s) == 7);

        begin = set_find(str, s, "cf");
        end = NULL;
        set_erase(str, s, begin, end);
        assert(set_size(s) == 3);
    }

    {
        char *comparison[] = {"bb","cd","ce"};
        SetEntry_str *it;
        int i = 0;
        set_iter(str, s, it) {
            assert(streq(it->data, comparison[i++]));
        }
    }
    set_free(str, s);
}

void test_union(void) {
    int comparison[] = {1, 2, 3, 4, 5};

    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 2, 3}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){3, 4, 5}), 3);

    Set_int *s3 = set_union(int, s1, s2);
    assert(set_size(s3) == 5);

    for (int i = 0; i < 5; ++i) {
        assert(set_contains(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_intersection(void) {
    int comparison[] = {3};

    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 2, 3}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){3, 4, 5}), 3);

    Set_int *s3 = set_intersection(int, s1, s2);
    assert(set_size(s3) == 1);
    assert(s3->root->data == 3);

    for (int i = 0; i < 1; ++i) {
        assert(set_contains(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_difference(void) {
    int comparison[] = {1, 2};

    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 2, 3}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){3, 4, 5}), 3);

    Set_int *s3 = set_difference(int, s1, s2);
    assert(set_size(s3) == 2);

    for (int i = 0; i < 2; ++i) {
        assert(set_contains(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_symmetric_difference(void) {
    int comparison[] = {1, 2, 4, 5};

    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 2, 3}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){3, 4, 5}), 3);

    Set_int *s3 = set_symmetric_difference(int, s1, s2);
    assert(set_size(s3) == 4);

    for (int i = 0; i < 4; ++i) {
        assert(set_contains(int, s3, comparison[i]));
    }

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

void test_subset(void) {
    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 2, 3}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){1, 2, 3, 4, 5}), 5);
    Set_int *s3 = set_new_fromArray(int, ((int[]){3, 4, 5}), 3);

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
    Set_int *s1 = set_new_fromArray(int, ((int[]){1, 3, 5}), 3);
    Set_int *s2 = set_new_fromArray(int, ((int[]){2, 4, 6, 8, 10}), 5);
    Set_int *s3 = set_new_fromArray(int, ((int[]){5, 7, 8}), 3);

    assert(set_isdisjoint(int, s1, s2));
    assert(!set_isdisjoint(int, s2, s3));
    assert(!set_isdisjoint(int, s2, s3));

    set_free(int, s1);
    set_free(int, s2);
    set_free(int, s3);
}

int main(void) {
    test_basic_rbtree_ints();
    test_basic_rbtree_strs();
    test_init_clear();
    test_membership();
    test_strings();
    test_remove();
    test_insert();
    test_insert_strs();
    test_erase();
    test_erase_strs();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_subset();
    test_disjoint();
    return 0;
}
