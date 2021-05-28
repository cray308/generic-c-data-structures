#include "set.h"
#include <assert.h>

gen_set(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_set(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

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

void compare_ints(Set_int *s, int *comparison, int size) {
    int i = 0;
    SetEntry_int *it;
    assert(set_size(s) == size);
    set_iter(int, s, it) {
        assert(it->data == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    set_riter(int, s, it) {
        assert(it->data == comparison[i--]);
    }
    assert(i == -1);
}

void compare_strs(Set_str *s, char **comparison, int size) {
    int i = 0;
    SetEntry_str *it;
    assert(set_size(s) == size);
    set_iter(str, s, it) {
        assert(streq(it->data, comparison[i++]));
    }
    assert(i == size);
    i = size - 1;
    set_riter(str, s, it) {
        assert(streq(it->data, comparison[i--]));
    }
    assert(i == -1);
}

void test_basic_ints(void) {
    Set_int *s = set_new(int);
    SetEntry_int *e;
    int i;
    assert(set_empty(s));
    compare_ints(s, ints, 0);

    for (i = 0; i < 50; ++i) {
        set_insert(int, s, ints_rand[i]);
    }
    set_insert(int, s, ints[0]);
    set_insert(int, s, ints[49]);
    assert(!set_empty(s));
    compare_ints(s, ints, 50);

    assert(set_find(int, s, 121) == NULL);
    e = set_find(int, s, 120);
    assert(e != NULL);
    set_remove_entry(int, s, e);
    assert(set_find(int, s, 120) == NULL);
    assert(set_size(s) == 49);
    assert(s->root->data == 125);

    set_remove_value(int, s, 60);
    assert(set_find(int, s, 60) == NULL);
    set_remove_value(int, s, 60);
    assert(set_size(s) == 48);
    set_free(int, s);
}

void test_basic_strs(void) {
    Set_str *s = set_new(str);
    SetEntry_str *e;
    int i;
    assert(set_empty(s));
    compare_strs(s, strs, 0);

    for (i = 0; i < 50; ++i) {
        set_insert(str, s, strs_rand[i]);
    }
    set_insert(str, s, strs[0]);
    set_insert(str, s, strs[49]);
    assert(!set_empty(s));
    compare_strs(s, strs, 50);

    assert(set_find(str, s, "121") == NULL);
    e = set_find(str, s, "120");
    assert(e != NULL);
    set_remove_entry(str, s, e);
    assert(set_find(str, s, "120") == NULL);
    assert(set_size(s) == 49);
    assert(streq(s->root->data, "125"));

    set_remove_value(str, s, "060");
    assert(set_find(str, s, "060") == NULL);
    set_remove_value(str, s, "060");
    assert(set_size(s) == 48);
    set_free(str, s);
}

void test_init_clear(void) {
    Set_int *si = set_new_fromArray(int, ints_rand, 50), *si2;
    Set_str *ss = set_new_fromArray(str, strs_rand, 50), *ss2;
    compare_ints(si, ints, 50);
    compare_strs(ss, strs, 50);

    si2 = set_createCopy(int, si);
    ss2 = set_createCopy(str, ss);
    compare_ints(si2, ints, 50);
    compare_strs(ss2, strs, 50);

    set_clear(int, si);
    set_clear(str, ss);
    compare_ints(si, ints, 0);
    compare_strs(ss, strs, 0);

    set_free(int, si);
    set_free(str, ss);
    set_free(int, si2);
    set_free(str, ss2);
}

void test_membership(void) {
    Set_int *si = set_new_fromArray(int, ints_rand, 50);
    Set_str *ss = set_new_fromArray(str, strs_rand, 50);
    assert(!set_contains(int, si, 64));
    assert(!set_contains(str, ss, "064"));
    assert(!set_contains(int, si, 121));
    assert(!set_contains(str, ss, "121"));
    assert(set_contains(int, si, 0));
    assert(set_contains(str, ss, "000"));
    assert(set_contains(int, si, 245));
    assert(set_contains(str, ss, "245"));
    assert(!set_contains(int, si, -1));
    assert(!set_contains(str, ss, "..."));
    assert(!set_contains(int, si, 246));
    assert(!set_contains(str, ss, "246"));
    set_free(int, si); set_free(str, ss);
}

void test_remove(void) {
    int c1[] = {10,20,35,40};
    char *c2[] = {"010","020","035","040"};
    SetEntry_int *begin1 = NULL, *end1 = NULL;
    SetEntry_str *begin2 = NULL, *end2 = NULL;
    Set_int *si = set_new_fromArray(int, ints, 10);
    Set_str *ss = set_new_fromArray(str, strs, 10);

    set_remove_entry(int, si, si->root);
    set_remove_entry(str, ss, ss->root);
    set_remove_value(int, si, 5);
    set_remove_value(str, ss, "005");

    set_erase(int, si, begin1, end1);
    set_erase(str, ss, begin2, end2);
    begin1 = set_find(int, si, 0), end1 = begin1;
    begin2 = set_find(str, ss, "000"), end2 = begin2;
    set_erase(int, si, begin1, end1);
    set_erase(str, ss, begin2, end2);
    assert(set_size(si) == 8);
    assert(set_size(ss) == 8);

    end1 = set_find(int, si, 10), end2 = set_find(str, ss, "010");
    set_erase(int, si, begin1, end1);
    set_erase(str, ss, begin2, end2);

    begin1 = set_find(int, si, 45), end1 = NULL;
    begin2 = set_find(str, ss, "045"), end2 = NULL;
    set_erase(int, si, begin1, end1);
    set_erase(str, ss, begin2, end2);

    begin1 = set_find(int, si, 25), end1 = set_find(int, si, 35);
    begin2 = set_find(str, ss, "025"), end2 = set_find(str, ss, "035");
    set_erase(int, si, begin1, end1);
    set_erase(str, ss, begin2, end2);

    compare_ints(si, c1, 4);
    compare_strs(ss, c2, 4);
    set_free(int, si);
    set_free(str, ss);
}

void test_insert(void) {
    int c1[2][8] = {{0,4,5,10,14,15,16,20},{0,4,16,20}};
    char *c2[2][8] = {{"000","004","005","010","014","015","016","020"},{"000","004","016","020"}};
    int inserted = -1;
    SetEntry_int *begin1, *end1;
    SetEntry_str *begin2, *end2;
    Set_int *si = set_new_fromArray(int, ints, 5), *si2;
    Set_str *ss = set_new_fromArray(str, strs, 5), *ss2;

    set_insert_withResult(int, si, 5, &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(str, ss, "005", &inserted);
    assert(!inserted);
    inserted = -1;
    set_insert_withResult(int, si, 4, &inserted);
    assert(inserted);
    inserted = -1;
    set_insert_withResult(str, ss, "004", &inserted);
    assert(inserted);

    set_insert_fromArray(int, si, NULL, 0);
    set_insert_fromArray(str, ss, NULL, 0);
    set_insert_fromArray(int, si, &c1[0][4], 3);
    set_insert_fromArray(str, ss, &c2[0][4], 3);
    compare_ints(si, c1[0], 8);
    compare_strs(ss, c2[0], 8);

    si2 = set_new(int);
    ss2 = set_new(str);
    begin1 = set_find(int, si, 0), end1 = begin1;
    begin2 = set_find(str, ss, "000"), end2 = begin2;
    set_insert_fromSet(int, si2, begin1, end1);
    set_insert_fromSet(str, ss2, begin2, end2);
    assert(set_empty(si2));
    assert(set_empty(ss2));

    end1 = set_find(int, si, 5);
    end2 = set_find(str, ss, "005");
    set_insert_fromSet(int, si2, begin1, end1);
    set_insert_fromSet(str, ss2, begin2, end2);

    begin1 = set_find(int, si, 16), end1 = NULL;
    begin2 = set_find(str, ss, "016"), end2 = NULL;
    set_insert_fromSet(int, si2, begin1, end1);
    set_insert_fromSet(str, ss2, begin2, end2);
    compare_ints(si2, c1[1], 4);
    compare_strs(ss2, c2[1], 4);
    set_free(int, si2);
    set_free(str, ss2);
    set_free(int, si);
    set_free(str, ss);
}

void test_union(void) {
    int c1[] = {0,5,10,15,20};
    char *c2[] = {"000","005","010","015","020"};
    Set_int *si1 = set_new_fromArray(int, ints, 3), *si2 = set_new_fromArray(int, &ints[2], 3);
    Set_int *si3 = set_union(int, si1, si2);
    Set_str *ss1 = set_new_fromArray(str, strs, 3), *ss2 = set_new_fromArray(str, &strs[2], 3);
    Set_str *ss3 = set_union(str, ss1, ss2);
    compare_ints(si3, c1, 5);
    compare_strs(ss3, c2, 5);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

void test_intersection(void) {
    int c1[] = {10};
    char *c2[] = {"010"};
    Set_int *si1 = set_new_fromArray(int, ints, 3), *si2 = set_new_fromArray(int, &ints[2], 3);
    Set_int *si3 = set_intersection(int, si1, si2);
    Set_str *ss1 = set_new_fromArray(str, strs, 3), *ss2 = set_new_fromArray(str, &strs[2], 3);
    Set_str *ss3 = set_intersection(str, ss1, ss2);
    compare_ints(si3, c1, 1);
    compare_strs(ss3, c2, 1);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

void test_difference(void) {
    int c1[] = {0,5};
    char *c2[] = {"000","005"};
    Set_int *si1 = set_new_fromArray(int, ints, 3), *si2 = set_new_fromArray(int, &ints[2], 3);
    Set_int *si3 = set_difference(int, si1, si2);
    Set_str *ss1 = set_new_fromArray(str, strs, 3), *ss2 = set_new_fromArray(str, &strs[2], 3);
    Set_str *ss3 = set_difference(str, ss1, ss2);
    compare_ints(si3, c1, 2);
    compare_strs(ss3, c2, 2);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

void test_symmetric_difference(void) {
    int c1[] = {0,5,15,20};
    char *c2[] = {"000","005","015","020"};
    Set_int *si1 = set_new_fromArray(int, ints, 3), *si2 = set_new_fromArray(int, &ints[2], 3);
    Set_int *si3 = set_symmetric_difference(int, si1, si2);
    Set_str *ss1 = set_new_fromArray(str, strs, 3), *ss2 = set_new_fromArray(str, &strs[2], 3);
    Set_str *ss3 = set_symmetric_difference(str, ss1, ss2);
    compare_ints(si3, c1, 4);
    compare_strs(ss3, c2, 4);
    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

void test_subset(void) {
    Set_str *ss1 = set_new_fromArray(str, strs, 3), *ss2 = set_new_fromArray(str, strs, 5), *ss3 = set_new_fromArray(str, &strs[2], 3);
    Set_int *si1 = set_new_fromArray(int, ints, 3), *si2 = set_new_fromArray(int, ints, 5), *si3 = set_new_fromArray(int, &ints[2], 3);

    assert(set_issubset(int, si1, si1));
    assert(set_issubset(str, ss1, ss1));
    assert(set_issuperset(int, si1, si1));
    assert(set_issuperset(str, ss1, ss1));
    assert(set_issubset(int, si2, si2));
    assert(set_issubset(str, ss2, ss2));
    assert(set_issuperset(int, si2, si2));
    assert(set_issuperset(str, ss2, ss2));

    assert(set_issubset(int, si1, si2));
    assert(set_issubset(str, ss1, ss2));
    assert(!set_issubset(int, si2, si3));
    assert(!set_issubset(str, ss2, ss3));
    assert(!set_issubset(int, si1, si3));
    assert(!set_issubset(str, ss1, ss3));

    assert(set_issuperset(int, si2, si1));
    assert(set_issuperset(str, ss2, ss1));
    assert(!set_issuperset(int, si1, si2));
    assert(!set_issuperset(str, ss1, ss2));
    assert(!set_issuperset(int, si1, si3));
    assert(!set_issuperset(str, ss1, ss3));

    set_free(int, si1);
    set_free(int, si2);
    set_free(int, si3);
    set_free(str, ss1);
    set_free(str, ss2);
    set_free(str, ss3);
}

void test_disjoint(void) {
    int a1[] = {1,3,5}, a2[] = {2,4,6,8,10}, a3[] = {5,7,8};
    char *b1[] = {"01","03","05"}, *b2[] = {"02","04","06","08","10"}, *b3[] = {"05","07","08"};
    Set_int *si1 = set_new_fromArray(int, a1, 3), *si2 = set_new_fromArray(int, a2, 5), *si3 = set_new_fromArray(int, a3, 3);
    Set_str *ss1 = set_new_fromArray(str, b1, 3), *ss2 = set_new_fromArray(str, b2, 5), *ss3 = set_new_fromArray(str, b3, 3);

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
    test_basic_ints();
    test_basic_strs();
    test_init_clear();
    test_membership();
    test_remove();
    test_insert();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_subset();
    test_disjoint();
    return 0;
}
