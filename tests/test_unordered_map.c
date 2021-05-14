#include "unordered_map.h"
#include <assert.h>

char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August",
                  "September", "October", "November", "December"};

char *words[] = {"cellar", "fold", "criminal", "strict", "trend", "assume", "union", "tropical",
                 "abundant", "fraction", "key", "me", "freighter", "wrong", "investigation",
                 "construct", "colleague", "formulate", "simplicity", "fever", "discipline",
                 "genetic", "shorts", "understand", "recording", "hobby", "prove", "bucket",
                 "revenge", "ego", "tablet", "minute", "teenager", "crevice", "sympathetic",
                 "crutch", "rib", "confession", "right", "star", "fun", "exclude", "senior",
                 "suspect", "reliable", "harm", "consciousness", "loud", "necklace", "elaborate",
                 "desk", "plagiarize", "restless", "middle", "federation", "mosque", "sweet", "red",
                 "bring", "lemon", "thick", "unlikely", "store", "plot", "census", "prevent",
                 "paralyzed", "movie", "winner", "mild", "foundation", "exploit", "dismiss", "die",
                 "owl", "studio", "represent", "feminine", "school", "mail", "disaster", "salmon",
                 "vegetation", "stage", "hate", "ear", "staff", "enjoy", "arm", "reservoir", "appeal",
                 "scatter", "lounge", "cucumber", "ally", "faithful", "chapter", "frown", "sell"};

gen_umap(strv_int, char *, PTR_COPY, int, ds_cmp_str_lt)
gen_umap(int_str, int, VALUE, char *, ds_cmp_num_lt)
gen_umap(strp_int, char *, PTR, int, ds_cmp_str_lt)

void test_insert_find_string(void) {
    UMap_strv_int *m = umap_new(strv_int);
    assert(umap_empty(m));

    for (int i = 0; i < 7; ++i) {
        umap_insert(strv_int, m, days[i], i);
        assert(umap_size(m) == i + 1);
    }
    assert(!umap_empty(m));
    Pair_strv_int *it = NULL;

    for (int i = 0; i < 7; ++i) {
        it = umap_find(strv_int, m, days[i]);
        assert(it != NULL);
        assert(it->second == i);
    }

    it = umap_find(strv_int, m, "hi");
    assert(it == NULL);

    *umap_at(strv_int, m, "Wednesday") = 58;
    it = umap_find(strv_int, m, "Wednesday");
    assert(it->second == 58);

    umap_clear(strv_int, m);
    assert(umap_size(m) == 0);
    assert(umap_empty(m));

    umap_free(strv_int, m);
}

void test_insert_find_int(void) {
    UMap_int_str *m = umap_new(int_str);
    Pair_int_str *it = NULL;

    for (int i = 0; i < 12; ++i) {
        umap_insert(int_str, m, i, months[i]);
        assert(umap_size(m) == i + 1);
    }

    for (int i = 0; i < 12; ++i) {
        it = umap_find(int_str, m, i);
        assert(streq(it->second, months[i]));
    }

    *umap_at(int_str, m, 3) = "Hello";
    it = umap_find(int_str, m, 3);
    assert(streq(it->second, "Hello"));

    umap_clear(int_str, m);
    assert(umap_size(m) == 0);
    assert(umap_empty(m));

    umap_free(int_str, m);
}

void test_resizing_deletion(void) {
    UMap_strp_int *m = umap_new(strp_int);

    const int len = 99;
    for (int i = 0; i < len; ++i) {
        umap_insert(strp_int, m, words[i], i);
    }

    assert(m->cap != 32);
    Pair_strp_int *it = NULL;

    for (int i = 0; i < len; ++i) {
        it = umap_find(strp_int, m, words[i]);
        assert(it->second == i);
    }

    int key = len - 1;
    while (!umap_empty(m)) {
        umap_erase_key(strp_int, m, words[key]);
        assert(umap_size(m) == key);
        it = umap_find(strp_int, m, words[key]);
        assert(it == NULL);
        key--;
    }

    assert(umap_size(m) == 0);

    umap_free(strp_int, m);
}

void test_iter(void) {
    UMap_int_str *m = umap_new(int_str);

    for (int i = 0; i < 12; ++i) {
        umap_insert(int_str, m, i, months[i]);
        assert(umap_size(m) == i + 1);
    }
    int counter = 0;
    Pair_int_str *it = NULL;

    umap_iter(int_str, m, it) {
        assert(it != NULL);
        assert(strlen(it->second) > 0);
        ++counter;
    }
    assert(counter == 12);

    umap_free(int_str, m);
}

int main(void) {
    test_insert_find_string();
    test_insert_find_int();
    test_resizing_deletion();
    test_iter();
    return 0;
}
