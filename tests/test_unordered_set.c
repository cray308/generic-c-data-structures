#include "unordered_set.h"
#include <assert.h>

gen_uset_headers(int, int)
gen_uset_headers(str, char *)
gen_uset_source(int, int, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_uset_source(str, char *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete)

char *strs[] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049"};
int ints[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};

typedef struct {
    int i;
    int found;
} IntData;

typedef struct {
    char *s;
    int found;
} StrData;

int search_int(const void *a, const void *b) {
    int x = ((IntData *) a)->i, y = ((IntData *) b)->i;
    return x < y ? -1 : (x > y ? 1 : 0);
}

int search_str(const void *a, const void *b) {
    return strcmp(((StrData *) a)->s, ((StrData *) b)->s);
}

void compare_ints(USet_int *s, IntData *comparison, unsigned size) {
    unsigned i = 0;
    int *it;
    IntData data, *found;
    assert(uset_size(s) == size);
    if (size) {
        assert(!uset_empty(s));
    } else {
        assert(uset_empty(s));
    }
    for (i = 0; i < size; ++i) {
        comparison[i].found = 0;
    }
    i = 0;
    uset_iter(int, s, it) {
        data.i = *it;
        found = bsearch(&data, comparison, (size_t) size, sizeof(IntData), search_int);
        assert(found && found->i == *it);
        assert(found->found == 0);
        found->found = 1;
        ++i;
    }
    assert(i == size);
    for (i = 0; i < size; ++i) {
        assert(comparison[i].found);
    }
}

void compare_strs(USet_str *s, StrData *comparison, unsigned size) {
    unsigned i = 0;
    char **it;
    StrData data, *found;
    assert(uset_size(s) == size);
    if (size) {
        assert(!uset_empty(s));
    } else {
        assert(uset_empty(s));
    }
    for (i = 0; i < size; ++i) {
        comparison[i].found = 0;
    }
    i = 0;
    uset_iter(str, s, it) {
        data.s = *it;
        found = bsearch(&data, comparison, (size_t) size, sizeof(StrData), search_str);
        assert(found && streq(found->s, *it));
        assert(found->found == 0);
        found->found = 1;
        ++i;
    }
    assert(i == size);
    for (i = 0; i < size; ++i) {
        assert(comparison[i].found);
    }
}

void test_empty_init(void) {
    IntData c1[] = {{0}};
    StrData c2[] = {{0}};
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new(str);
    compare_ints(si, c1, 0);
    compare_strs(ss, c2, 0);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_init_fromArray(void) {
    USet_int *si = uset_new_fromArray(int, NULL, 5);
    USet_str *ss;
    int i;
    IntData c1[50] = {0};
    StrData c2[50] = {0};
    for (i = 0; i < 50; ++i) {
        c1[i].i = i;
        c2[i].s = strs[i];
    }
    compare_ints(si, c1, 0);
    uset_free(int, si);
    si = uset_new_fromArray(int, ints, 0);
    compare_ints(si, c1, 0);
    uset_free(int, si);
    si = uset_new_fromArray(int, ints, 1);
    ss = uset_new_fromArray(str, strs, 1);
    compare_ints(si, c1, 1);
    compare_strs(ss, c2, 1);
    uset_free(int, si);
    uset_free(str, ss);
    si = uset_new_fromArray(int, ints, 50);
    ss = uset_new_fromArray(str, strs, 50);
    compare_ints(si, c1, 50);
    compare_strs(ss, c2, 50);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_createCopy(void) {
    USet_int *b = uset_new_fromArray(int, NULL, 0), *si;
    USet_str *c, *ss;
    int i;
    IntData c1[50] = {0};
    StrData c2[50] = {0};
    for (i = 0; i < 50; ++i) {
        c1[i].i = i;
        c2[i].s = strs[i];
    }
    si = uset_createCopy(int, b);
    compare_ints(si, c1, 0);
    uset_free(int, si);
    uset_free(int, b);
    b = uset_new_fromArray(int, ints, 50);
    c = uset_new_fromArray(str, strs, 50);
    si = uset_createCopy(int, b);
    ss = uset_createCopy(str, c);
    uset_free(int, b);
    uset_free(str, c);
    compare_ints(si, c1, 50);
    compare_strs(ss, c2, 50);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_insert_element(void) {
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new(str);
    IntData c1[] = {{10,0},{20,0},{30,0},{40,0},{50,0}};
    StrData c2[] = {{"010",0},{"020",0},{"030",0},{"040",0},{"050",0}};
    int a1[] = {50,40,30,20,10}, i, inserted = -1;
    char *a2[] = {"050","040","030","020","010"};

    for (i = 0; i < 5; ++i) {
        uset_insert_withResult(int, si, a1[i], &inserted);
        assert(inserted);
        inserted = -1;
        uset_insert_withResult(str, ss, a2[i], &inserted);
        assert(inserted);
        inserted = -1;
    }
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);

    uset_insert_withResult(int, si, 40, &inserted);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(str, ss, "040", &inserted);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(int, si, 10, &inserted);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(str, ss, "010", &inserted);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(int, si, 50, &inserted);
    assert(!inserted);
    inserted = -1;
    uset_insert_withResult(str, ss, "050", &inserted);
    assert(!inserted);
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_insert_fromArray(void) {
    int a1[7] = {50,50,40,30,20,10,30};
    char *a2[7] = {"050","050","040","030","020","010","030"};
    IntData c1[] = {{10,0},{20,0},{30,0},{40,0},{50,0}};
    StrData c2[] = {{"010",0},{"020",0},{"030",0},{"040",0},{"050",0}};
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new(str);

    uset_insert_fromArray(int, si, NULL, 5);
    uset_insert_fromArray(int, si, ints, 0);
    compare_ints(si, c1, 0);
    uset_insert_fromArray(int, si, a1, 7);
    uset_insert_fromArray(str, ss, a2, 7);
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_remove_value(void) {
    int a1[] = {50,40,30,20,10}, removedInts[] = {0,60,39,41,40,10,50,40}, i;
    char *a2[] = {"050","040","030","020","010"}, *removedStrs[] = {"000","060","039","041","040","010","050","040"};
    IntData c1[] = {{20,0},{30,0}};
    StrData c2[] = {{"020",0},{"030",0}};
    USet_int *si = uset_new_fromArray(int, a1, 5);
    USet_str *ss = uset_new_fromArray(str, a2, 5);
    for (i = 0; i < 8; ++i) {
        uset_remove(int, si, removedInts[i]);
        uset_remove(str, ss, removedStrs[i]);
    }
    compare_ints(si, c1, 2);
    compare_strs(ss, c2, 2);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_find(void) {
    int a1[] = {100,90,80,70,60,50,40,30,20,10};
    char *a2[] = {"100","090","080","070","060","050","040","030","020","010"};
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new_fromArray(str, a2, 10);

    assert(!uset_contains(int, si, 0));
    uset_insert_fromArray(int, si, a1, 10);
    assert(!uset_contains(int, si, 9));
    assert(!uset_contains(str, ss, "009"));
    assert(!uset_contains(int, si, 101));
    assert(!uset_contains(str, ss, "101"));
    assert(!uset_contains(int, si, 69));
    assert(!uset_contains(str, ss, "069"));
    assert(!uset_contains(int, si, 71));
    assert(!uset_contains(str, ss, "071"));

    assert(uset_contains(int, si, 70));
    assert(uset_contains(str, ss, "070"));
    assert(uset_contains(int, si, 60));
    assert(uset_contains(str, ss, "060"));
    assert(uset_contains(int, si, 80));
    assert(uset_contains(str, ss, "080"));
    assert(uset_contains(int, si, 10));
    assert(uset_contains(str, ss, "010"));
    assert(uset_contains(int, si, 100));
    assert(uset_contains(str, ss, "100"));
    uset_free(int, si);
    uset_free(str, ss);
}

void test_set_load_factor(void) {
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new(str);
    IntData c1[50] = {0};
    StrData c2[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        c1[i].i = i;
        c2[i].s = strs[i];
    }
    assert(uset_max_load_factor(si) == 0.75);
    assert(uset_max_load_factor(ss) == 0.75);
    uset_set_load_factor(int, si, 0.25);
    uset_set_load_factor(str, ss, 0.25);
    assert(uset_max_load_factor(si) == 0.75);
    assert(uset_max_load_factor(ss) == 0.75);
    uset_set_load_factor(int, si, 1.25);
    uset_set_load_factor(str, ss, 1.25);
    assert(uset_max_load_factor(si) == 0.75);
    assert(uset_max_load_factor(ss) == 0.75);

    uset_insert_fromArray(int, si, ints, 50);
    uset_insert_fromArray(str, ss, strs, 50);
    assert(uset_max_load_factor(si) == 0.75);
    assert(uset_max_load_factor(ss) == 0.75);
    uset_set_load_factor(int, si, 1);
    uset_set_load_factor(str, ss, 1);
    assert(uset_max_load_factor(si) == 1.0);
    assert(uset_max_load_factor(ss) == 1.0);
    compare_ints(si, c1, 50);
    compare_strs(ss, c2, 50);
    uset_set_load_factor(int, si, 0.5);
    uset_set_load_factor(str, ss, 0.5);
    assert(uset_max_load_factor(si) == 0.5);
    assert(uset_max_load_factor(ss) == 0.5);
    compare_ints(si, c1, 50);
    compare_strs(ss, c2, 50);
    uset_free(int, si);
    uset_free(str, ss);
}

void test_rehash(void) {
    USet_int *si = uset_new(int);
    USet_str *ss = uset_new(str);
    IntData c1[50] = {0};
    StrData c2[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        c1[i].i = i;
        c2[i].s = strs[i];
    }
    assert(uset_bucket_count(si) == 32);
    assert(uset_bucket_count(ss) == 32);
    uset_rehash(int, si, 16);
    uset_rehash(str, ss, 16);
    assert(uset_bucket_count(si) == 32);
    assert(uset_bucket_count(ss) == 32);
    uset_rehash(int, si, 33);
    uset_rehash(str, ss, 33);
    assert(uset_bucket_count(si) == 64);
    assert(uset_bucket_count(ss) == 64);

    uset_insert_fromArray(int, si, ints, 50);
    uset_insert_fromArray(str, ss, strs, 50);
    assert(uset_bucket_count(si) == 128);
    assert(uset_bucket_count(ss) == 128);
    uset_rehash(int, si, 1024);
    uset_rehash(str, ss, 1024);
    assert(uset_bucket_count(si) == 1024);
    assert(uset_bucket_count(ss) == 1024);
    compare_ints(si, c1, 50);
    compare_strs(ss, c2, 50);
    uset_free(int, si);
    uset_free(str, ss);
}

int main(void) {
    test_empty_init();
    test_init_fromArray();
    test_createCopy();
    test_insert_element();
    test_insert_fromArray();
    test_remove_value();
    test_find();
    test_set_load_factor();
    test_rehash();
    return 0;
}
