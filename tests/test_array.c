#include "array.h"
#include <assert.h>

gen_array_withalg(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)
gen_array_withalg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_matrix(int, int)

void compare_ints(Array_int *a, int *comparison, int size) {
    assert(array_size(a) == size);
    int i = 0;
    int *it;
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
    assert(array_size(a) == size);
    int i = 0;
    char **it;
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
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);

    assert(array_size(ai) == 0 && array_size(as) == 0);
    assert(array_capacity(ai) == 8 && array_capacity(as) == 8);
    assert(array_empty(ai) && array_empty(as));
    assert(array_at(int, ai, 0) == NULL && array_at(str, as, 0) == NULL);
    assert(!array_front(ai) && !array_front(as));
    assert(!array_back(ai) && !array_back(as));

    compare_ints(ai, (int[]){}, 0);
    compare_strs(as, (char*[]){}, 0);

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
    compare_ints(ai, (int[]){0, 1}, 2);
    compare_strs(as, (char*[]){"0", "1"}, 2);

    array_pop_back(int, ai); array_pop_back(str, as);
    compare_ints(ai, (int[]){0}, 1);
    compare_strs(as, (char*[]){"0"}, 1);

    array_clear(int, ai); array_clear(str, as);
    compare_ints(ai, (int[]){}, 0);
    compare_strs(as, (char*[]){}, 0);

    array_free(int, ai); array_free(str, as);
}

void test_resizing(void) {
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);

    array_reserve(int, ai, 4); array_reserve(str, as, 4);
    assert(array_capacity(ai) == 8 && array_capacity(as) == 8);

    array_reserve(int, ai, 9); array_reserve(str, as, 9);
    assert(array_capacity(ai) == 16 && array_capacity(as) == 16);

    array_resize_usingValue(int, ai, 10, 5);
    array_resize_usingValue(str, as, 10, "5");
    compare_ints(ai, (int[]){5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, 10);
    compare_strs(as, (char*[]){"5", "5", "5", "5", "5", "5", "5", "5", "5", "5"}, 10);

    array_resize(int, ai, 3); array_resize(str, as, 3);
    compare_ints(ai, (int[]){5, 5, 5}, 3);
    compare_strs(as, (char*[]){"5", "5", "5"}, 3);

    array_resize_usingValue(int, ai, 6, 10); array_resize_usingValue(str, as, 6, "10");
    compare_ints(ai, (int[]){5, 5, 5, 10, 10, 10}, 6);
    compare_strs(as, (char*[]){"5", "5", "5", "10", "10", "10"}, 6);
    array_free(int, ai); array_free(str, as);
}

void test_custom_initializers(void) {
    Array_int *ai = array_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    Array_str *as = array_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);
    compare_ints(ai, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
    compare_strs(as, (char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10);

    Array_int *ax = array_new_fromArray(int, array_front(ai), array_size(ai));
    Array_str *ay = array_new_fromArray(str, array_front(as), array_size(as));
    compare_ints(ax, array_front(ai), 10);
    compare_strs(ay, array_front(as), 10);
    array_free(int, ax); array_free(str, ay);

    ax = array_new_repeatingValue(int, 5, 5);
    ay = array_new_repeatingValue(str, "5", 5);
    compare_ints(ax, (int[]){5, 5, 5, 5, 5}, 5);
    compare_strs(ay, (char*[]){"5", "5", "5", "5", "5"}, 5);
    array_free(int, ax); array_free(str, ay);
    array_free(int, ai); array_free(str, as);
}

void test_insert(void) {
    Array_int *ai = array_new(int);
    Array_str *as = array_new(str);

    assert(array_insert(int, ai, array_size(ai), 2) == 0 && array_insert(str, as, array_size(as), "2") == 0); //2
    assert(array_insert(int, ai, array_size(ai), 4) == 1 && array_insert(str, as, array_size(as), "4") == 1); //2, 4
    assert(array_insert(int, ai, 0, 1) == 0 && array_insert(str, as, 0, "1") == 0); //1, 2, 4
    assert(array_insert(int, ai, 2, 3) == 2 && array_insert(str, as, 2, "3") == 2); //1, 2, 3, 4
    assert(array_insert(int, ai, array_size(ai), 5) == 4 && array_insert(str, as, array_size(as), "5") == 4); //1, 2, 3, 4, 5

    compare_ints(ai, (int[]){1, 2, 3, 4, 5}, 5);
    compare_strs(as, (char*[]){"1", "2", "3", "4", "5"}, 5);
    array_free(int, ai); array_free(str, as);
}

void test_insert_arr(void) {
    Array_int *ai1 = array_new(int);
    Array_int *ai2 = array_new_fromArray(int, ((int[]){0, 1, 2, 3, 4}), 5);
    Array_str *as1 = array_new(str);
    Array_str *as2 = array_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4"}), 5);

    assert(array_insert_fromArray(int, ai1, 0, array_at(int, ai2, 10), 5) == ARRAY_ERROR);
    assert(array_insert_fromArray(str, as1, 0, array_at(str, as2, 10), 5) == ARRAY_ERROR);

    assert(array_insert_fromArray(int, ai1, 0, array_at(int, ai2, 2), 0) == ARRAY_ERROR);
    assert(array_insert_fromArray(str, as1, 0, array_at(str, as2, 2), 0) == ARRAY_ERROR);

    assert(array_insert_fromArray(int, ai1, -1, array_front(ai2), 5) == 0);
    assert(array_insert_fromArray(str, as1, -1, array_front(as2), 5) == 0);
    compare_ints(ai1, array_front(ai2), 5);
    compare_strs(as1, array_front(as2), 5);

    assert(array_insert_fromArray(int, ai1, -4, array_front(ai2), 2) == 1);
    assert(array_insert_fromArray(str, as1, -4, array_front(as2), 2) == 1);
    compare_ints(ai1, (int[]){0, 0, 1, 1, 2, 3, 4}, 7);
    compare_strs(as1, (char*[]){"0", "0", "1", "1", "2", "3", "4"}, 7);

    array_free(int, ai1); array_free(int, ai2);
    array_free(str, as1); array_free(str, as2);
}

void test_erase(void) {
    Array_int *ai = array_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    Array_str *as = array_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);

    assert(array_erase(int, ai, 15, 2) == ARRAY_ERROR && array_erase(str, as, 15, 2) == ARRAY_ERROR);
    assert(array_erase(int, ai, 2, 0) == ARRAY_ERROR && array_erase(str, as, 2, 0) == ARRAY_ERROR);
    assert(array_erase(int, ai, 0, 2) == 0 && array_erase(str, as, 0, 2) == 0); //2, 3, 4, 5, 6, 7, 8, 9
    assert(array_erase(int, ai, -2, -1) == 6 && array_erase(str, as, -2, -1) == 6); //2, 3, 4, 5, 6, 7
    assert(array_erase(int, ai, 2, 2) == 2 && array_erase(str, as, 2, 2) == 2); //2, 3, 6, 7

    compare_ints(ai, (int[]){2, 3, 6, 7}, 4);
    compare_strs(as, (char*[]){"2", "3", "6", "7"}, 4);
    array_free(int, ai); array_free(str, as);
}

void test_shrink(void) {
    Array_int *a = array_new(int);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) == 8);

    array_insert_fromArray(int, a, -1, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) == 10);
    array_free(int, a);
}

void test_utility(void) {
    char *names[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
        "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd","polkitd",
        "chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};
    char *names_sorted[] = {"avahi","bin","chrisray","colord","daemon","dbus","ftp","geoclue","git",
        "http","lightdm","mail","nobody","polkitd","root","rtkit","systemd-coredump","systemd-journal-remote",
        "systemd-network","systemd-resolve","systemd-timesync","usbmux","uuidd"};

    Array_str *a = array_new_fromArray(str, names, 23);
    array_sort(str, a);
    compare_strs(a, names_sorted, 23);

    char **res = array_find(str, a, "root");
    assert(res && streq(*res, "root"));
    assert(array_find(str, a, "nope") == NULL);
    array_free(str, a);
}

void test_subarr(void) {
    Array_int *ai1 = array_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    Array_int *ai2 = array_subarr(int, ai1, 0, -1, 1);
    Array_str *as1 = array_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);
    Array_str *as2 = array_subarr(str, as1, 0, -1, 1);

    compare_ints(ai2, array_front(ai1), 10);
    compare_strs(as2, array_front(as1), 10);
    array_free(int, ai2); array_free(str, as2);

    ai2 = array_subarr(int, ai1, -2, -1, -1);
    as2 = array_subarr(str, as1, -2, -1, -1);
    compare_ints(ai2, (int[]){8, 7, 6, 5, 4, 3, 2, 1, 0}, 9);
    compare_strs(as2, (char*[]){"8", "7", "6", "5", "4", "3", "2", "1", "0"}, 9);
    array_free(int, ai2); array_free(str, as2);

    ai2 = array_subarr(int, ai1, -2, -1, -2);
    as2 = array_subarr(str, as1, -2, -1, -2);
    compare_ints(ai2, (int[]){8, 6, 4, 2, 0}, 5);
    compare_strs(as2, (char*[]){"8", "6", "4", "2", "0"}, 5);
    array_free(int, ai2); array_free(str, as2);
    array_free(int, ai1); array_free(str, as1);
}

void test_2d(void) {
    const int rows = 10, cols = 10;
    Array_2d_int *arr2d = matrix_new(int, rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *matrix_at(int, arr2d, i, j) = (i * 10) + j;
        }
    }

    Array_int **arrptr;
    int *iptr;
    int i = 0;

    array_iter(arr2d, arrptr) {
        array_iter(*arrptr, iptr) {
            assert(*iptr == i++);
        }
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            assert(matrix_index(arr2d, row, col) == ((row * 10) + col));
        }
    }

    matrix_free(int, arr2d);
}

void test_alg_funcs(void) {
    int firstI[] = {5,10,15,20,25};
    int secondI[] = {50,40,30,20,10};
    char *firstS[] = {"a5", "b0", "b5", "c0", "c5"};
    char *secondS[] = {"f0", "e0", "d0", "c0", "b0"};
    sort(int, secondI, 5); sort(str, secondS, 5);

    Array_int *ai = merge_array(int, firstI, &firstI[5], secondI, &secondI[5]);
    Array_str *as = merge_array(str, firstS, &firstS[5], secondS, &secondS[5]);
    compare_ints(ai, (int[]){5, 10, 10, 15, 20, 20, 25, 30, 40, 50}, 10);
    compare_strs(as, (char*[]){"a5", "b0", "b0", "b5", "c0", "c0", "c5", "d0", "e0", "f0"}, 10);
    array_free(int, ai); array_free(str, as);

    ai = set_union_array(int, firstI, &firstI[5], secondI, &secondI[5]);
    as = set_union_array(str, firstS, &firstS[5], secondS, &secondS[5]);
    compare_ints(ai, (int[]){5, 10, 15, 20, 25, 30, 40, 50}, 8);
    compare_strs(as, (char*[]){"a5", "b0", "b5", "c0", "c5", "d0", "e0", "f0"}, 8);
    array_free(int, ai); array_free(str, as);

    ai = set_intersection_array(int, firstI, &firstI[5], secondI, &secondI[5]);
    as = set_intersection_array(str, firstS, &firstS[5], secondS, &secondS[5]);
    compare_ints(ai, (int[]){10, 20}, 2);
    compare_strs(as, (char*[]){"b0", "c0"}, 2);
    array_free(int, ai); array_free(str, as);

    ai = set_difference_array(int, firstI, &firstI[5], secondI, &secondI[5]);
    as = set_difference_array(str, firstS, &firstS[5], secondS, &secondS[5]);
    compare_ints(ai, (int[]){5, 15, 25}, 3);
    compare_strs(as, (char*[]){"a5", "b5", "c5"}, 3);
    array_free(int, ai); array_free(str, as);

    ai = set_symmetric_difference_array(int, firstI, &firstI[5], secondI, &secondI[5]);
    as = set_symmetric_difference_array(str, firstS, &firstS[5], secondS, &secondS[5]);
    compare_ints(ai, (int[]){5, 15, 25, 30, 40, 50}, 6);
    compare_strs(as, (char*[]){"a5", "b5", "c5", "d0", "e0", "f0"}, 6);
    array_free(int, ai); array_free(str, as);

    int container[] = {5,10,15,20,25,30,35,40,45,50};
    int continent[] = {10, 20, 30, 40};
    assert(includes_array(int, container, &container[10], continent, &continent[4]));
}

int main(void) {    
    test_macros();
    test_resizing();
    test_custom_initializers();
    test_insert();
    test_insert_arr();
    test_erase();
    test_shrink();
    test_utility();
    test_subarr();
    test_2d();
    test_alg_funcs();
    return 0;
}
