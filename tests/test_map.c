#include "map.h"
#include <assert.h>

#define NWORDS 100
#define LEN_INTS 30

char words[NWORDS][16] = {0};

int ints[LEN_INTS] = {58490, 13829, 44828, 35725, 20384, 46283, 56369, 21807, 15590, 1520, 60285, 11387,
    34372, 49312, 52170, 30455, 52223, 41564, 53018, 3098, 25875, 45077, 36617, 58448, 42128, 13655,
    7077, 62936, 46937, 14427};
char strs[LEN_INTS][16] = {"58490","13829","44828","35725","20384","46283","56369","21807","15590","1520",
    "60285","11387","34372","49312","52170","30455","52223","41564","53018","3098","25875","45077","36617",
    "58448","42128","13655","7077","62936","46937","14427"};

gen_map(int_str, int, char *, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(strv_int, char *, int, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(strp_int, char *, int, ds_cmp_str_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(nested, char *, Map_strv_int *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, __rbtree_free_strv_int)

void test_basic_rbtree_ints(void) {
    Map_int_str *m = map_new(int_str);
    assert(m->root == NULL);
    assert(map_empty(m));
    assert(map_size(m) == 0);

    {
        int i = 0;
        MapEntry_int_str *it;
        map_iter(int_str, m, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }

    int ints_sorted[] = {1520, 3098, 7077, 11387, 13655, 13829, 14427, 15590, 20384, 21807, 25875, 30455,
        34372, 35725, 36617, 41564, 42128, 44828, 45077, 46283, 46937, 49312, 52170, 52223, 53018, 56369,
        58448, 58490, 60285, 62936};
    char strs_sorted[][16] = {"1520","3098","7077","11387","13655","13829","14427","15590","20384","21807",
    "25875","30455","34372","35725","36617","41564","42128","44828","45077","46283","46937","49312",
    "52170","52223","53018","56369","58448","58490","60285","62936"};

    {
        int counter = 0;
        MapEntry_int_str *ptr;

        for (int i = 0; i < LEN_INTS; ++i) {
            map_insert(int_str, m, pair_make(int_str, ints[i], strs[i]));
        }
        map_iter(int_str, m, ptr) {
            assert(ptr->data.first == ints_sorted[counter]);
            assert(streq(ptr->data.second, strs_sorted[counter++]));
        }
        assert(counter == LEN_INTS);
        assert(counter == map_size(m));

        counter = LEN_INTS - 1;
        map_riter(int_str, m, ptr) {
            assert(ptr->data.first == ints_sorted[counter]);
            assert(streq(ptr->data.second, strs_sorted[counter--]));
        }
        assert(counter == -1);
    }

    {
        int test_int = 20384;

        MapEntry_int_str *e = map_find(int_str, m, test_int);
        assert(e != NULL);
        map_remove_entry(int_str, m, e);
        e = map_find(int_str, m, test_int);
        assert(e == NULL);

        test_int = 3098;
        map_remove_key(int_str, m, test_int);
        e = map_find(int_str, m, test_int);
        assert(e == NULL);

        assert(map_find(int_str, m, 84896) == NULL);
        int oldCount = map_size(m);
        map_remove_key(int_str, m, 84896);
        assert(oldCount == map_size(m));
    }

    map_free(int_str, m);
}

void test_basic_rbtree_strs(void) {
    Map_strv_int *m = map_new(strv_int);
    assert(m->root == NULL);
    assert(map_empty(m));
    assert(map_size(m) == 0);

    {
        int i = 0;
        MapEntry_strv_int *it;
        map_iter(strv_int, m, it) {
            i++;
        }
        assert(it == NULL);
        assert(i == 0);
    }

    char strs_sorted[][16] = {"11387","13655","13829","14427","1520","15590","20384","21807","25875",
    "30455","3098","34372","35725","36617","41564","42128","44828","45077","46283","46937","49312","52170",
    "52223","53018","56369","58448","58490","60285","62936","7077"};
    int ints_sorted[] = {11387, 13655, 13829, 14427, 1520, 15590, 20384, 21807, 25875, 30455, 3098,
        34372, 35725, 36617, 41564, 42128, 44828, 45077, 46283, 46937, 49312, 52170, 52223, 53018, 56369,
        58448, 58490, 60285, 62936, 7077};

    {
        int counter = 0;
        MapEntry_strv_int *ptr;

        for (int i = 0; i < LEN_INTS; ++i) {
            map_insert(strv_int, m, pair_make(strv_int, strs[i], ints[i]));
        }
        map_iter(strv_int, m, ptr) {
            assert(streq(ptr->data.first, strs_sorted[counter]));
            assert(ptr->data.second == ints_sorted[counter++]);
        }
        assert(counter == LEN_INTS);
        assert(counter == map_size(m));

        counter = LEN_INTS - 1;
        map_riter(strv_int, m, ptr) {
            assert(streq(ptr->data.first, strs_sorted[counter]));
            assert(ptr->data.second == ints_sorted[counter--]);
        }
        assert(counter == -1);
    }

    {
        char *test_str = "20384";

        MapEntry_strv_int *e = map_find(strv_int, m, test_str);
        assert(e != NULL);
        map_remove_entry(strv_int, m, e);
        e = map_find(strv_int, m, test_str);
        assert(e == NULL);

        test_str = "3098";
        map_remove_key(strv_int, m, test_str);
        e = map_find(strv_int, m, test_str);
        assert(e == NULL);

        assert(map_find(strv_int, m, "0") == NULL);
        int oldCount = map_size(m);
        map_remove_key(strv_int, m, "0");
        assert(oldCount == map_size(m));
    }
    map_free(strv_int, m);
}

void test_insert_find_string(void) {
    Map_strv_int *m = map_new(strv_int);
    MapEntry_strv_int *it = NULL;

    for (int i = 0; i <= 10; ++i) {
        map_insert(strv_int, m, pair_make(strv_int, words[i], i));
        assert(map_size(m) == i + 1);
    }
    assert(!map_empty(m));

    for (int i = 0; i <= 10; ++i) {
        it = map_find(strv_int, m, words[i]);
        assert(it != NULL);
        assert(it->data.second == i);
    }

    it = map_find(strv_int, m, "hi");
    assert(it == NULL);

    *map_at(strv_int, m, "10") = 58;
    it = map_find(strv_int, m, "10");
    assert(it->data.second == 58);

    map_clear(strv_int, m);
    assert(map_size(m) == 0);
    assert(map_empty(m));

    map_free(strv_int, m);
}

void test_insert_find_int(void) {
    Map_int_str *m = map_new(int_str);
    MapEntry_int_str *it = NULL;

    for (int i = 0; i <= 10; ++i) {
        map_insert(int_str, m, pair_make(int_str, i, words[i]));
        assert(map_size(m) == i + 1);
    }

    for (int i = 0; i <= 10; ++i) {
        it = map_find(int_str, m, i);
        assert(streq(it->data.second, words[i]));
    }

    *map_at(int_str, m, 3) = "Hello";
    it = map_find(int_str, m, 3);
    assert(streq(it->data.second, "Hello"));

    map_clear(int_str, m);
    assert(map_size(m) == 0);
    assert(map_empty(m));

    map_free(int_str, m);
}

void test_arr_init_and_insert(void) {
    Pair_strv_int arr[11] = {};
    for (int i = 0; i <= 10; ++i) {
        arr[i] = pair_make(strv_int, words[i], i);
    }
    Map_strv_int *m = map_new_fromArray(strv_int, arr, 11);
    assert(m->size == 11);
    MapEntry_strv_int *it = NULL;
    int counter = 0;
    map_iter(strv_int, m, it) {
        MapEntry_strv_int *found = map_find(strv_int, m, words[counter]);
        assert(found);
        assert(found->data.second == counter++);
    }
    assert(counter == 11);

    for (int i = 0; i <= 10; ++i) { arr[i].first = words[50 + i]; }
    map_insert_fromArray(strv_int, m, arr, 11);
    assert(m->size == 22);

    for (int i = 0; i <= 10; ++i) {
        MapEntry_strv_int *found = map_find(strv_int, m, words[50 + i]);
        assert(found);
        assert(found->data.second == i);
    }

    counter = 0;
    map_iter(strv_int, m, it) {
        assert(it);
        ++counter;
    }
    assert(counter == 22);

    map_free(strv_int, m);
}

void test_deletion(void) {
    Map_strp_int *m = map_new(strp_int);

    for (int i = 0; i < NWORDS; ++i) {
        map_insert(strp_int, m, pair_make(strp_int, words[i], i));
    }

    MapEntry_strp_int *it = NULL;

    for (int i = 0; i < NWORDS; ++i) {
        it = map_find(strp_int, m, words[i]);
        assert(it->data.second == i);
    }

    {
        int key = NWORDS - 1;
        while (!map_empty(m)) {
            map_remove_key(strp_int, m, words[key]);
            assert(map_size(m) == key);
            it = map_find(strp_int, m, words[key]);
            assert(it == NULL);
            key--;
        }
        assert(map_size(m) == 0);
        assert(map_empty(m));
    }
    map_free(strp_int, m);
}

void test_erase_range(void) {
    char keys[10][4] = {"a","ba","bb","ca","cd","ce","cf","da","ea","fa"};
    Pair_strv_int arr[10] = {};
    for (int i = 0; i < 10; ++i) {
        arr[i] = pair_make(strv_int, keys[i], i);
    }
    Map_strv_int *m = map_new_fromArray(strv_int, arr, 10);
    assert(map_size(m) == 10);

    map_remove_key(strv_int, m, "ca");
    assert(map_size(m) == 9);

    map_remove_key(strv_int, m, "ca");
    assert(map_size(m) == 9);

    {
        MapEntry_strv_int *begin = map_find(strv_int, m, "a");
        MapEntry_strv_int *end = map_find(strv_int, m, "bb");

        map_erase(strv_int, m, begin, end);
        assert(map_size(m) == 7);

        begin = map_find(strv_int, m, "cf");
        end = NULL;
        map_erase(strv_int, m, begin, end);
        assert(map_size(m) == 3);
    }

    {
        Pair_strv_int comparison[] = {pair_make(strv_int, "bb", 2), pair_make(strv_int, "cd", 4), pair_make(strv_int, "ce", 5)};
        MapEntry_strv_int *it;
        int i = 0;
        map_iter(strv_int, m, it) {
            assert(streq(it->data.first, comparison[i].first));
            assert(it->data.second == comparison[i++].second);
        }
    }
    map_free(strv_int, m);
}

void test_nested_dicts(void) {
    Map_nested *m = map_new(nested);

    for (int i = 0; i < 10; ++i) {
        int success = 0;
        Map_strv_int *inner = map_new(strv_int);
        for (int j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            map_insert(strv_int, inner, pair_make(strv_int, words[value], value));
        }
        assert(map_size(inner) == 10);
        map_insert_withResult(nested, m, pair_make(nested, words[i], inner), &success);
        assert(success);
        assert(map_size(m) == i + 1);
    }

    MapEntry_nested *it = NULL;

    for (int i = 0; i < 10; ++i) {
        it = map_find(nested, m, words[i]);
        assert(it);
        Map_strv_int *inner = it->data.second;
        for (int j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            MapEntry_strv_int *ptr = map_find(strv_int, inner, words[value]);
            assert(ptr);
            assert(ptr->data.second == value);
        }
    }

    int counter = 0;
    map_iter(nested, m, it) {
        MapEntry_strv_int *jt = NULL;
        map_iter(strv_int, it->data.second, jt) {
            counter += 1;
        }
    }
    assert(counter == 100);
    map_free(nested, m);
}

int main(void) {
    for (int i = 0; i < NWORDS; ++i) {
        snprintf(words[i], 16, "%d", i);
    }
    test_basic_rbtree_ints();
    test_basic_rbtree_strs();
    test_insert_find_string();
    test_insert_find_int();
    test_arr_init_and_insert();
    test_deletion();
    test_erase_range();
    test_nested_dicts();
    return 0;
}
