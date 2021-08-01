#include "set.h"
#include <assert.h>
#include <limits.h>

gen_set_headers(int, int)
gen_set_headers(str, char *)
gen_set_source(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_set_source(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

int ints_rand[] = {200,25,220,120,5,205,50,15,60,235,10,70,130,105,185,225,90,30,155,100,150,0,95,170,190,
125,210,75,45,160,175,145,55,230,35,65,110,140,115,20,215,85,195,240,245,135,80,180,40,165};
int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs_rand[] = {"200","025","220","120","005","205","050","015","060","235","010","070","130","105",
"185","225","090","030","155","100","150","000","095","170","190","125","210","075","045","160","175",
"145","055","230","035","065","110","140","115","020","215","085","195","240","245","135","080","180",
"040","165"};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

void compare_ints(Set_int *s, int *comparison, unsigned size) {
    unsigned i = 0;
    SetEntry_int *it;
    assert(set_size(s) == size);
    if (size) {
        assert(!set_empty(s));
    } else {
        assert(set_empty(s));
    }
    set_iter(int, s, it) {
        assert(it->data == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    set_riter(int, s, it) {
        assert(it->data == comparison[i--]);
    }
    assert(i == UINT_MAX);
}

void compare_strs(Set_str *s, char **comparison, unsigned size) {
    unsigned i = 0;
    SetEntry_str *it;
    assert(set_size(s) == size);
    if (size) {
        assert(!set_empty(s));
    } else {
        assert(set_empty(s));
    }
    set_iter(str, s, it) {
        assert(streq(it->data, comparison[i++]));
    }
    assert(i == size);
    i = size - 1;
    set_riter(str, s, it) {
        assert(streq(it->data, comparison[i--]));
    }
    assert(i == UINT_MAX);
}

void test_empty_init(void) {
    Set_int *si = set_new(int);
    Set_str *ss = set_new(str);
    compare_ints(si, ints, 0);
    compare_strs(ss, strs, 0);
    set_free(int, si);
    set_free(str, ss);
}

void test_init_fromArray(void) {
    int c1[] = {200};
    char *c2[] = {"200"};
    Set_int *si = set_new_fromArray(int, NULL, 5);
    Set_str *ss;

    compare_ints(si, ints, 0);
    set_free(int, si);
    si = set_new_fromArray(int, ints, 0);
    compare_ints(si, ints, 0);
    set_free(int, si);

    si = set_new_fromArray(int, ints_rand, 1);
    ss = set_new_fromArray(str, strs_rand, 1);
    compare_ints(si, c1, 1);
    compare_strs(ss, c2, 1);
    set_free(int, si);
    set_free(str, ss);

    si = set_new_fromArray(int, ints_rand, 50);
    ss = set_new_fromArray(str, strs_rand, 50);
    compare_ints(si, ints, 50);
    compare_strs(ss, strs, 50);
    set_free(int, si);
    set_free(str, ss);
}

void test_createCopy(void) {
    Set_int *b = set_new_fromArray(int, NULL, 0), *si;
    Set_str *c, *ss;

    si = set_createCopy(int, b);
    compare_ints(si, ints, 0);
    set_free(int, si);
    set_free(int, b);
    b = set_new_fromArray(int, ints_rand, 50);
    c = set_new_fromArray(str, strs_rand, 50);
    si = set_createCopy(int, b);
    ss = set_createCopy(str, c);
    set_free(int, b);
    set_free(str, c);
    compare_ints(si, ints, 50);
    compare_strs(ss, strs, 50);
    set_free(int, si);
    set_free(str, ss);
}

void test_insert_element(void) {
    Set_int *si = set_new(int);
    Set_str *ss = set_new(str);
    int a1[] = {50,40,30,20,10}, c1[] = {10,20,30,40,50}, i, inserted = -1;
    char *a2[] = {"050","040","030","020","010"}, *c2[] = {"010","020","030","040","050"};

    for (i = 0; i < 5; ++i) {
        set_insert_withResult(int, si, a1[i], &inserted);
        assert(inserted);
        inserted = -1;
        set_insert_withResult(str, ss, a2[i], &inserted);
        assert(inserted);
        inserted = -1;
    }
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);

    set_insert_withResult(int, si, 40, &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(str, ss, "040", &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(int, si, 10, &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(str, ss, "010", &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(int, si, 50, &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(str, ss, "050", &inserted);
    assert(!inserted);
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);
    set_free(int, si);
    set_free(str, ss);
}

void test_insert_fromArray(void) {
    int a1[7] = {50,50,40,30,20,10,30}, c1[] = {10,20,30,40,50};
    char *a2[7] = {"050","050","040","030","020","010","030"}, *c2[] = {"010","020","030","040","050"};
    Set_int *si = set_new(int);
    Set_str *ss = set_new(str);

    set_insert_fromArray(int, si, NULL, 5);
    set_insert_fromArray(int, si, ints, 0);
    compare_ints(si, c1, 0);
    set_insert_fromArray(int, si, a1, 7);
    set_insert_fromArray(str, ss, a2, 7);
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);
    set_free(int, si);
    set_free(str, ss);
}

void test_insert_fromSet(void) {
    int a1[] = {50,40,30,20,10}, c1[] = {10,20,30,40,50};
    char *a2[] = {"050","040","030","020","010"}, *c2[] = {"010","020","030","040","050"};
    Set_int *b = set_new_fromArray(int, a1, 5), *si = set_new(int);
    Set_str *c = set_new_fromArray(str, a2, 5), *ss = set_new(str);

    set_insert_fromSet(int, si, NULL, b->root);
    set_insert_fromSet(int, si, b->root, b->root);
    compare_ints(si, c1, 0);

    set_insert_fromSet(int, si, b->root->left->left, b->root->left->left->parent);
    set_insert_fromSet(str, ss, c->root->left->left, c->root->left->left->parent);
    set_insert_fromSet(int, si, b->root->right, NULL);
    set_insert_fromSet(str, ss, c->root->right, NULL);
    set_insert_fromSet(int, si, b->root->left, b->root->right);
    set_insert_fromSet(str, ss, c->root->left, c->root->right);
    set_free(int, b);
    set_free(str, c);
    compare_ints(si, c1, 5);
    compare_strs(ss, c2, 5);
    set_free(int, si);
    set_free(str, ss);
}

void test_remove_value(void) {
    int a1[] = {50,40,30,20,10}, c1[] = {20,30}, removedInts[] = {0,60,39,41,40,10,50,40}, i;
    char *a2[] = {"050","040","030","020","010"}, *c2[] = {"020","030"}, *removedStrs[] = {"000","060","039","041","040","010","050","040"};
    Set_int *si = set_new_fromArray(int, a1, 5);
    Set_str *ss = set_new_fromArray(str, a2, 5);

    for (i = 0; i < 8; ++i) {
        set_remove_value(int, si, removedInts[i]);
        set_remove_value(str, ss, removedStrs[i]);
    }
    compare_ints(si, c1, 2);
    compare_strs(ss, c2, 2);
    set_free(int, si);
    set_free(str, ss);
}

void test_remove_entry(void) {
    int a1[] = {50,40,30,20,10}, c1[] = {20,30};
    char *a2[] = {"050","040","030","020","010"}, *c2[] = {"020","030"};
    Set_int *si = set_new_fromArray(int, a1, 5);
    Set_str *ss = set_new_fromArray(str, a2, 5);

    set_remove_entry(int, si, NULL);
    set_remove_entry(int, si, si->root);
    set_remove_entry(str, ss, ss->root);
    set_remove_entry(int, si, set_begin(int, si));
    set_remove_entry(str, ss, set_begin(str, ss));
    set_remove_entry(int, si, set_rbegin(int, si));
    set_remove_entry(str, ss, set_rbegin(str, ss));
    compare_ints(si, c1, 2);
    compare_strs(ss, c2, 2);
    set_free(int, si);
    set_free(str, ss);
}

void test_erase_entries(void) {
    int a1[] = {100,90,80,70,60,50,40,30,20,10}, c1[] = {30,40,50,80};
    char *a2[] = {"100","090","080","070","060","050","040","030","020","010"}, *c2[] = {"030","040","050","080"};
    Set_int *si = set_new_fromArray(int, a1, 10);
    Set_str *ss = set_new_fromArray(str, a2, 10);
    SetEntry_int *p1 = si->root, *p2 = si->root;
    SetEntry_str *p3 = ss->root, *p4 = ss->root;

    setEntry_advance(int, &p1, -1);
    setEntry_advance(int, &p2, 1);
    setEntry_advance(str, &p3, -1);
    setEntry_advance(str, &p4, 1);
    set_erase(int, si, NULL, p2);
    set_erase(int, si, p1, p1);
    set_erase(int, si, p1, p2);
    set_erase(str, ss, p3, p4);
    p1 = set_rbegin(int, si);
    setEntry_advance(int, &p1, -1);
    p3 = set_rbegin(str, ss);
    setEntry_advance(str, &p3, -1);
    set_erase(int, si, p1, NULL);
    set_erase(str, ss, p3, NULL);
    p1 = set_begin(int, si), p2 = p1;
    setEntry_advance(int, &p2, 2);
    p3 = set_begin(str, ss), p4 = p3;
    setEntry_advance(str, &p4, 2);
    set_erase(int, si, p1, p2);
    set_erase(str, ss, p3, p4);
    compare_ints(si, c1, 4);
    compare_strs(ss, c2, 4);
    set_free(int, si);
    set_free(str, ss);
}

void test_find(void) {
    int a1[] = {100,90,80,70,60,50,40,30,20,10}, v1;
    char *a2[] = {"100","090","080","070","060","050","040","030","020","010"}, *v2;
    Set_int *si = set_new(int);
    Set_str *ss = set_new_fromArray(str, a2, 10);

    assert(!set_contains(int, si, 0));
    set_insert_fromArray(int, si, a1, 10);
    assert(!set_contains(int, si, 9));
    assert(!set_contains(str, ss, "009"));
    assert(!set_contains(int, si, 101));
    assert(!set_contains(str, ss, "101"));
    assert(!set_contains(int, si, 69));
    assert(!set_contains(str, ss, "069"));
    assert(!set_contains(int, si, 71));
    assert(!set_contains(str, ss, "071"));

    v1 = set_find(int, si, 70)->data;
    v2 = set_find(str, ss, "070")->data;
    assert(v1 == 70);
    assert(streq(v2, "070"));
    v1 = set_find(int, si, 60)->data;
    v2 = set_find(str, ss, "060")->data;
    assert(v1 == 60);
    assert(streq(v2, "060"));
    v1 = set_find(int, si, 80)->data;
    v2 = set_find(str, ss, "080")->data;
    assert(v1 == 80);
    assert(streq(v2, "080"));
    v1 = set_find(int, si, 10)->data;
    v2 = set_find(str, ss, "010")->data;
    assert(v1 == 10);
    assert(streq(v2, "010"));
    v1 = set_find(int, si, 100)->data;
    v2 = set_find(str, ss, "100")->data;
    assert(v1 == 100);
    assert(streq(v2, "100"));
    set_free(int, si);
    set_free(str, ss);
}

void test_union(void) {
    int c1[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060","065","070"};
    Set_int *si1 = set_new_fromArray(int, ints, 10), *si2 = set_new(int), *ri;
    Set_str *ss1 = set_new_fromArray(str, strs, 10), *ss2 = set_new_fromArray(str, &strs[5], 10), *rs;

    ri = set_union(int, si1, si2);
    compare_ints(ri, ints, 10);
    set_free(int, ri);
    ri = set_union(int, si2, si1);
    compare_ints(ri, ints, 10);
    set_free(int, ri);
    ri = set_union(int, si2, si2);
    compare_ints(ri, ints, 0);
    set_free(int, ri);

    set_insert_fromArray(int, si2, &ints[5], 10);
    ri = set_union(int, si1, si2);
    rs = set_union(str, ss1, ss2);
    compare_ints(ri, c1, 15);
    compare_strs(rs, c2, 15);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, ri);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, rs);
}

void test_intersection(void) {
    int c1[] = {25,30,35,40,45};
    char *c2[] = {"025","030","035","040","045"};
    Set_int *si1 = set_new_fromArray(int, ints, 10), *si2 = set_new(int), *ri;
    Set_str *ss1 = set_new_fromArray(str, strs, 10), *ss2 = set_new_fromArray(str, &strs[5], 10), *rs;

    ri = set_intersection(int, si1, si2);
    compare_ints(ri, ints, 0);
    set_free(int, ri);
    ri = set_intersection(int, si2, si1);
    compare_ints(ri, ints, 0);
    set_free(int, ri);
    ri = set_intersection(int, si2, si2);
    compare_ints(ri, ints, 0);
    set_free(int, ri);

    set_insert_fromArray(int, si2, &ints[5], 10);
    ri = set_intersection(int, si1, si2);
    rs = set_intersection(str, ss1, ss2);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, ri);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, rs);
}

void test_difference(void) {
    int c1[] = {0,5,10,15,20};
    char *c2[] = {"000","005","010","015","020"};
    Set_int *si1 = set_new_fromArray(int, ints, 10), *si2 = set_new(int), *ri;
    Set_str *ss1 = set_new_fromArray(str, strs, 10), *ss2 = set_new_fromArray(str, &strs[5], 10), *rs;

    ri = set_difference(int, si1, si2);
    compare_ints(ri, ints, 10);
    set_free(int, ri);
    ri = set_difference(int, si2, si1);
    compare_ints(ri, ints, 0);
    set_free(int, ri);
    ri = set_difference(int, si2, si2);
    compare_ints(ri, ints, 0);
    set_free(int, ri);

    set_insert_fromArray(int, si2, &ints[5], 10);
    ri = set_difference(int, si1, si2);
    rs = set_difference(str, ss1, ss2);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, ri);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, rs);
}

void test_symmetric_difference(void) {
    int c1[] = {0,5,10,15,20,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","050","055","060","065","070"};
    Set_int *si1 = set_new_fromArray(int, ints, 10), *si2 = set_new(int), *ri;
    Set_str *ss1 = set_new_fromArray(str, strs, 10), *ss2 = set_new_fromArray(str, &strs[5], 10), *rs;

    ri = set_symmetric_difference(int, si1, si2);
    compare_ints(ri, ints, 10);
    set_free(int, ri);
    ri = set_symmetric_difference(int, si2, si1);
    compare_ints(ri, ints, 10);
    set_free(int, ri);
    ri = set_symmetric_difference(int, si2, si2);
    compare_ints(ri, ints, 0);
    set_free(int, ri);

    set_insert_fromArray(int, si2, &ints[5], 10);
    ri = set_symmetric_difference(int, si1, si2);
    rs = set_symmetric_difference(str, ss1, ss2);
    compare_ints(ri, c1, 10);
    compare_strs(rs, c2, 10);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, ri);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, rs);
}

void test_subset_superset(void) {
    int i;
    Set_int *a[3];
    Set_str *b[3];
    a[0] = set_new(int), a[1] = set_new_fromArray(int, &ints[5], 6), a[2] = set_new_fromArray(int, ints, 11);
    b[0] = set_new_fromArray(str, strs, 6), b[1] = set_new_fromArray(str, &strs[5], 6), b[2] = set_new_fromArray(str, strs, 11);

    assert(set_issubset(int, a[0], a[0]));
    assert(set_issuperset(int, a[0], a[0]));
    assert(set_issubset(int, a[0], a[1]));
    assert(!set_issubset(int, a[1], a[0]));
    assert(!set_issuperset(int, a[0], a[1]));
    assert(set_issuperset(int, a[1], a[0]));

    set_insert_fromArray(int, a[0], ints, 6);

    for (i = 0; i < 3; ++i) {
        assert(set_issubset(int, a[i], a[i]));
        assert(set_issubset(str, b[i], b[i]));
        assert(set_issuperset(int, a[i], a[i]));
        assert(set_issuperset(str, b[i], b[i]));
    }

    for (i = 0; i < 2; ++i) {
        assert(set_issubset(int, a[i], a[2]));
        assert(!set_issubset(int, a[2], a[i]));
        assert(!set_issuperset(int, a[i], a[2]));
        assert(set_issuperset(int, a[2], a[i]));

        assert(set_issubset(str, b[i], b[2]));
        assert(!set_issubset(str, b[2], b[i]));
        assert(!set_issuperset(str, b[i], b[2]));
        assert(set_issuperset(str, b[2], b[i]));
    }

    assert(!set_issubset(int, a[0], a[1]));
    assert(!set_issubset(int, a[1], a[0]));
    assert(!set_issuperset(int, a[0], a[1]));
    assert(!set_issuperset(int, a[1], a[0]));

    assert(!set_issubset(str, b[0], b[1]));
    assert(!set_issubset(str, b[1], b[0]));
    assert(!set_issuperset(str, b[0], b[1]));
    assert(!set_issuperset(str, b[1], b[0]));

    for (i = 0; i < 3; ++i) {
        set_free(int, a[i]);
        set_free(str, b[i]);
    }
}

void test_disjoint(void) {
    int a1[] = {1,3,5}, a2[] = {2,4,6,8,10}, a3[] = {5,7,8};
    char *b1[] = {"01","03","05"}, *b2[] = {"02","04","06","08","10"}, *b3[] = {"05","07","08"};
    Set_int *si1 = set_new(int), *si2 = set_new_fromArray(int, a2, 5), *si3 = set_new_fromArray(int, a3, 3);
    Set_str *ss1 = set_new_fromArray(str, b1, 3), *ss2 = set_new_fromArray(str, b2, 5), *ss3 = set_new_fromArray(str, b3, 3);

    assert(set_isdisjoint(int, si1, si1));
    assert(set_isdisjoint(int, si1, si2));
    assert(set_isdisjoint(int, si2, si1));
    set_insert_fromArray(int, si1, a1, 3);

    assert(set_isdisjoint(int, si1, si2));
    assert(set_isdisjoint(int, si2, si1));
    assert(!set_isdisjoint(int, si2, si3));
    assert(!set_isdisjoint(int, si3, si2));
    assert(!set_isdisjoint(int, si1, si3));
    assert(!set_isdisjoint(int, si3, si1));
    assert(set_isdisjoint(str, ss1, ss2));
    assert(set_isdisjoint(str, ss2, ss1));
    assert(!set_isdisjoint(str, ss2, ss3));
    assert(!set_isdisjoint(str, ss3, ss2));
    assert(!set_isdisjoint(str, ss1, ss3));
    assert(!set_isdisjoint(str, ss3, ss1));

    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

int main(void) {
    test_empty_init();
    test_init_fromArray();
    test_createCopy();
    test_insert_element();
    test_insert_fromArray();
    test_insert_fromSet();
    test_remove_value();
    test_remove_entry();
    test_erase_entries();
    test_find();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_subset_superset();
    test_disjoint();
    return 0;
}
