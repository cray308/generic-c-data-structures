#include "unordered_map.h"
#ifndef __CDS_SCAN
#include <assert.h>
#endif

#define freeNestedMap(x) umap_free(strv_int, x)

gen_umap_headers(strv_int, char *, int)
gen_umap_headers(int_str, int, char *)
gen_umap_headers(strp_int, char *, int)
gen_umap_headers(nested, char *, UMap_strv_int *)

gen_umap_source(strv_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap_source(int_str, int, char *, ds_cmp_num_eq, DSDefault_addrOfVal, DSDefault_sizeOfVal, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap_source(strp_int, char *, int, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_umap_source(nested, char *, UMap_strv_int *, ds_cmp_str_eq, DSDefault_addrOfRef, DSDefault_sizeOfStr, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, freeNestedMap)

char *strs[] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049"};

typedef struct {
    int i;
    char *s;
    int found;
} DictData;

typedef struct {
    char *s;
    DictData *data;
    int found;
} NestedDictData;

int search_int(const void *a, const void *b) {
    int x = ((DictData *) a)->i, y = ((DictData *) b)->i;
    return x < y ? -1 : (x > y ? 1 : 0);
}

int search_str(const void *a, const void *b) {
    return strcmp(((DictData *) a)->s, ((DictData *) b)->s);
}

void compare_int_str(UMap_int_str *m, DictData *comparison, unsigned size) {
    unsigned i = 0;
    char **at;
    Pair_int_str *it;
    DictData data, *found;
    assert(umap_size(m) == size);
    if (size) {
        assert(!umap_empty(m));
    } else {
        assert(umap_empty(m));
    }
    for (i = 0; i < size; ++i) {
        comparison[i].found = 0;
    }
    i = 0;
    umap_iter(int_str, m, it) {
        data.i = it->first;
        found = bsearch(&data, comparison, (size_t) size, sizeof(DictData), search_int);
        assert(found && found->i == it->first);
        assert(found->found == 0);
        at = umap_at(int_str, m, it->first);
        assert(at && streq(*at, found->s));
        assert(streq(it->second, found->s));
        found->found = 1;
        ++i;
    }
    assert(i == size);
    for (i = 0; i < size; ++i) {
        assert(comparison[i].found);
    }
}

void compare_strv_int(UMap_strv_int *m, DictData *comparison, unsigned size) {
    unsigned i = 0;
    int *at;
    Pair_strv_int *it;
    DictData data, *found;
    assert(umap_size(m) == size);
    if (size) {
        assert(!umap_empty(m));
    } else {
        assert(umap_empty(m));
    }
    for (i = 0; i < size; ++i) {
        comparison[i].found = 0;
    }
    i = 0;
    umap_iter(strv_int, m, it) {
        data.s = it->first;
        found = bsearch(&data, comparison, (size_t) size, sizeof(DictData), search_str);
        assert(found && streq(found->s, it->first));
        assert(found->found == 0);
        at = umap_at(strv_int, m, it->first);
        assert(at && *at == found->i);
        assert(it->second == found->i);
        found->found = 1;
        ++i;
    }
    assert(i == size);
    for (i = 0; i < size; ++i) {
        assert(comparison[i].found);
    }
}

void compare_strp_int(UMap_strp_int *m, DictData *comparison, unsigned size) {
    unsigned i = 0;
    int *at;
    Pair_strp_int *it;
    DictData data, *found;
    assert(umap_size(m) == size);
    if (size) {
        assert(!umap_empty(m));
    } else {
        assert(umap_empty(m));
    }
    for (i = 0; i < size; ++i) {
        comparison[i].found = 0;
    }
    i = 0;
    umap_iter(strp_int, m, it) {
        data.s = it->first;
        found = bsearch(&data, comparison, (size_t) size, sizeof(DictData), search_str);
        assert(found && streq(found->s, it->first));
        assert(found->found == 0);
        at = umap_at(strp_int, m, it->first);
        assert(at && *at == found->i);
        assert(it->second == found->i);
        found->found = 1;
        ++i;
    }
    assert(i == size);
    for (i = 0; i < size; ++i) {
        assert(comparison[i].found);
    }
}

void test_empty_init(void) {
    DictData c[] = {{0,"000",0}};
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new(strv_int);
    UMap_strp_int *m3 = umap_new(strp_int);
    compare_int_str(m1, c, 0);
    compare_strv_int(m2, c, 0);
    compare_strp_int(m3, c, 0);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_init_fromArray(void) {
    DictData c1[] = {{0,"000",0}};
    UMap_int_str *m1 = umap_new_fromArray(int_str, NULL, 5);
    UMap_strv_int *m2;
    UMap_strp_int *m3;

    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    DictData data[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = i;
        arrInt[i].second = strs[i];
        arrStr1[i].first = strs[i];
        arrStr1[i].second = i;
        arrStr2[i].first = strs[i];
        arrStr2[i].second = i;
        data[i].i = i;
        data[i].s = strs[i];
    }
    compare_int_str(m1, data, 0);
    umap_free(int_str, m1);
    m1 = umap_new_fromArray(int_str, arrInt, 0);
    compare_int_str(m1, data, 0);
    umap_free(int_str, m1);

    m1 = umap_new_fromArray(int_str, arrInt, 1);
    m2 = umap_new_fromArray(strv_int, arrStr1, 1);
    m3 = umap_new_fromArray(strp_int, arrStr2, 1);
    compare_int_str(m1, c1, 1);
    compare_strv_int(m2, c1, 1);
    compare_strp_int(m3, c1, 1);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);

    m1 = umap_new_fromArray(int_str, arrInt, 50);
    m2 = umap_new_fromArray(strv_int, arrStr1, 50);
    m3 = umap_new_fromArray(strp_int, arrStr2, 50);
    compare_int_str(m1, data, 50);
    compare_strv_int(m2, data, 50);
    compare_strp_int(m3, data, 50);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_createCopy(void) {
    UMap_int_str *b = umap_new_fromArray(int_str, NULL, 0), *m1;
    UMap_strv_int *c, *m2;
    UMap_strp_int *d, *m3;
    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    DictData data[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = i;
        arrInt[i].second = strs[i];
        arrStr1[i].first = strs[i];
        arrStr1[i].second = i;
        arrStr2[i].first = strs[i];
        arrStr2[i].second = i;
        data[i].i = i;
        data[i].s = strs[i];
    }
    m1 = umap_createCopy(int_str, b);
    compare_int_str(m1, data, 0);
    umap_free(int_str, m1);
    umap_free(int_str, b);
    b = umap_new_fromArray(int_str, arrInt, 50);
    c = umap_new_fromArray(strv_int, arrStr1, 50);
    d = umap_new_fromArray(strp_int, arrStr2, 50);
    m1 = umap_createCopy(int_str, b);
    m2 = umap_createCopy(strv_int, c);
    m3 = umap_createCopy(strp_int, d);
    umap_free(int_str, b);
    umap_free(strv_int, c);
    umap_free(strp_int, d);
    compare_int_str(m1, data, 50);
    compare_strv_int(m2, data, 50);
    compare_strp_int(m3, data, 50);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_insert_element(void) {
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new(strv_int);
    UMap_strp_int *m3 = umap_new(strp_int);
    Pair_int_str x, *p1;
    Pair_strv_int y, *p2;
    Pair_strp_int z, *p3;
    int a1[] = {50,40,30,20,10}, i, inserted = -1;
    char *a2[] = {"050","040","030","020","010"};
    DictData c1[] = {{10,"010",0},{20,"020",0},{30,"030",0},{40,"040",0},{50,"050",0}}, c2[] = {{10,"100",0},{20,"020",0},{30,"030",0},{40,"400",0},{50,"500",0}}, c3[] = {{100,"010",0},{20,"020",0},{30,"030",0},{400,"040",0},{500,"050",0}};

    for (i = 0; i < 5; ++i) {
        x.first = a1[i];
        x.second = a2[i];
        y.first = a2[i];
        y.second = a1[i];
        z.first = a2[i];
        z.second = a1[i];
        p1 = umap_insert_withResult(int_str, m1, x, &inserted);
        assert(p1 && p1->first == x.first && streq(p1->second, x.second));
        assert(inserted);
        inserted = -1;
        p2 = umap_insert_withResult(strv_int, m2, y, &inserted);
        assert(p2 && streq(p2->first, y.first) && p2->second == y.second);
        assert(inserted);
        inserted = -1;
        p3 = umap_insert_withResult(strp_int, m3, z, &inserted);
        assert(p3 && streq(p3->first, z.first) && p3->second == z.second);
        assert(inserted);
        inserted = -1;
    }
    compare_int_str(m1, c1, 5);
    compare_strv_int(m2, c1, 5);
    compare_strp_int(m3, c1, 5);

    x.first = 40;
    x.second = "400";
    y.first = "040";
    y.second = 400;
    z.first = "040";
    z.second = 400;
    p1 = umap_insert_withResult(int_str, m1, x, &inserted);
    assert(p1 && p1->first == x.first && streq(p1->second, x.second));
    assert(!inserted);
    inserted = -1;
    p2 = umap_insert_withResult(strv_int, m2, y, &inserted);
    assert(p2 && streq(p2->first, y.first) && p2->second == y.second);
    assert(!inserted);
    inserted = -1;
    p3 = umap_insert_withResult(strp_int, m3, z, &inserted);
    assert(p3 && streq(p3->first, z.first) && p3->second == z.second);
    assert(!inserted);
    inserted = -1;
    x.first = 10;
    x.second = "100";
    y.first = "010";
    y.second = 100;
    z.first = "010";
    z.second = 100;
    p1 = umap_insert_withResult(int_str, m1, x, &inserted);
    assert(p1 && p1->first == x.first && streq(p1->second, x.second));
    assert(!inserted);
    inserted = -1;
    p2 = umap_insert_withResult(strv_int, m2, y, &inserted);
    assert(p2 && streq(p2->first, y.first) && p2->second == y.second);
    assert(!inserted);
    inserted = -1;
    p3 = umap_insert_withResult(strp_int, m3, z, &inserted);
    assert(p3 && streq(p3->first, z.first) && p3->second == z.second);
    assert(!inserted);
    inserted = -1;
    x.first = 50;
    x.second = "500";
    y.first = "050";
    y.second = 500;
    z.first = "050";
    z.second = 500;
    p1 = umap_insert_withResult(int_str, m1, x, &inserted);
    assert(p1 && p1->first == x.first && streq(p1->second, x.second));
    assert(!inserted);
    inserted = -1;
    p2 = umap_insert_withResult(strv_int, m2, y, &inserted);
    assert(p2 && streq(p2->first, y.first) && p2->second == y.second);
    assert(!inserted);
    inserted = -1;
    p3 = umap_insert_withResult(strp_int, m3, z, &inserted);
    assert(p3 && streq(p3->first, z.first) && p3->second == z.second);
    assert(!inserted);
    compare_int_str(m1, c2, 5);
    compare_strv_int(m2, c3, 5);
    compare_strp_int(m3, c3, 5);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_insert_fromArray(void) {
    Pair_int_str arrInt[7] = {{50,"050"},{50,"500"},{40,"040"},{30,"030"},{20,"020"},{10,"010"},{30,"300"}};
    Pair_strv_int arrStr1[7] = {{"050",50},{"050",500},{"040",40},{"030",30},{"020",20},{"010",10},{"030",300}};
    Pair_strp_int arrStr2[7] = {{"050",50},{"050",500},{"040",40},{"030",30},{"020",20},{"010",10},{"030",300}};
    DictData c1[] = {{10,"010",0},{20,"020",0},{30,"300",0},{40,"040",0},{50,"500",0}}, c2[] = {{10,"010",0},{20,"020",0},{300,"030",0},{40,"040",0},{500,"050",0}};
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new(strv_int);
    UMap_strp_int *m3 = umap_new(strp_int);

    umap_insert_fromArray(int_str, m1, arrInt, 0);
    compare_int_str(m1, c1, 0);
    umap_insert_fromArray(int_str, m1, arrInt, 7);
    umap_insert_fromArray(strv_int, m2, arrStr1, 7);
    umap_insert_fromArray(strp_int, m3, arrStr2, 7);
    compare_int_str(m1, c1, 5);
    compare_strv_int(m2, c2, 5);
    compare_strp_int(m3, c2, 5);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_remove_key(void) {
    unsigned char removed[] = {0, 0, 0, 0, 1, 1, 1, 0};
    DictData c[] = {{20,"020",0},{30,"030",0}};
    int removedInts[] = {0,60,39,41,40,10,50,40}, i;
    char *removedStrs[] = {"000","060","039","041","040","010","050","040"};
    Pair_int_str arrInt[5] = {{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    UMap_int_str *m1 = umap_new_fromArray(int_str, arrInt, 5);
    UMap_strv_int *m2 = umap_new_fromArray(strv_int, arrStr1, 5);
    UMap_strp_int *m3 = umap_new_fromArray(strp_int, arrStr2, 5);

    for (i = 0; i < 8; ++i) {
        assert(umap_remove_key(int_str, m1, removedInts[i]) == removed[i]);
        assert(umap_remove_key(strv_int, m2, removedStrs[i]) == removed[i]);
        assert(umap_remove_key(strp_int, m3, removedStrs[i]) == removed[i]);
    }
    compare_int_str(m1, c, 2);
    compare_strv_int(m2, c, 2);
    compare_strp_int(m3, c, 2);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_find(void) {
    Pair_int_str arrInt[10] = {{100,"100"},{90,"090"},{80,"080"},{70,"070"},{60,"060"},{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new_fromArray(strv_int, arrStr1, 10);
    UMap_strp_int *m3 = umap_new_fromArray(strp_int, arrStr2, 10);
    Pair_int_str p1;
    Pair_strv_int p2;
    Pair_strp_int p3;

    assert(!umap_find(int_str, m1, 0));
    umap_insert_fromArray(int_str, m1, arrInt, 10);
    assert(!umap_find(int_str, m1, 9));
    assert(!umap_find(strv_int, m2, "009"));
    assert(!umap_find(strp_int, m3, "009"));
    assert(!umap_find(int_str, m1, 101));
    assert(!umap_find(strv_int, m2, "101"));
    assert(!umap_find(strp_int, m3, "101"));
    assert(!umap_find(int_str, m1, 69));
    assert(!umap_find(strv_int, m2, "069"));
    assert(!umap_find(strp_int, m3, "069"));
    assert(!umap_find(int_str, m1, 71));
    assert(!umap_find(strv_int, m2, "071"));
    assert(!umap_find(strp_int, m3, "071"));

    p1 = *umap_find(int_str, m1, 70);
    p2 = *umap_find(strv_int, m2, "070");
    p3 = *umap_find(strp_int, m3, "070");
    assert(p1.first == 70 && streq(p1.second, "070"));
    assert(streq(p2.first, "070") && p2.second == 70);
    assert(streq(p3.first, "070") && p3.second == 70);
    p1 = *umap_find(int_str, m1, 60);
    p2 = *umap_find(strv_int, m2, "060");
    p3 = *umap_find(strp_int, m3, "060");
    assert(p1.first == 60 && streq(p1.second, "060"));
    assert(streq(p2.first, "060") && p2.second == 60);
    assert(streq(p3.first, "060") && p3.second == 60);
    p1 = *umap_find(int_str, m1, 80);
    p2 = *umap_find(strv_int, m2, "080");
    p3 = *umap_find(strp_int, m3, "080");
    assert(p1.first == 80 && streq(p1.second, "080"));
    assert(streq(p2.first, "080") && p2.second == 80);
    assert(streq(p3.first, "080") && p3.second == 80);
    p1 = *umap_find(int_str, m1, 10);
    p2 = *umap_find(strv_int, m2, "010");
    p3 = *umap_find(strp_int, m3, "010");
    assert(p1.first == 10 && streq(p1.second, "010"));
    assert(streq(p2.first, "010") && p2.second == 10);
    assert(streq(p3.first, "010") && p3.second == 10);
    p1 = *umap_find(int_str, m1, 100);
    p2 = *umap_find(strv_int, m2, "100");
    p3 = *umap_find(strp_int, m3, "100");
    assert(p1.first == 100 && streq(p1.second, "100"));
    assert(streq(p2.first, "100") && p2.second == 100);
    assert(streq(p3.first, "100") && p3.second == 100);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_set_load_factor(void) {
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new(strv_int);
    UMap_strp_int *m3 = umap_new(strp_int);
    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    DictData data[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = i;
        arrInt[i].second = strs[i];
        arrStr1[i].first = strs[i];
        arrStr1[i].second = i;
        arrStr2[i].first = strs[i];
        arrStr2[i].second = i;
        data[i].i = i;
        data[i].s = strs[i];
    }

    assert(umap_max_load_factor(m1) == 75);
    assert(umap_max_load_factor(m2) == 75);
    assert(umap_max_load_factor(m3) == 75);
    umap_set_load_factor(int_str, m1, 24);
    umap_set_load_factor(strv_int, m2, 24);
    umap_set_load_factor(strp_int, m3, 24);
    assert(umap_max_load_factor(m1) == 75);
    assert(umap_max_load_factor(m2) == 75);
    assert(umap_max_load_factor(m3) == 75);
    umap_set_load_factor(int_str, m1, 125);
    umap_set_load_factor(strv_int, m2, 125);
    umap_set_load_factor(strp_int, m3, 125);
    assert(umap_max_load_factor(m1) == 75);
    assert(umap_max_load_factor(m2) == 75);
    assert(umap_max_load_factor(m3) == 75);

    umap_insert_fromArray(int_str, m1, arrInt, 50);
    umap_insert_fromArray(strv_int, m2, arrStr1, 50);
    umap_insert_fromArray(strp_int, m3, arrStr2, 50);
    assert(umap_max_load_factor(m1) == 75);
    assert(umap_max_load_factor(m2) == 75);
    assert(umap_max_load_factor(m3) == 75);
    umap_set_load_factor(int_str, m1, 100);
    umap_set_load_factor(strv_int, m2, 100);
    umap_set_load_factor(strp_int, m3, 100);
    assert(umap_max_load_factor(m1) == 100);
    assert(umap_max_load_factor(m2) == 100);
    assert(umap_max_load_factor(m3) == 100);
    compare_int_str(m1, data, 50);
    compare_strv_int(m2, data, 50);
    compare_strp_int(m3, data, 50);
    umap_set_load_factor(int_str, m1, 25);
    umap_set_load_factor(strv_int, m2, 25);
    umap_set_load_factor(strp_int, m3, 25);
    assert(umap_max_load_factor(m1) == 25);
    assert(umap_max_load_factor(m2) == 25);
    assert(umap_max_load_factor(m3) == 25);
    compare_int_str(m1, data, 50);
    compare_strv_int(m2, data, 50);
    compare_strp_int(m3, data, 50);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_rehash(void) {
    UMap_int_str *m1 = umap_new(int_str);
    UMap_strv_int *m2 = umap_new(strv_int);
    UMap_strp_int *m3 = umap_new(strp_int);
    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    DictData data[50] = {0};
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = i;
        arrInt[i].second = strs[i];
        arrStr1[i].first = strs[i];
        arrStr1[i].second = i;
        arrStr2[i].first = strs[i];
        arrStr2[i].second = i;
        data[i].i = i;
        data[i].s = strs[i];
    }

    assert(umap_bucket_count(m1) == 32);
    assert(umap_bucket_count(m2) == 32);
    assert(umap_bucket_count(m3) == 32);
    assert(umap_rehash(int_str, m1, 16));
    assert(umap_rehash(strv_int, m2, 16));
    assert(umap_rehash(strp_int, m3, 16));
    assert(umap_bucket_count(m1) == 32);
    assert(umap_bucket_count(m2) == 32);
    assert(umap_bucket_count(m3) == 32);
    assert(umap_rehash(int_str, m1, 33));
    assert(umap_rehash(strv_int, m2, 33));
    assert(umap_rehash(strp_int, m3, 33));
    assert(umap_bucket_count(m1) == 64);
    assert(umap_bucket_count(m2) == 64);
    assert(umap_bucket_count(m3) == 64);

    umap_insert_fromArray(int_str, m1, arrInt, 50);
    umap_insert_fromArray(strv_int, m2, arrStr1, 50);
    umap_insert_fromArray(strp_int, m3, arrStr2, 50);
    assert(umap_bucket_count(m1) == 128);
    assert(umap_bucket_count(m2) == 128);
    assert(umap_bucket_count(m3) == 128);
    assert(umap_rehash(int_str, m1, 1024));
    assert(umap_rehash(strv_int, m2, 1024));
    assert(umap_rehash(strp_int, m3, 1024));
    assert(umap_bucket_count(m1) == 1024);
    assert(umap_bucket_count(m2) == 1024);
    assert(umap_bucket_count(m3) == 1024);
    assert(!umap_rehash(int_str, m1, UINT_MAX));
    compare_int_str(m1, data, 50);
    compare_strv_int(m2, data, 50);
    compare_strp_int(m3, data, 50);
    umap_free(int_str, m1);
    umap_free(strv_int, m2);
    umap_free(strp_int, m3);
}

void test_nested_dicts(void) {
    Pair_strv_int arrStr[50];
    DictData data[50] = {0};
    NestedDictData results[5] = {0};
    UMap_nested *m = umap_new(nested);
    Pair_nested *it;
    int i;
    for (i = 0; i < 50; ++i) {
        arrStr[i].first = strs[i];
        arrStr[i].second = i;
        data[i].i = i;
        data[i].s = strs[i];
    }

    for (i = 0; i < 5; ++i) {
        int success = 0;
        Pair_nested p;
        UMap_strv_int *inner = umap_new_fromArray(strv_int, &arrStr[10 * i], 10);
        p.first = arrStr[10 * i].first;
        p.second = inner;
        umap_insert_withResult(nested, m, p, &success);
        assert(success);
        assert(umap_size(m) == (unsigned) i + 1);
        results[i].s = arrStr[10 * i].first;
        results[i].data = &data[10 * i];
    }

    for (i = 0; i < 5; ++i) {
        UMap_strv_int *inner;
        int j = 0;
        assert((it = umap_find(nested, m, arrStr[10 * i].first)));
        inner = it->second;
        for (; j < 10; ++j) {
            int value = (i * 10) + j;
            Pair_strv_int *ptr = umap_find(strv_int, inner, arrStr[value].first);
            assert(ptr && ptr->second == arrStr[value].second);
        }
    }

    i = 0;
    umap_iter(nested, m, it) {
        UMap_strv_int **at;
        int j;
        NestedDictData *r = NULL;
        for (j = 0; j < 5; ++j) {
            if (streq(it->first, results[j].s)) {
                r = &results[j];
                break;
            }
        }
        assert(r);
        assert(r->found == 0);
        at = umap_at(nested, m, it->first);
        assert(at && *at == it->second);
        compare_strv_int(it->second, r->data, 10);
        r->found = 1;
        ++i;
    }
    assert(i == 5);
    for (i = 0; i < 5; ++i) {
        assert(results[i].found);
    }
    umap_free(nested, m);
}

int main(void) {
    test_empty_init();
    test_init_fromArray();
    test_createCopy();
    test_insert_element();
    test_insert_fromArray();
    test_remove_key();
    test_find();
    test_set_load_factor();
    test_rehash();
    test_nested_dicts();
    return 0;
}
