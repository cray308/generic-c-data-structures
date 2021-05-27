#include "array.h"
#include <assert.h>

gen_array_withalg(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)
gen_array_withalg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_matrix(int, int)

int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

void compare_ints(Array_int *a, int *comparison, int size) {
    int i = 0, *it;
    assert(array_size(a) == size);
    if (size) {
        assert(*array_front(a) == comparison[0] && *array_back(a) == comparison[size-1]);
    } else {
        assert(!array_front(a) && !array_back(a));
    }
    array_iter(a, it) {
        assert(*it == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    array_riter(a, it) {
        assert(*it == comparison[i--]);
    }
    assert(i == -1);
}

void compare_strs(Array_str *a, char **comparison, int size) {
    int i = 0; char **it;
    assert(array_size(a) == size);
    if (size) {
        assert(streq(*array_front(a), comparison[0]) && streq(*array_back(a), comparison[size-1]));
    } else {
        assert(!array_front(a) && !array_back(a));
    }
    array_iter(a, it) {
        assert(streq(*it, comparison[i++]));
    }
    assert(i == size);
    i = size - 1;
    array_riter(a, it) {
        assert(streq(*it, comparison[i--]));
    }
    assert(i == -1);
}

void test_macros(void) {
    int c1[] = {0,1};
    char *c2[] = {"0","1"};
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);

    assert(array_capacity(ai) == 8 && array_capacity(as) == 8);
    assert(array_empty(ai) && array_empty(as));
    assert(array_at(int, ai, 0) == NULL && array_at(str, as, 0) == NULL);
    compare_ints(ai, ints, 0);
    compare_strs(as, strs, 0);

    array_push_back(int, ai, 0); array_push_back(str, as, "0");
    array_push_back(int, ai, 1); array_push_back(str, as, "1");

    assert(array_capacity(ai) == 8 && array_capacity(as) == 8);
    assert(!array_empty(ai) && !array_empty(as));
    assert(array_at(int, ai, 0) && *array_at(int, ai, 0) == 0);
    assert(array_at(str, as, 0) && streq(*array_at(str, as, 0), "0"));
    assert(array_index(ai, 0) == 0 && streq(array_index(as, 0), "0"));
    assert(array_at(int, ai, 1) && *array_at(int, ai, 1) == 1);
    assert(array_at(str, as, 1) && streq(*array_at(str, as, 1), "1"));
    assert(array_index(ai, 1) == 1 && streq(array_index(as, 1), "1"));
    assert(!array_at(int, ai, 2) && !array_at(str, as, 2));
    compare_ints(ai, c1, 2);
    compare_strs(as, c2, 2);

    array_pop_back(int, ai); array_pop_back(str, as);
    compare_ints(ai, c1, 1);
    compare_strs(as, c2, 1);

    array_clear(int, ai); array_clear(str, as);
    compare_ints(ai, ints, 0);
    compare_strs(as, strs, 0);

    array_free(int, ai); array_free(str, as);
}

void test_resizing(void) {
    int c1[3][10] = {{5,5,5,5,5,5,5,5,5,5},{5,5,5},{5,5,5,10,10,10}};
    char *c2[3][10] = {{"5","5","5","5","5","5","5","5","5","5"},{"5","5","5"},{"5","5","5","10","10","10"}};
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);

    array_reserve(int, ai, 4); array_reserve(str, as, 4);
    assert(array_capacity(ai) == 8 && array_capacity(as) == 8);

    array_reserve(int, ai, 9); array_reserve(str, as, 9);
    assert(array_capacity(ai) == 16 && array_capacity(as) == 16);

    array_resize_usingValue(int, ai, 10, 5);
    array_resize_usingValue(str, as, 10, "5");
    compare_ints(ai, c1[0], 10);
    compare_strs(as, c2[0], 10);

    array_resize(int, ai, 3); array_resize(str, as, 3);
    compare_ints(ai, c1[1], 3);
    compare_strs(as, c2[1], 3);

    array_resize_usingValue(int, ai, 6, 10); array_resize_usingValue(str, as, 6, "10");
    compare_ints(ai, c1[2], 6);
    compare_strs(as, c2[2], 6);
    array_free(int, ai); array_free(str, as);
}

void test_custom_initializers(void) {
    int c1[] = {5,5,5,5,5};
    char *c2[] = {"5","5","5","5","5"};
    Array_int *ai = array_new_fromArray(int, ints, 10), *ax;
    Array_str *as = array_new_fromArray(str, strs, 10), *ay;
    compare_ints(ai, ints, 10);
    compare_strs(as, strs, 10);

    ax = array_new_fromArray(int, array_front(ai), 5);
    ay = array_new_fromArray(str, array_front(as), 5);
    compare_ints(ax, array_front(ai), 5);
    compare_strs(ay, array_front(as), 5);
    array_free(int, ax); array_free(str, ay);

    ax = array_new_repeatingValue(int, 5, 5);
    ay = array_new_repeatingValue(str, "5", 5);
    compare_ints(ax, c1, 5);
    compare_strs(ay, c2, 5);
    array_free(int, ax); array_free(str, ay);
    array_free(int, ai); array_free(str, as);
}

void test_insert(void) {
    int c1[] = {-1,0,1,2,3,4,5,6,7};
    char *c2[] = {"-1","0","1","2","3","4","5","6","7"};
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);
    /* Single element */
    assert(array_insert(int, ai, array_size(ai), 2) == 0 && array_insert(str, as, array_size(as), "2") == 0); /* 2 */
    assert(array_insert(int, ai, array_size(ai), 4) == 1 && array_insert(str, as, array_size(as), "4") == 1); /* 2, 4 */
    assert(array_insert(int, ai, 0, 1) == 0 && array_insert(str, as, 0, "1") == 0); /* 1, 2, 4 */
    assert(array_insert(int, ai, 2, 3) == 2 && array_insert(str, as, 2, "3") == 2); /* 1, 2, 3, 4 */
    assert(array_insert(int, ai, array_size(ai), 5) == 4 && array_insert(str, as, array_size(as), "5") == 4); /* 1, 2, 3, 4, 5 */

    compare_ints(ai, &c1[2], 5);
    compare_strs(as, &c2[2], 5);
    array_free(int, ai); array_free(str, as);

    ai = array_new(int), as = array_new(str);
    /* Insert from array */
    assert(array_insert_fromArray(int, ai, 0, NULL, 5) == ARRAY_ERROR);
    assert(array_insert_fromArray(str, as, 0, NULL, 5) == ARRAY_ERROR);
    assert(array_insert_fromArray(int, ai, 0, ints, 0) == ARRAY_ERROR);
    assert(array_insert_fromArray(str, as, 0, strs, 0) == ARRAY_ERROR);
    {
        int a1[] = {1,5}; char *a2[] = {"1","5"};
        assert(array_insert_fromArray(int, ai, array_size(ai), a1, 2) == 0);
        assert(array_insert_fromArray(str, as, array_size(as), a2, 2) == 0);
    }
    assert(array_insert_fromArray(int, ai, 1, &c1[3], 3) == 1);
    assert(array_insert_fromArray(str, as, 1, &c2[3], 3) == 1);
    assert(array_insert_fromArray(int, ai, -5, c1, 2) == 0);
    assert(array_insert_fromArray(str, as, -5, c2, 2) == 0);
    assert(array_insert_fromArray(int, ai, array_size(ai), &c1[7], 2) == 7);
    assert(array_insert_fromArray(str, as, array_size(as), &c2[7], 2) == 7);

    compare_ints(ai, c1, 9);
    compare_strs(as, c2, 9);
    array_free(int, ai); array_free(str, as);
}

void test_erase(void) {
    int c1[] = {10,15,30,35};
    char *c2[] = {"010","015","030","035"};
    Array_int *ai = array_new_fromArray(int, ints, 10);
    Array_str *as = array_new_fromArray(str, strs, 10);

    assert(array_erase(int, ai, 15, 2) == ARRAY_ERROR && array_erase(str, as, 15, 2) == ARRAY_ERROR);
    assert(array_erase(int, ai, 2, 0) == ARRAY_ERROR && array_erase(str, as, 2, 0) == ARRAY_ERROR);
    assert(array_erase(int, ai, 0, 2) == 0 && array_erase(str, as, 0, 2) == 0); /* 10, 15, 20, 25, 30, 35, 40, 45 */
    assert(array_erase(int, ai, -2, -1) == 6 && array_erase(str, as, -2, -1) == 6); /* 10, 15, 20, 25, 30, 35 */
    assert(array_erase(int, ai, 2, 2) == 2 && array_erase(str, as, 2, 2) == 2); /* 10, 15, 30, 35 */

    compare_ints(ai, c1, 4);
    compare_strs(as, c2, 4);
    array_free(int, ai); array_free(str, as);
}

void test_shrink(void) {
    Array_int *a = array_new(int);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) == 8);

    array_insert_fromArray(int, a, -1, ints, 10);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) == 10);
    array_free(int, a);
}

void test_utility(void) {
    int ints_rand[] = {200,25,220,120,5,205,50,15,60,235,10,70,130,105,185,225,90,30,155,100,150,0,95,170,190,
125,210,75,45,160,175,145,55,230,35,65,110,140,115,20,215,85,195,240,245,135,80,180,40,165};
    char *strs_rand[] = {"200","025","220","120","005","205","050","015","060","235","010","070","130","105",
"185","225","090","030","155","100","150","000","095","170","190","125","210","075","045","160","175",
"145","055","230","035","065","110","140","115","020","215","085","195","240","245","135","080","180",
"040","165"};

    Array_str *as = array_new_fromArray(str, strs_rand, 50);
    Array_int *ai = array_new_fromArray(int, ints_rand, 50);
    int *r1; char **r2;
    array_sort(str, as); array_sort(int, ai);
    compare_ints(ai, ints, 50);
    compare_strs(as, strs, 50);

    r1 = array_find(int, ai, 85);
    assert(r1 && *r1 == 85);
    r2 = array_find(str, as, "085");
    assert(r2 && streq(*r2, "085"));
    assert(array_find(int, ai, 250) == NULL && array_find(str, as, "250") == NULL);
    array_free(int, ai); array_free(str, as);
}

void test_subarr(void) {
    int c1[2][9] = {{40,35,30,25,20,15,10,5,0},{40,30,20,10,0}};
    char *c2[2][9] = {{"040","035","030","025","020","015","010","005","000"},{"040","030","020","010","000"}};
    Array_int *ai1 = array_new_fromArray(int, ints, 10);
    Array_int *ai2 = array_subarr(int, ai1, 0, -1, 1);
    Array_str *as1 = array_new_fromArray(str, strs, 10);
    Array_str *as2 = array_subarr(str, as1, 0, -1, 1);

    compare_ints(ai2, ints, 10);
    compare_strs(as2, strs, 10);
    array_free(int, ai2); array_free(str, as2);

    ai2 = array_subarr(int, ai1, -2, -1, -1);
    as2 = array_subarr(str, as1, -2, -1, -1);
    compare_ints(ai2, c1[0], 9);
    compare_strs(as2, c2[0], 9);
    array_free(int, ai2); array_free(str, as2);

    ai2 = array_subarr(int, ai1, -2, -1, -2);
    as2 = array_subarr(str, as1, -2, -1, -2);
    compare_ints(ai2, c1[1], 5);
    compare_strs(as2, c2[1], 5);
    array_free(int, ai2); array_free(str, as2);
    array_free(int, ai1); array_free(str, as1);
}

void test_2d(void) {
    Array_int **arrptr;
    int i;
    Array_2d_int *arr2d = matrix_new(int, 5, 10);
    for (i = 0; i < 5; ++i) {
        int j;
        for (j = 0; j < 10; ++j) {
            *matrix_at(int, arr2d, i, j) = ints[(i * 10) + j];
        }
    }

    i = 0;
    array_iter(arr2d, arrptr) {
        int *iptr;
        array_iter(*arrptr, iptr) {
            assert(*iptr == ints[i++]);
        }
    }
    assert(i == 50);

    for (i = 0; i < 5; ++i) {
        int j;
        for (j = 0; j < 10; ++j) {
            assert(matrix_index(arr2d, i, j) == ints[((i * 10) + j)]);
        }
    }
    matrix_free(int, arr2d);
}

void test_alg_funcs(void) {
    int c1[5][6] = {{0,5,10,10,15,20},{0,5,10,15,20},{10},{0,5},{0,5,15,20}};
    char *c2[5][6] = {{"000","005","010","010","015","020"},{"000","005","010","015","020"},{"010"},
    {"000","005"},{"000","005","015","020"}};
    int continent[] = {10,20,30,40};
    char *continent2[] = {"010","020","030","040"};

    Array_int *ai = merge_array(int, ints, &ints[3], &ints[2], &ints[5]);
    Array_str *as = merge_array(str, strs, &strs[3], &strs[2], &strs[5]);
    compare_ints(ai, c1[0], 6);
    compare_strs(as, c2[0], 6);
    array_free(int, ai); array_free(str, as);

    ai = set_union_array(int, ints, &ints[3], &ints[2], &ints[5]);
    as = set_union_array(str, strs, &strs[3], &strs[2], &strs[5]);
    compare_ints(ai, c1[1], 5);
    compare_strs(as, c2[1], 5);
    array_free(int, ai); array_free(str, as);

    ai = set_intersection_array(int, ints, &ints[3], &ints[2], &ints[5]);
    as = set_intersection_array(str, strs, &strs[3], &strs[2], &strs[5]);
    compare_ints(ai, c1[2], 1);
    compare_strs(as, c2[2], 1);
    array_free(int, ai); array_free(str, as);

    ai = set_difference_array(int, ints, &ints[3], &ints[2], &ints[5]);
    as = set_difference_array(str, strs, &strs[3], &strs[2], &strs[5]);
    compare_ints(ai, c1[3], 2);
    compare_strs(as, c2[3], 2);
    array_free(int, ai); array_free(str, as);

    ai = set_symmetric_difference_array(int, ints, &ints[3], &ints[2], &ints[5]);
    as = set_symmetric_difference_array(str, strs, &strs[3], &strs[2], &strs[5]);
    compare_ints(ai, c1[4], 4);
    compare_strs(as, c2[4], 4);
    array_free(int, ai); array_free(str, as);

    assert(includes_array(int, ints, &ints[11], continent, &continent[4]));
    assert(includes_array(str, strs, &strs[11], continent2, &continent2[4]));
}

int main(void) {    
    test_macros();
    test_resizing();
    test_custom_initializers();
    test_insert();
    test_erase();
    test_shrink();
    test_utility();
    test_subarr();
    test_2d();
    test_alg_funcs();
    return 0;
}
