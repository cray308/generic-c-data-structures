#include "unordered_map.h"
#include <assert.h>

#define NWORDS 100

char words[NWORDS][16] = {0};

gen_umap(strv_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(int_str, int, char *, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(strp_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(nested, char *, UMap_strv_int *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, __htable_free_strv_int)

void test_insert_find_string(void) {
    UMap_strv_int *m = umap_new(strv_int);
    assert(umap_empty(m));

    for (int i = 0; i <= 10; ++i) {
        umap_insert(strv_int, m, pair_make(strv_int, words[i], i));
        assert(umap_size(m) == i + 1);
    }
    assert(!umap_empty(m));
    Pair_strv_int *it = NULL;

    for (int i = 0; i <= 10; ++i) {
        it = umap_find(strv_int, m, words[i]);
        assert(it != NULL);
        assert(it->second == i);
    }

    it = umap_find(strv_int, m, "hi");
    assert(it == NULL);

    *umap_at(strv_int, m, "10") = 58;
    it = umap_find(strv_int, m, "10");
    assert(it->second == 58);

    umap_clear(strv_int, m);
    assert(umap_size(m) == 0);
    assert(umap_empty(m));

    umap_free(strv_int, m);
}

void test_insert_find_int(void) {
    UMap_int_str *m = umap_new(int_str);
    Pair_int_str *it = NULL;

    for (int i = 0; i <= 10; ++i) {
        umap_insert(int_str, m, pair_make(int_str, i, words[i]));
        assert(umap_size(m) == i + 1);
    }

    for (int i = 0; i <= 10; ++i) {
        it = umap_find(int_str, m, i);
        assert(streq(it->second, words[i]));
    }

    *umap_at(int_str, m, 3) = "Hello";
    it = umap_find(int_str, m, 3);
    assert(streq(it->second, "Hello"));

    umap_clear(int_str, m);
    assert(umap_size(m) == 0);
    assert(umap_empty(m));

    umap_free(int_str, m);
}

void test_arr_init_and_insert(void) {
    Pair_strv_int arr[11] = {};
    for (int i = 0; i <= 10; ++i) {
        arr[i] = pair_make(strv_int, words[i], i);
    }
    UMap_strv_int *m = umap_new_fromArray(strv_int, arr, 11);
    assert(m->size == 11);
    Pair_strv_int *it = NULL;
    int counter = 0;
    umap_iter(strv_int, m, it) {
        Pair_strv_int *found = umap_find(strv_int, m, words[counter]);
        assert(found);
        assert(found->second == counter++);
    }
    assert(counter == 11);

    for (int i = 0; i <= 10; ++i) { arr[i].first = words[50 + i]; }
    umap_insert_fromArray(strv_int, m, arr, 11);
    assert(m->size == 22);

    for (int i = 0; i <= 10; ++i) {
        Pair_strv_int *found = umap_find(strv_int, m, words[50 + i]);
        assert(found);
        assert(found->second == i);
    }

    counter = 0;
    umap_iter(strv_int, m, it) {
        assert(it);
        ++counter;
    }
    assert(counter == 22);

    umap_free(strv_int, m);
}

void test_resizing_deletion(void) {
    UMap_strp_int *m = umap_new(strp_int);

    for (int i = 0; i < NWORDS; ++i) {
        umap_insert(strp_int, m, pair_make(strp_int, words[i], i));
    }

    assert(m->cap != 32);
    Pair_strp_int *it = NULL;

    for (int i = 0; i < NWORDS; ++i) {
        it = umap_find(strp_int, m, words[i]);
        assert(it->second == i);
    }

    {
        int key = NWORDS - 1;
        while (!umap_empty(m)) {
            umap_remove_key(strp_int, m, words[key]);
            assert(umap_size(m) == key);
            it = umap_find(strp_int, m, words[key]);
            assert(it == NULL);
            key--;
        }
        assert(umap_size(m) == 0);
    }
    umap_free(strp_int, m);
}

void test_iter(void) {
    UMap_int_str *m = umap_new(int_str);

    for (int i = 0; i <= 10; ++i) {
        umap_insert(int_str, m, pair_make(int_str, i, words[i]));
        assert(umap_size(m) == i + 1);
    }

    {
        int counter = 0;
        Pair_int_str *it;
        umap_iter(int_str, m, it) {
            assert(it != NULL);
            assert(strlen(it->second) > 0);
            ++counter;
        }
        assert(counter == 11);
    }
    umap_free(int_str, m);
}

void test_nested_dicts(void) {
    UMap_nested *m = umap_new(nested);

    for (int i = 0; i < 10; ++i) {
        int success = 0;
        UMap_strv_int *inner = umap_new(strv_int);
        for (int j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            umap_insert(strv_int, inner, pair_make(strv_int, words[value], value));
        }
        assert(umap_size(inner) == 10);
        umap_insert_withResult(nested, m, pair_make(nested, words[i], inner), &success);
        assert(success);
        assert(umap_size(m) == i + 1);
    }

    Pair_nested *it = NULL;

    for (int i = 0; i < 10; ++i) {
        it = umap_find(nested, m, words[i]);
        assert(it);
        UMap_strv_int *inner = it->second;
        for (int j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            Pair_strv_int *ptr = umap_find(strv_int, inner, words[value]);
            assert(ptr);
            assert(ptr->second == value);
        }
    }

    int counter = 0;
    umap_iter(nested, m, it) {
        Pair_strv_int *jt = NULL;
        umap_iter(strv_int, it->second, jt) {
            counter += 1;
        }
    }
    assert(counter == 100);
    umap_free(nested, m);
}

int main(void) {
    for (int i = 0; i < NWORDS; ++i) {
        snprintf(words[i], 16, "%d", i);
    }
    test_insert_find_string();
    test_insert_find_int();
    test_arr_init_and_insert();
    test_resizing_deletion();
    test_iter();
    test_nested_dicts();
    return 0;
}
