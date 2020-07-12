#include "array.h"
#include <assert.h>

char *strs[] = {"Ten", "Nine", "Eight", "Seven", "Six", "Five",
                "Four", "Three", "Two", "One"};

char *names[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
                 "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd",
                 "polkitd","chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};

char *words[] = {"religiosos", "literature", "uncleansed", "skittishly", "acceptable", "gametogeny",
                 "thereafter", "encourages", "reservable", "mustardier"};

#define LEN_STRS 10
#define LEN_NAMES 23
#define LEN_WORDS 10

void test_macros(void) {
    Array *a = array_new(&str_val_helper, ARR_INIT_EMPTY);
    // test macros with empty array

    assert(array_size(a) == 0);
    assert(array_capacity(a) != 0);
    assert(array_empty(a));

    char **ptr;
    ptr = array_at(a, 5);
    assert(!ptr);

    ptr = array_front(a);
    assert(!ptr);

    ptr = array_back(a);
    assert(!ptr);

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
    array_push_back(a, &strs[0]);
    array_push_back(a, &strs[1]);

    assert(array_size(a) == 2);
    assert(array_capacity(a) != 0);
    assert(!array_empty(a));

    ptr = array_at(a, 0);
    assert(ptr);
    assert(streq(*ptr, strs[0]));

    ptr = array_at(a, 1);
    assert(ptr);
    assert(streq(*ptr, strs[1]));

    ptr = array_at(a, 5);
    assert(!ptr);

    ptr = array_front(a);
    assert(ptr);
    assert(streq(*ptr, strs[0]));

    ptr = array_back(a);
    assert(ptr);
    assert(streq(*ptr, strs[1]));

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

    array_free(a);
}

void test_resizing(void) {
    Array *a = array_new(&int_helper, ARR_INIT_EMPTY);

    int old = array_capacity(a);

    array_reserve(a, old >> 1);
    assert(array_capacity(a) == old);

    array_reserve(a, old + 2);
    assert(array_capacity(a) > old);

    int value = 8;
    array_resize(a, 10, &value);
    assert(array_size(a) == 10);

    int *ptr;
    int count = 0;
    array_iter(a, ptr) {
        assert(*ptr == 8);
        count++;
    }
    assert(count == 10);

    array_resize(a, 3, NULL);
    assert(array_size(a) == 3);
    count = 0;
    array_iter(a, ptr) {
        assert(*ptr == 8);
        count++;
    }
    assert(count == 3);

    value = 2;
    array_resize(a, 6, &value);
    assert(array_size(a) == 6);
    count = 0;
    for (int i = 0; i < array_size(a); ++i) {
        ptr = array_at(a, i);
        if (i < 3) {
            assert(*ptr == 8);
        } else {
            assert(*ptr == 2);
        }
        ++count;
    }
    assert(count == 6);
    array_free(a);
}

void test_custom_initializers(void) {
    Array *a = array_new(&str_ptr_helper, ARR_INIT_BUILTIN, &words, 10);
    assert(array_size(a) == 10);

    char **ptr;
    int i = 0;
    array_iter(a, ptr) {
        assert(streq(*ptr, words[i++]));
    }

    Array *a2 = array_new(&str_ptr_helper, ARR_INIT_ARRAY, a);
    assert(array_size(a2) == 10);

    char **ptr2;
    i = 0;
    array_iter(a, ptr2) {
        ptr = array_at(a, i++);
        assert(streq(*ptr, *ptr2));
    }

    array_free(a);
    array_free(a2);
}

void test_insert(void) {
    Array *a = array_new(&int_helper, ARR_INIT_EMPTY);
    int value = 2;

    int rv = array_insert(a, -1, ARR_INSERT_SINGLE, &value);
    assert(rv == 0);

    value = 4;
    rv = array_insert(a, -1, ARR_INSERT_SINGLE, &value);
    assert(rv == 0);

    value = 1;
    rv = array_insert(a, 0, ARR_INSERT_SINGLE, &value);
    assert(rv == 0);

    value = 3;
    rv = array_insert(a, 2, ARR_INSERT_SINGLE, &value);
    assert(rv == 2);

    value = 5;
    rv = array_insert(a, ARRAY_END(a), ARR_INSERT_SINGLE, &value);
    assert(rv == array_size(a) - 1);

    array_free(a);
}

void test_insert_arr(void) {
    int vals[] = {0, 1, 2, 3, 4};
    Array *a1 = array_new(&int_helper, ARR_INIT_EMPTY);
    Array *a2 = array_new(&int_helper, ARR_INIT_BUILTIN, &vals, 5);
    assert(array_size(a2) == 5);

    int rv = array_insert(a1, 0, ARR_INSERT_ARRAY, a2, 10, 5);
    assert(rv == ARRAY_ERROR);

    rv = array_insert(a1, 0, ARR_INSERT_ARRAY, a2, 2, 0);
    assert(rv == ARRAY_ERROR);

    rv = array_insert(a1, -1, ARR_INSERT_ARRAY, a2, 0, 5);
    assert(rv == 0);
    assert(array_size(a1) == 5);
    assert(array_size(a2) == 5);

    array_clear(a2);
    rv = array_insert(a2, -1, ARR_INSERT_BUILTIN, &vals, 0, 5);
    assert(rv == 0);
    assert(array_size(a2) == 5);

    rv = array_insert(a1, -4, ARR_INSERT_ARRAY, a2, 0, 2);
    assert(rv == 1);

    assert(array_size(a1) == 7);

    int *ptr = array_at(a1, 0);
    assert(*ptr == 0);
    ptr = array_at(a1, 1);
    assert(*ptr == 0);
    ptr = array_at(a1, 2);
    assert(*ptr == 1);

    for (int i = 3; i < array_size(a1); ++i) {
        ptr = array_at(a1, i);
        assert(*ptr == i - 2);
    }

    array_pop_back(a1);
    ptr = array_back(a1);
    assert(*ptr == 3);

    array_clear(a1);
    assert(array_size(a1) == 0);

    array_free(a1);
    array_free(a2);
}

void test_erase(void) {
    Array *a = array_new(&str_ptr_helper, ARR_INIT_BUILTIN, &words, 10);

    int rv = array_erase(a, 15, 2);
    assert(rv == ARRAY_ERROR);
    rv = array_erase(a, 2, 0);
    assert(rv == ARRAY_ERROR);

    rv = array_erase(a, 0, 2);
    assert(rv == 0);

    char **ptr = array_at(a, 0);
    assert(streq(*ptr, words[2]));

    rv = array_erase(a, -2, -1);
    assert(rv == ARRAY_END(a));

    rv = array_erase(a, 2, 2);
    assert(rv == 2);
    assert(array_size(a) == 4);

    array_free(a);
}

void test_shrink(void) {
    Array *a = array_new(&int_helper, ARR_INIT_EMPTY);
    array_shrink_to_fit(a);
    assert(array_capacity(a) > array_size(a));

    int value = 5;
    array_push_back(a, &value);
    array_shrink_to_fit(a);
    assert(array_capacity(a) == 1);
    array_free(a);
}

void test_utility(void) {
    Array *a = array_new(&str_val_helper, ARR_INIT_BUILTIN, &names, 23);

    char **curr = NULL;
    char **prev = NULL;
    array_sort(a);

    for (int i = 1; i < array_size(a); ++i) {
        prev = array_at(a, i-1);
        curr = array_at(a, i);
        assert(strcmp(*prev, *curr) <= 0);
    }

    char *key = "root";
    char **res = array_find(a, &key);
    assert(streq(*res, key));

    key = "nope";
    res = array_find(a, &key);
    assert(res == NULL);
    array_free(a);
}

void test_subarr(void) {
    int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Array *a = array_new(&int_helper, ARR_INIT_BUILTIN, &vals, 10);
    Array *a2 = array_subarr(a, 0, -1, 1);
    assert(a2->size == a->size);
    int *ptr;
    int i = 0;
    array_iter(a2, ptr) {
        assert(*ptr == i++);
    }

    array_free(a2);
    a2 = array_subarr(a, -2, -1, -1);
    assert(a2->size == 9);
    i = 8;
    array_iter(a2, ptr) {
        assert(*ptr == i--);
    }
    array_free(a2);

    a2 = array_subarr(a, -2, -1, -2);
    assert(a2->size == 5);
    i = 8;
    array_iter(a2, ptr) {
        assert(*ptr == i);
        i -= 2;
    }
    array_free(a2);

    array_free(a);
}

void test_2d(void) {
    Array temp = {0, 0, int_helper, NULL};
    Array *arr2d = array_new(&vec_2d_helper, ARR_INIT_SIZE, 10, &temp);
    Array *row;

    for (int i = 0; i < 10; ++i) {
        int begin = (i * 10);
        row = array_at(arr2d, i);
        for (int j = begin; j < begin + 10; ++j) {
            array_push_back(row, &j);
        }
    }

    Array *arrptr;
    int *iptr;
    int i = 0;

    array_iter(arr2d, arrptr) {
        array_iter(arrptr, iptr) {
            assert(*iptr == i++);
        }
    }

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            iptr = matrix_at(arr2d, row, col);
            assert(*iptr == ((row * 10) + col));
        }
    }

    array_free(arr2d);
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
    return 0;
}
