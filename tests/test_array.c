#include "array.h"
#include <assert.h>

char *strs[] = {"Ten", "Nine", "Eight", "Seven", "Six", "Five",
                "Four", "Three", "Two", "One"};

char *names[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
                 "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd",
                 "polkitd","chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};

char *words[] = {"religiosos", "literature", "uncleansed", "skittishly", "acceptable", "gametogeny",
                 "thereafter", "encourages", "reservable", "mustardier"};

gen_array_withalg(str, char *, ds_cmp_str_lt)

gen_array_withalg(int, int, ds_cmp_num_lt)

gen_matrix(int, int)

#define LEN_STRS 10
#define LEN_NAMES 23
#define LEN_WORDS 10

void test_macros(void) {
    Array_str *a = array_new(str, ARR_INIT_EMPTY);
    // test macros with empty array

    assert(array_size(a) == 0);
    assert(array_capacity(a) != 0);
    assert(array_empty(a));

    char **ptr = NULL;
    assert(array_at(str, a, 5) == NULL);

    assert(!array_front(a));
    assert(!array_back(a));

    int i = 0;
    array_iter(a, ptr) {
        i++;
    }
    assert(i == 0);

    array_riter(a, ptr) {
        i++;
    }
    assert(i == 0);

    // test with non-empty array
    array_push_back(str, a, strs[0]);
    array_push_back(str, a, strs[1]);

    assert(array_size(a) == 2);
    assert(array_capacity(a) != 0);
    assert(!array_empty(a));

    assert(array_at(str, a, 0) != NULL);
    assert(streq(*array_at(str, a, 0), strs[0]));
    assert(streq(array_index(a, 0), strs[0]));

    assert(array_at(str, a, 1));
    assert(streq(*array_at(str, a, 1), strs[1]));
    assert(streq(array_index(a, 1), strs[1]));

    assert(!array_at(str, a, 5));

    assert(array_front(a));
    assert(streq(*array_front(a), strs[0]));

    assert(array_back(a));
    assert(streq(*array_back(a), strs[1]));

    i = 0;
    array_iter(a, ptr) {
        i++;
    }
    assert(i == 2);

    i = 0;
    array_riter(a, ptr) {
        i++;
    }
    assert(i == 2);

    array_free(str, a);
}

void test_resizing(void) {
    Array_int *a = array_new(int, ARR_INIT_EMPTY);

    int old = array_capacity(a);

    array_reserve(int, a, old >> 1);
    assert(array_capacity(a) == old);

    array_reserve(int, a, old + 2);
    assert(array_capacity(a) > old);

    int value = 8;
    array_resize(int, a, ARR_RESIZE_VALUE, 10, value);
    assert(array_size(a) == 10);

    int *ptr;
    int count = 0;
    array_iter(a, ptr) {
        assert(*ptr == 8);
        count++;
    }
    assert(count == 10);

    array_resize(int, a, ARR_RESIZE_EMPTY, 3);
    assert(array_size(a) == 3);
    count = 0;
    array_iter(a, ptr) {
        assert(*ptr == 8);
        count++;
    }
    assert(count == 3);

    value = 2;
    array_resize(int, a, ARR_RESIZE_VALUE, 6, value);
    assert(array_size(a) == 6);
    count = 0;
    for (int i = 0; i < array_size(a); ++i) {
        ptr = array_at(int, a, i);
        if (i < 3) {
            assert(*ptr == 8);
        } else {
            assert(*ptr == 2);
        }
        ++count;
    }
    assert(count == 6);
    array_free(int, a);
}

void test_custom_initializers(void) {
    Array_str *a = array_new(str, ARR_INIT_BUILTIN, words, 10);
    assert(array_size(a) == 10);

    char **ptr;
    int i = 0;
    array_iter(a, ptr) {
        assert(streq(*ptr, words[i++]));
    }

    Array_str *a2 = array_new(str, ARR_INIT_BUILTIN, array_front(a), array_size(a));
    assert(array_size(a2) == 10);

    char **ptr2;
    i = 0;
    array_iter(a, ptr2) {
        char *val = array_index(a, i++);
        assert(streq(val, *ptr2));
    }

    array_free(str, a);
    array_free(str, a2);
}

void test_insert(void) {
    Array_int *a = array_new(int, ARR_INIT_EMPTY);
    int value = 2;

    int rv = array_insert(int, a, -1, ARR_INSERT_SINGLE, value);
    assert(rv == 0);

    value = 4;
    rv = array_insert(int, a, -1, ARR_INSERT_SINGLE, value);
    assert(rv == 0);

    value = 1;
    rv = array_insert(int, a, 0, ARR_INSERT_SINGLE, value);
    assert(rv == 0);

    value = 3;
    rv = array_insert(int, a, 2, ARR_INSERT_SINGLE, value);
    assert(rv == 2);

    value = 5;
    rv = array_insert(int, a, ARRAY_END(a), ARR_INSERT_SINGLE, value);
    assert(rv == array_size(a) - 1);

    array_free(int, a);
}

void test_insert_arr(void) {
    int vals[] = {0, 1, 2, 3, 4};
    Array_int *a1 = array_new(int, ARR_INIT_EMPTY);
    Array_int *a2 = array_new(int, ARR_INIT_BUILTIN, vals, 5);
    assert(array_size(a2) == 5);

    int rv = array_insert(int, a1, 0, ARR_INSERT_BUILTIN, array_iterator(int, a2, 10), 5);
    assert(rv == ARRAY_ERROR);

    rv = array_insert(int, a1, 0, ARR_INSERT_BUILTIN, array_iterator(int, a2, 2), 0);
    assert(rv == ARRAY_ERROR);

    rv = array_insert(int, a1, -1, ARR_INSERT_BUILTIN, array_front(a2), 5);
    assert(rv == 0);
    assert(array_size(a1) == 5);
    assert(array_size(a2) == 5);

    array_clear(int, a2);
    rv = array_insert(int, a2, -1, ARR_INSERT_BUILTIN, vals, 5);
    assert(rv == 0);
    assert(array_size(a2) == 5);

    rv = array_insert(int, a1, -4, ARR_INSERT_BUILTIN, array_front(a2), 2);
    assert(rv == 1);

    assert(array_size(a1) == 7);
    int i = 0;
    assert(*array_at(int, a1, i++) == 0);
    assert(*array_at(int, a1, i++) == 0);
    assert(*array_at(int, a1, i++) == 1);

    for (i = 3; i < array_size(a1); ++i) {
        int val = array_index(a1, i);
        assert(val == i - 2);
    }

    array_pop_back(int, a1);
    int *ptr = array_back(a1);
    assert(*ptr == 3);

    array_clear(int, a1);
    assert(array_size(a1) == 0);

    array_free(int, a1);
    array_free(int, a2);
}

void test_erase(void) {
    Array_str *a = array_new(str, ARR_INIT_BUILTIN, words, 10);

    int rv = array_erase(str, a, 15, 2);
    assert(rv == ARRAY_ERROR);
    rv = array_erase(str, a, 2, 0);
    assert(rv == ARRAY_ERROR);

    rv = array_erase(str, a, 0, 2);
    assert(rv == 0);

    assert(streq(array_index(a, 0), words[2]));

    rv = array_erase(str, a, -2, -1);
    assert(rv == ARRAY_END(a));

    rv = array_erase(str, a, 2, 2);
    assert(rv == 2);
    assert(array_size(a) == 4);

    array_free(str, a);
}

void test_shrink(void) {
    Array_int *a = array_new(int, ARR_INIT_EMPTY);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) > array_size(a));

    int value = 5;
    array_push_back(int, a, value);
    array_shrink_to_fit(int, a);
    assert(array_capacity(a) == 1);
    array_free(int, a);
}

void test_utility(void) {
    Array_str *a = array_new(str, ARR_INIT_BUILTIN, names, 23);
    array_sort(str, a);

    for (int i = 1; i < array_size(a); ++i) {
        assert(strcmp(array_index(a, i-1), array_index(a, i)) <= 0);
    }

    char *key = "root";
    char **res = array_find(str, a, key);
    assert(streq(*res, key));

    key = "nope";
    res = array_find(str, a, key);
    assert(res == NULL);
    array_free(str, a);
}

void test_subarr(void) {
    int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Array_int *a = array_new(int, ARR_INIT_BUILTIN, vals, 10);
    Array_int *a2 = array_subarr(int, a, 0, -1, 1);
    assert(a2->size == a->size);
    int *ptr;
    int i = 0;
    array_iter(a2, ptr) {
        assert(*ptr == i++);
    }

    array_free(int, a2);
    a2 = array_subarr(int, a, -2, -1, -1);
    assert(a2->size == 9);
    i = 8;
    array_iter(a2, ptr) {
        assert(*ptr == i--);
    }
    array_free(int, a2);

    a2 = array_subarr(int, a, -2, -1, -2);
    assert(a2->size == 5);
    i = 8;
    array_iter(a2, ptr) {
        assert(*ptr == i);
        i -= 2;
    }
    array_free(int, a2);

    array_free(int, a);
}

void test_2d(void) {
    const int rows = 10, cols = 10;
    Matrix_int *arr2d = matrix_new(int, rows, cols);
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
    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    sort(int, second, 5);

    Array_int *res = merge_array(int, first, &first[5], second, &second[5]);
    int comparison1[] = {5, 10, 10, 15, 20, 20, 25, 30, 40, 50};
    for (size_t i = 0; i < res->size; ++i) {
        assert(res->arr[i] == comparison1[i]);
    }
    array_free(int, res);

    res = set_union_array(int, first, &first[5], second, &second[5]);
    int comparison2[] = {5, 10, 15, 20, 25, 30, 40, 50};
    for (size_t i = 0; i < res->size; ++i) {
        assert(res->arr[i] == comparison2[i]);
    }
    array_free(int, res);

    res = set_intersection_array(int, first, &first[5], second, &second[5]);
    int comparison3[] = {10, 20};
    for (size_t i = 0; i < res->size; ++i) {
        assert(res->arr[i] == comparison3[i]);
    }
    array_free(int, res);

    res = set_difference_array(int, first, &first[5], second, &second[5]);
    int comparison4[] = {5, 15, 25};
    for (size_t i = 0; i < res->size; ++i) {
        assert(res->arr[i] == comparison4[i]);
    }
    array_free(int, res);

    res = set_symmetric_difference_array(int, first, &first[5], second, &second[5]);
    int comparison5[] = {5, 15, 25, 30, 40, 50};
    for (size_t i = 0; i < res->size; ++i) {
        assert(res->arr[i] == comparison5[i]);
    }
    array_free(int, res);

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
