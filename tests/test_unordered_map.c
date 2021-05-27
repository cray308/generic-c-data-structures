#include "unordered_map.h"
#include <assert.h>

#define LEN 100

char *strs[LEN] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049","050","051","052",
"053","054","055","056","057","058","059","060","061","062","063","064","065","066","067","068","069","070","071",
"072","073","074","075","076","077","078","079","080","081","082","083","084","085","086","087","088","089","090",
"091","092","093","094","095","096","097","098","099"};

gen_umap(strv_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(int_str, int, char *, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(strp_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap(nested, char *, UMap_strv_int *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, __htable_free_strv_int)

void test_insert_find_string(void) {
    int i, rv = -1;
    Pair_strv_int entry = {"005", 256}, *it;
    UMap_strv_int *m = umap_new(strv_int);
    assert(umap_empty(m));

    for (i = 0; i <= 10; ++i) {
        Pair_strv_int p;
        p.first = strs[i], p.second = i;
        umap_insert(strv_int, m, p);
        assert(umap_size(m) == i + 1);
    }
    assert(!umap_empty(m));

    for (i = 0; i <= 10; ++i) {
        it = umap_find(strv_int, m, strs[i]);
        assert(it && it->second == i);
    }

    assert(umap_find(strv_int, m, "hi") == NULL);

    *umap_at(strv_int, m, "010") = 58;
    it = umap_find(strv_int, m, "010");
    assert(it->second == 58);

    it = umap_insert_withResult(strv_int, m, entry, &rv);
    assert(streq(it->first, "005"));
    assert(it->second == 256);
    assert(rv == 0);

    umap_clear(strv_int, m);
    assert(umap_size(m) == 0);
    assert(umap_empty(m));
    umap_free(strv_int, m);
}

void test_insert_find_int(void) {
    int i;
    UMap_int_str *m = umap_new(int_str); Pair_int_str *it;

    for (i = 0; i <= 10; ++i) {
        Pair_int_str p;
        p.first = i, p.second = strs[i];
        umap_insert(int_str, m, p);
        assert(umap_size(m) == i + 1);
    }
    for (i = 0; i <= 10; ++i) {
        it = umap_find(int_str, m, i);
        assert(streq(it->second, strs[i]));
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
    int i;
    Pair_strv_int arr[11], *it; UMap_strv_int *m;
    for (i = 0; i <= 10; ++i) {
        arr[i].first = strs[i], arr[i].second = i;
    }
    m = umap_new_fromArray(strv_int, arr, 11);
    assert(m->size == 11);
    i = 0;
    umap_iter(strv_int, m, it) {
        Pair_strv_int *found = umap_find(strv_int, m, strs[i]);
        assert(found && found->second == i++);
    }
    assert(i == 11);

    for (i = 0; i <= 10; ++i) { arr[i].first = strs[50 + i]; }
    umap_insert_fromArray(strv_int, m, arr, 11);
    assert(m->size == 22);

    for (i = 0; i <= 10; ++i) {
        Pair_strv_int *found = umap_find(strv_int, m, strs[50 + i]);
        assert(found);
        assert(found->second == i);
    }

    i = 0;
    umap_iter(strv_int, m, it) {
        assert(it);
        ++i;
    }
    assert(i == 22);
    umap_free(strv_int, m);
}

void test_create_copy(void) {
    int i;
    Pair_strv_int arr[11], *it; UMap_strv_int *m, *second;
    for (i = 0; i <= 10; ++i) {
        arr[i].first = strs[i], arr[i].second = i;
    }
    m = umap_new_fromArray(strv_int, arr, 11);
    second = umap_createCopy(strv_int, m);
    assert(second->size == 11);
    i = 0;
    umap_iter(strv_int, second, it) {
        Pair_strv_int *found = umap_find(strv_int, second, strs[i]);
        assert(found);
        assert(found->second == i++);
    }
    assert(i == 11);
    umap_free(strv_int, second); umap_free(strv_int, m);
}

void test_resizing_deletion(void) {
    int i;
    Pair_strp_int *it = NULL;
    UMap_strp_int *m = umap_new(strp_int);

    for (i = 0; i < LEN; ++i) {
        Pair_strp_int p;
        p.first = strs[i], p.second = i;
        umap_insert(strp_int, m, p);
    }

    assert(m->cap != 32);
    {
        double oldLoadFactor = umap_max_load_factor(m);
        size_t oldCap = m->cap;
        umap_rehash(strp_int, m, oldCap);
        assert(m->cap == oldCap);
        umap_set_load_factor(strp_int, m, 0.5);
        assert(oldLoadFactor != umap_max_load_factor(m));
    }

    for (i = 0; i < LEN; ++i) {
        it = umap_find(strp_int, m, strs[i]);
        assert(it->second == i);
    }

    i = LEN - 1;
    while (!umap_empty(m)) {
        umap_remove_key(strp_int, m, strs[i]);
        assert(umap_size(m) == i);
        it = umap_find(strp_int, m, strs[i]);
        assert(it == NULL);
        i--;
    }
    assert(umap_size(m) == 0);
    umap_free(strp_int, m);
}

void test_iter(void) {
    int i;
    Pair_int_str *it;
    UMap_int_str *m = umap_new(int_str);

    for (i = 0; i <= 10; ++i) {
        Pair_int_str p;
        p.first = i, p.second = strs[i];
        umap_insert(int_str, m, p);
        assert(umap_size(m) == i + 1);
    }

    i = 0;
    umap_iter(int_str, m, it) {
        assert(it != NULL);
        assert(strlen(it->second) > 0);
        ++i;
    }
    assert(i == 11);
    umap_free(int_str, m);
}

void test_nested_dicts(void) {
    int i;
    UMap_nested *m = umap_new(nested); Pair_nested *it;

    for (i = 0; i < 10; ++i) {
        Pair_nested outer;
        int success = 0, j;
        UMap_strv_int *inner = umap_new(strv_int);
        for (j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            Pair_strv_int p;
            p.first = strs[value], p.second = value;
            umap_insert(strv_int, inner, p);
        }
        assert(umap_size(inner) == 10);
        outer.first = strs[i], outer.second = inner;
        umap_insert_withResult(nested, m, outer, &success);
        assert(success);
        assert(umap_size(m) == i + 1);
    }

    for (i = 0; i < 10; ++i) {
        UMap_strv_int *inner;
        int j;
        it = umap_find(nested, m, strs[i]);
        assert(it);
        inner = it->second;
        for (j = 0; j < 10; ++j) {
            int value = (i * 10) + j;
            Pair_strv_int *ptr = umap_find(strv_int, inner, strs[value]);
            assert(ptr);
            assert(ptr->second == value);
        }
    }

    i = 0;
    umap_iter(nested, m, it) {
        Pair_strv_int *jt;
        umap_iter(strv_int, it->second, jt) {
            i += 1;
        }
    }
    assert(i == 100);
    umap_free(nested, m);
}

int main(void) {
    test_insert_find_string();
    test_insert_find_int();
    test_arr_init_and_insert();
    test_create_copy();
    test_resizing_deletion();
    test_iter();
    test_nested_dicts();
    return 0;
}
