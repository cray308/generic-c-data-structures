#include "array.h"
#include <assert.h>

#define customStrCopy(dest, src) do { if (src) { dest = malloc(strlen(src) + 1); strcpy(dest, src); } } while(0)
#define customStrDelete(x) do { if (x) free(x); } while(0)

gen_array_headers_withAlg(str, char *)
gen_array_headers_withAlg(int, int)
gen_array_source_withAlg(str, char *, ds_cmp_str_lt, customStrCopy, customStrDelete)
gen_array_source_withAlg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)

int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

void compare_ints(Array_int *a, int *comparison, unsigned size) {
    unsigned i = 0, j = size;
    int *it, *ref;
    assert(array_size(a) == size);
    if (size) {
        assert(!array_empty(a));
        assert(*array_front(a) == comparison[0] && *array_back(a) == comparison[size-1]);
    } else {
        assert(array_empty(a));
    }
    array_iter(a, it) {
        ref = array_at(a, size - j);
        assert(ref && *ref == comparison[i]);
        assert(array_index(a, i) == comparison[i]);
        assert(*it == comparison[i++]); --j;
    }
    assert(i == size);
    i = size - 1;
    j = 1;
    array_riter(a, it) {
        ref = array_at(a, size - j);
        assert(ref && *ref == comparison[i]);
        assert(array_index(a, i) == comparison[i]);
        assert(*it == comparison[i--]); ++j;
    }
    assert(i == UINT_MAX);
}

void compare_strs(Array_str *a, char **comparison, unsigned size) {
    unsigned i = 0, j = size;
    char **it, **ref;
    assert(array_size(a) == size);
    if (size) {
        assert(!array_empty(a));
        assert(streq(*array_front(a), comparison[0]) && streq(*array_back(a), comparison[size-1]));
    } else {
        assert(array_empty(a));
    }
    array_iter(a, it) {
        ref = array_at(a, size - j);
        assert(ref && streq(*ref, comparison[i]));
        assert(streq(array_index(a, i), comparison[i]));
        assert(streq(*it, comparison[i++])); --j;
    }
    assert(i == size);
    i = size - 1;
    j = 1;
    array_riter(a, it) {
        ref = array_at(a, size - j);
        assert(ref && streq(*ref, comparison[i]));
        assert(streq(array_index(a, i), comparison[i]));
        assert(streq(*it, comparison[i--])); ++j;
    }
    assert(i == UINT_MAX);
}

void test_empty_init(void) {
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    compare_ints(ai, ints, 0);
    compare_strs(as, strs, 0);
    array_free(int, ai);
    array_free(str, as);
}

void test_init_repeatingValue(void) {
    int c1[] = {5, 5};
    char *c2[] = {"005", "005"};
    Array_int *ai = array_new_repeatingValue(int, 0, 5);
    Array_str *as;
    compare_ints(ai, c1, 0);
    array_free(int, ai);

    ai = array_new_repeatingValue(int, 1, 5);
    as = array_new_repeatingValue(str, 1, "005");
    compare_ints(ai, c1, 1);
    compare_strs(as, c2, 1);
    array_free(int, ai);
    array_free(str, as);

    ai = array_new_repeatingValue(int, 2, 5);
    as = array_new_repeatingValue(str, 2, "005");
    compare_ints(ai, c1, 2);
    compare_strs(as, c2, 2);
    array_free(int, ai);
    array_free(str, as);
}

void test_init_fromArray(void) {
    Array_int *ai = array_new_fromArray(int, NULL, 5);
    Array_str *as;
    compare_ints(ai, ints, 0);
    array_free(int, ai);

    ai = array_new_fromArray(int, ints, 0);
    compare_ints(ai, ints, 0);
    array_free(int, ai);

    ai = array_new_fromArray(int, ints, 1);
    as = array_new_fromArray(str, strs, 1);
    compare_ints(ai, ints, 1);
    compare_strs(as, strs, 1);
    array_free(int, ai);
    array_free(str, as);

    ai = array_new_fromArray(int, ints, 2);
    as = array_new_fromArray(str, strs, 2);
    compare_ints(ai, ints, 2);
    compare_strs(as, strs, 2);
    array_free(int, ai);
    array_free(str, as);
}

void test_create_copy(void) {
    Array_int *b = array_new_fromArray(int, ints, 0), *ai;
    Array_str *c = array_new_fromArray(str, strs, 5), *as;
    ai = array_createCopy(int, b);
    compare_ints(ai, ints, 0);
    array_free(int, ai);
    array_free(int, b);
    b = array_new_fromArray(int, ints, 5);

    ai = array_createCopy(int, b);
    as = array_createCopy(str, c);
    compare_ints(ai, ints, 5);
    compare_strs(as, strs, 5);
    array_free(int, ai);
    array_free(str, as);
    array_free(int, b);
    array_free(str, c);
}

void test_push_back(void) {
    int i;
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    for (i = 0; i < 3; ++i) {
        array_push_back(int, ai, ints[i]);
        array_push_back(str, as, strs[i]);
    }
    compare_ints(ai, ints, 3);
    compare_strs(as, strs, 3);
    array_free(int, ai);
    array_free(str, as);
}

void test_pop_back(void) {
    unsigned i = 2;
    Array_int *ai = array_new_fromArray(int, ints, 3);
    Array_str *as = array_new_fromArray(str, strs, 3);
    while (!array_empty(ai)) {
        array_pop_back(int, ai);
        compare_ints(ai, ints, i--);
    }
    array_pop_back(int, ai);
    i = 2;
    while (!array_empty(as)) {
        array_pop_back(str, as);
        compare_strs(as, strs, i--);
    }
    array_pop_back(str, as);
    compare_ints(ai, ints, 0);
    compare_strs(as, strs, 0);
    array_free(int, ai);
    array_free(str, as);
}

void test_resize(void) {
    int c1[] = {15,15,15,20,20};
    char *c2[] = {"015","015","015","020","020"};
    unsigned i = 2;
    Array_int *ai = array_new_fromArray(int, ints, 3);
    Array_str *as = array_new_fromArray(str, strs, 3);
    while (!array_empty(ai)) {
        array_resize(int, ai, i);
        compare_ints(ai, ints, i--);
    }
    array_resize(int, ai, 0);
    i = 2;
    while (!array_empty(as)) {
        array_resize(str, as, i);
        compare_strs(as, strs, i--);
    }
    array_resize(str, as, 0);
    compare_ints(ai, ints, 0);
    compare_strs(as, strs, 0);

    array_resize_usingValue(int, ai, 3, 15);
    array_resize_usingValue(str, as, 3, "015");
    array_resize_usingValue(int, ai, 5, 20);
    array_resize_usingValue(str, as, 5, "020");
    array_resize(int, ai, 5);
    array_resize(str, as, 5);
    compare_ints(ai, c1, 5);
    compare_strs(as, c2, 5);
    array_free(int, ai);
    array_free(str, as);
}

void test_reserve(void) {
    Array_str *as = array_new(str);
    assert(array_capacity(as) == 8);
    array_reserve(str, as, 4);
    assert(array_capacity(as) == 8);
    array_reserve(str, as, 9);
    assert(array_capacity(as) == 16);
    array_free(str, as);
}

void test_shrink(void) {
    Array_str *a = array_new(str);
    array_shrink_to_fit(str, a);
    assert(array_capacity(a) == 8);
    array_insert_fromArray(str, a, 0, strs, 8);
    array_shrink_to_fit(str, a);
    assert(array_capacity(a) == 8);
    array_insert_fromArray(str, a, a->size - 1, &strs[8], 1);
    assert(array_capacity(a) == 16);
    array_shrink_to_fit(str, a);
    assert(array_capacity(a) == 9);
    array_free(str, a);
}

void test_insert_element(void) {
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    assert(array_insert(int, ai, ai->size - 1, 10) == ARRAY_ERROR);
    assert(array_insert(int, ai, array_size(ai), 10) == 0);
    assert(array_insert(str, as, array_size(as), "010") == 0);
    assert(array_insert(int, ai, 0, 0) == 0);
    assert(array_insert(str, as, 0, "000") == 0);
    assert(array_insert(int, ai, ai->size - 1, 5) == 1);
    assert(array_insert(str, as, as->size - 1, "005") == 1);
    assert(array_insert(int, ai, array_size(ai), 15) == 3);
    assert(array_insert(str, as, array_size(as), "015") == 3);
    compare_ints(ai, ints, 4);
    compare_strs(as, strs, 4);
    array_free(int, ai);
    array_free(str, as);
}

void test_insert_repeatedValue(void) {
    int c1[] = {0,0,5,5,10,15,15};
    char *c2[] = {"000","000","005","005","010","015","015"};
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    assert(array_insert_repeatingValue(int, ai, array_size(ai), 0, 1) == ARRAY_ERROR);
    assert(array_insert_repeatingValue(int, ai, ai->size - 1, 1, 1) == ARRAY_ERROR);
    assert(array_insert_repeatingValue(int, ai, array_size(ai), 1, 10) == 0);
    assert(array_insert_repeatingValue(str, as, array_size(as), 1, "010") == 0);
    assert(array_insert_repeatingValue(int, ai, 0, 2, 0) == 0);
    assert(array_insert_repeatingValue(str, as, 0, 2, "000") == 0);
    assert(array_insert_repeatingValue(int, ai, ai->size - 1, 2, 5) == 2);
    assert(array_insert_repeatingValue(str, as, as->size - 1, 2, "005") == 2);
    assert(array_insert_repeatingValue(int, ai, array_size(ai), 2, 15) == 5);
    assert(array_insert_repeatingValue(str, as, array_size(as), 2, "015") == 5);
    compare_ints(ai, c1, 7);
    compare_strs(as, c2, 7);
    array_free(int, ai);
    array_free(str, as);
}

void test_insert_fromArray(void) {
    int arr1[4][2] = {{10,10},{0,0},{5,5},{15,15}};
    char *arr2[4][2] = {{"010","010"},{"000","000"},{"005","005"},{"015","015"}};
    int c1[] = {0,0,5,5,10,15,15};
    char *c2[] = {"000","000","005","005","010","015","015"};
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    assert(array_insert_fromArray(int, ai, 0, NULL, 2) == ARRAY_ERROR);
    assert(array_insert_fromArray(int, ai, 0, ints, 0) == ARRAY_ERROR);
    assert(array_insert_fromArray(int, ai, ai->size - 1, ints, 1) == ARRAY_ERROR);
    assert(array_insert_fromArray(int, ai, array_size(ai), arr1[0], 1) == 0);
    assert(array_insert_fromArray(str, as, array_size(as), arr2[0], 1) == 0);
    assert(array_insert_fromArray(int, ai, 0, arr1[1], 2) == 0);
    assert(array_insert_fromArray(str, as, 0, arr2[1], 2) == 0);
    assert(array_insert_fromArray(int, ai, ai->size - 1, arr1[2], 2) == 2);
    assert(array_insert_fromArray(str, as, as->size - 1, arr2[2], 2) == 2);
    assert(array_insert_fromArray(int, ai, array_size(ai), arr1[3], 2) == 5);
    assert(array_insert_fromArray(str, as, array_size(as), arr2[3], 2) == 5);
    compare_ints(ai, c1, 7);
    compare_strs(as, c2, 7);
    array_free(int, ai);
    array_free(str, as);
}

void test_remove_element(void) {
    int c1[] = {5,15};
    char *c2[] = {"005","015"};
    Array_int *ai = array_new_fromArray(int, ints, 5);
    Array_str *as = array_new_fromArray(str, strs, 5);

    assert(array_erase(int, ai, 5, 1) == ARRAY_ERROR);
    assert(array_erase(int, ai, 4, 0) == ARRAY_ERROR);
    assert(array_erase(int, ai, 0, 1) == 0);
    assert(array_erase(str, as, 0, 1) == 0);
    assert(array_erase(int, ai, ai->size - 1, 1) == 3);
    assert(array_erase(str, as, as->size - 1, 1) == 3);
    assert(array_erase(int, ai, 1, 1) == 1);
    assert(array_erase(str, as, 1, 1) == 1);
    compare_ints(ai, c1, 2);
    compare_strs(as, c2, 2);
    array_free(int, ai);
    array_free(str, as);
}

void test_erase_elements(void) {
    int c1[] = {10, 15, 30, 35};
    char *c2[] = {"010","015","030","035"};
    Array_int *ai = array_new_fromArray(int, ints, 10);
    Array_str *as = array_new_fromArray(str, strs, 10);

    assert(array_erase(int, ai, 0, 2) == 0);
    assert(array_erase(str, as, 0, 2) == 0);
    assert(array_erase(int, ai, ai->size - 2, DS_ARG_NOT_APPLICABLE) == 6);
    assert(array_erase(str, as, as->size - 2, DS_ARG_NOT_APPLICABLE) == 6);
    assert(array_erase(int, ai, 2, 2) == 2);
    assert(array_erase(str, as, 2, 2) == 2);
    compare_ints(ai, c1, 4);
    compare_strs(as, c2, 4);
    array_free(int, ai);
    array_free(str, as);
}

void test_subarr(void) {
    int c1[2][9] = {{40,35,30,25,20,15,10,5,0},{40,30,20,10,0}};
    char *c2[2][9] = {{"040","035","030","025","020","015","010","005","000"},{"040","030","020","010","000"}};
    Array_int *ai1 = array_new_fromArray(int, ints, 10), *ai2 = array_new(int);
    Array_str *as1 = array_new_fromArray(str, strs, 10), *as2;

    assert(array_subarr(int, ai1, 0, 0, 1) == NULL);
    assert(array_subarr(int, ai2, ai2->size - 1, DS_ARG_NOT_APPLICABLE, 0) == NULL);
    array_free(int, ai2);
    ai2 = array_subarr(int, ai1, 0, 200, 1);
    as2 = array_subarr(str, as1, 0, 200, 1);
    compare_ints(ai2, ints, 10);
    compare_strs(as2, strs, 10);
    array_free(int, ai2);
    array_free(str, as2);
    ai2 = array_subarr(int, ai1, ai1->size - 2, 200, -1);
    as2 = array_subarr(str, as1, as1->size - 2, 200, -1);
    compare_ints(ai2, c1[0], 9);
    compare_strs(as2, c2[0], 9);
    array_free(int, ai2);
    array_free(str, as2);
    ai2 = array_subarr(int, ai1, 0, 5, 0);
    as2 = array_subarr(str, as1, 0, 5, 0);
    compare_ints(ai2, ints, 5);
    compare_strs(as2, strs, 5);
    array_free(int, ai2);
    array_free(str, as2);
    ai2 = array_subarr(int, ai1, ai1->size -2, 5, -1);
    as2 = array_subarr(str, as1, as1->size - 2, 5, -1);
    compare_ints(ai2, c1[0], 5);
    compare_strs(as2, c2[0], 5);
    array_free(int, ai2);
    array_free(str, as2);
    ai2 = array_subarr(int, ai1, ai1->size - 2, DS_ARG_NOT_APPLICABLE, -2);
    as2 = array_subarr(str, as1, as1->size - 2, DS_ARG_NOT_APPLICABLE, -2);
    compare_ints(ai2, c1[1], 5);
    compare_strs(as2, c2[1], 5);
    array_free(int, ai2);
    array_free(str, as2);
    array_free(int, ai1);
    array_free(str, as1);
}

void test_find(void) {
    Array_int *ai = array_new_fromArray(int, ints, 10);
    Array_str *as = array_new_fromArray(str, strs, 10);
    assert(*array_find(int, ai, 30) == 30);
    assert(streq(*array_find(str, as, "030"), "030"));
    assert(*array_find(int, ai, 0) == 0);
    assert(streq(*array_find(str, as, "000"), "000"));
    assert(*array_find(int, ai, 45) == 45);
    assert(streq(*array_find(str, as, "045"), "045"));
    assert(array_find(int, ai, 11) == NULL);
    assert(array_find(str, as, "011") == NULL);
    assert(array_find(int, ai, -1) == NULL);
    assert(array_find(str, as, "...") == NULL);
    assert(array_find(int, ai, 50) == NULL);
    assert(array_find(str, as, "050") == NULL);
    array_free(str, as);
    array_free(int, ai);
}

void test_merge(void) {
    int a1[] = {10,20,30,40,50}, c1[] = {5,10,10,15,20,20,25,30,40,50};
    char *a2[] = {"010","020","030","040","050"}, *c2[] = {"005","010","010","015","020","020","025","030","040","050"};
    Array_str *as;
    Array_int *ai;

    ai = merge_array(int, NULL, &ints[6], a1, &a1[5]);
    compare_ints(ai, a1, 5);
    array_free(int, ai);
    ai = merge_array(int, a1, &a1[5], NULL, &ints[6]);
    compare_ints(ai, a1, 5);
    array_free(int, ai);
    ai = merge_array(int, NULL, &ints[6], NULL, &ints[6]);
    compare_ints(ai, c1, 0);
    array_free(int, ai);

    ai = merge_array(int, &ints[1], &ints[6], a1, &a1[5]);
    as = merge_array(str, &strs[1], &strs[6], a2, &a2[5]);
    compare_ints(ai, c1, 10);
    compare_strs(as, c2, 10);
    array_free(int, ai);
    array_free(str, as);
}

void test_sort(void) {
    int ints_rand[] = {200,25,220,120,5,205,50,15,60,235,10,70,130,105,185,225,90,30,155,100,150,0,95,170,190,
125,210,75,45,160,175,145,55,230,35,65,110,140,115,20,215,85,195,240,245,135,80,180,40,165};
    char *strs_rand[] = {"200","025","220","120","005","205","050","015","060","235","010","070","130","105",
"185","225","090","030","155","100","150","000","095","170","190","125","210","075","045","160","175",
"145","055","230","035","065","110","140","115","020","215","085","195","240","245","135","080","180",
"040","165"};
    Array_int *ai = array_new_fromArray(int, ints_rand, 50);
    Array_str *as = array_new_fromArray(str, strs_rand, 50);
    array_sort(int, ai);
    array_sort(str, as);
    compare_ints(ai, ints, 50);
    compare_strs(as, strs, 50);
    array_free(str, as);
    array_free(int, ai);
}

void test_union(void) {
    int c1[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060","065","070"};
    Array_int *ri;
    Array_str *rs;

    ri = array_union(int, NULL, &ints[15], ints, &ints[10]);
    compare_ints(ri, ints, 10);
    array_free(int, ri);
    ri = array_union(int, ints, &ints[10], NULL, &ints[15]);
    compare_ints(ri, ints, 10);
    array_free(int, ri);
    ri = array_union(int, NULL, &ints[15], NULL, &ints[15]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);

    ri = array_union(int, ints, &ints[10], &ints[5], &ints[15]);
    rs = array_union(str, strs, &strs[10], &strs[5], &strs[15]);
    compare_ints(ri, c1, 15);
    compare_strs(rs, c2, 15);
    array_free(int, ri);
    array_free(str, rs);
}

void test_intersection(void) {
    int c1[] = {25,30,35,40,45};
    char *c2[] = {"025","030","035","040","045"};
    Array_int *ri;
    Array_str *rs;

    ri = array_intersection(int, NULL, &ints[15], ints, &ints[10]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);
    ri = array_intersection(int, ints, &ints[10], NULL, &ints[15]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);
    ri = array_intersection(int, NULL, &ints[15], NULL, &ints[15]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);

    ri = array_intersection(int, ints, &ints[10], &ints[5], &ints[15]);
    rs = array_intersection(str, strs, &strs[10], &strs[5], &strs[15]);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    array_free(int, ri);
    array_free(str, rs);
}

void test_difference(void) {
    int c1[] = {0,5,10,15,20};
    char *c2[] = {"000","005","010","015","020"};
    Array_int *ri;
    Array_str *rs;

    ri = array_difference(int, NULL, &ints[15], ints, &ints[10]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);
    ri = array_difference(int, ints, &ints[10], NULL, &ints[15]);
    compare_ints(ri, ints, 10);
    array_free(int, ri);
    ri = array_difference(int, NULL, &ints[15], NULL, &ints[15]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);

    ri = array_difference(int, ints, &ints[10], &ints[5], &ints[15]);
    rs = array_difference(str, strs, &strs[10], &strs[5], &strs[15]);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    array_free(int, ri);
    array_free(str, rs);
}

void test_symmetric_difference(void) {
    int c1[] = {0,5,10,15,20,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","050","055","060","065","070"};
    Array_int *ri;
    Array_str *rs;

    ri = array_symmetric_difference(int, NULL, &ints[15], ints, &ints[10]);
    compare_ints(ri, ints, 10);
    array_free(int, ri);
    ri = array_symmetric_difference(int, ints, &ints[10], NULL, &ints[15]);
    compare_ints(ri, ints, 10);
    array_free(int, ri);
    ri = array_symmetric_difference(int, NULL, &ints[15], NULL, &ints[15]);
    compare_ints(ri, ints, 0);
    array_free(int, ri);

    ri = array_symmetric_difference(int, ints, &ints[10], &ints[5], &ints[15]);
    rs = array_symmetric_difference(str, strs, &strs[10], &strs[5], &strs[15]);
    compare_ints(ri, c1, 10);
    compare_strs(rs, c2, 10);
    array_free(int, ri);
    array_free(str, rs);
}

void test_includes(void) {
    assert(array_includes(int, NULL, &ints[6], NULL, &ints[6]));
    assert(!array_includes(int, NULL, &ints[6], &ints[5], &ints[11]));
    assert(array_includes(int, &ints[5], &ints[11], NULL, &ints[6]));

    assert(array_includes(int, ints, &ints[6], ints, &ints[6]));
    assert(array_includes(str, strs, &strs[6], strs, &strs[6]));
    assert(array_includes(int, &ints[5], &ints[11], &ints[5], &ints[11]));
    assert(array_includes(str, &strs[5], &strs[11], &strs[5], &strs[11]));
    assert(array_includes(int, ints, &ints[11], ints, &ints[11]));
    assert(array_includes(str, strs, &strs[11], strs, &strs[11]));

    assert(!array_includes(int, ints, &ints[6], ints, &ints[11]));
    assert(array_includes(int, ints, &ints[11], ints, &ints[6]));
    assert(!array_includes(str, strs, &strs[6], strs, &strs[11]));
    assert(array_includes(str, strs, &strs[11], strs, &strs[6]));
    assert(!array_includes(int, &ints[5], &ints[11], ints, &ints[11]));
    assert(array_includes(int, ints, &ints[11], &ints[5], &ints[11]));
    assert(!array_includes(str, &strs[5], &strs[11], strs, &strs[11]));
    assert(array_includes(str, strs, &strs[11], &strs[5], &strs[11]));

    assert(!array_includes(int, ints, &ints[6], &ints[5], &ints[11]));
    assert(!array_includes(int, &ints[5], &ints[11], ints, &ints[6]));
    assert(!array_includes(str, strs, &strs[6], &strs[5], &strs[11]));
    assert(!array_includes(str, &strs[5], &strs[11], strs, &strs[6]));
}

int main(void) {    
    test_empty_init();
    test_init_repeatingValue();
    test_init_fromArray();
    test_create_copy();
    test_push_back();
    test_pop_back();
    test_resize();
    test_reserve();
    test_shrink();
    test_insert_element();
    test_insert_repeatedValue();
    test_insert_fromArray();
    test_remove_element();
    test_erase_elements();
    test_subarr();
    test_find();
    test_merge();
    test_sort();
    test_union();
    test_intersection();
    test_difference();
    test_symmetric_difference();
    test_includes();
    return 0;
}
