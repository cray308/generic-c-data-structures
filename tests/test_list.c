#include "list.h"
#include <assert.h>

gen_list_withalg(int, int, ds_cmp_num_lt)

gen_list(str, char *, ds_cmp_str_lt)

gen_list(char, char, ds_cmp_num_lt)


char *names[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
                 "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd",
                 "polkitd","chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};

char *strs[] = {"Ten", "Nine", "Eight", "Seven", "Six", "Five",
                "Four", "Three", "Two", "One"};

int ints[] = {58490, 13829, 44828, 35725, 20384, 46283, 56369, 21807, 15590, 1520, 60285, 11387,
              34372, 49312, 52170, 30455, 52223, 41564, 53018, 3098, 25875, 45077, 36617, 58448,
              42128, 13655, 7077, 62936, 46937, 14427};

int ints_sorted[] = {1520, 3098, 5000, 7077, 10000, 11387, 13655, 13829, 14427, 15000, 15590, 20000,
                     20384, 21807, 25000, 25875, 30000, 30455, 34372, 35000, 35725, 36617, 41564,
                     42128, 44828, 45077, 46283, 46937, 49312, 52170, 52223, 53018, 56369, 58448,
                     58490, 60285, 62936};

static inline int testCond(int *val) {
    return (*val == 1);
}

void test_macros_insertion(void) {
    List_str *l = list_new(str);
    assert(list_size(l) == 0);
    assert(l->front == NULL);
    assert(l->back == NULL);

    list_push_front(str, l, names[0]);
    assert(list_size(l) == 1);
    char **ptr = list_front(l);
    assert(strcmp(*ptr, names[0]) == 0);

    list_push_back(str, l, names[1]);
    assert(list_size(l) == 2);
    ptr = list_back(l);
    assert(strcmp(*ptr, names[1]) == 0);

    ListEntry_str *it = l->front;
    int i = 0;
    for (; i < 10; ++i) {
        it = list_insert(str, l, it->next, strs[i]);
        assert(list_size(l) == i + 3);
    }

    assert(list_size(l) == 12);
    i = 0;
    list_iter(str, l, it) {
        if (i == 0) {
            assert(strcmp(iter_deref(LIST, it), names[0]) == 0);
        } else if (i == 11) {
            assert(strcmp(iter_deref(LIST, it), names[1]) == 0);
        } else {
            assert(strcmp(iter_deref(LIST, it), strs[i - 1]) == 0);
        }
        ++i;
    }

    i = 11;
    list_riter(str, l, it) {
        if (i == 0) {
            assert(strcmp(iter_deref(LIST, it), names[0]) == 0);
        } else if (i == 11) {
            assert(strcmp(iter_deref(LIST, it), names[1]) == 0);
        } else {
            assert(strcmp(iter_deref(LIST, it), strs[i - 1]) == 0);
        }
        --i;
    }

    list_clear(str, l);
    assert(list_size(l) == 0);
    list_free(str, l);
}

void test_custom_init(void) {
    int ints[] = {0, 1, 2, 3, 4};
    List_int *l = list_new_fromArray(int, ints, 5);
    assert(list_size(l) == 5);
    ListEntry_int *it;
    int i = 0;
    list_iter(int, l, it) {
        assert(iter_deref(LIST, it) == ints[i++]);
    }

    List_int *l2 = list_createCopy(int, l);
    assert(list_size(l2) == 5);
    i = 0;
    list_iter(int, l2, it) {
        assert(iter_deref(LIST, it) == ints[i++]);
    }
    list_free(int, l);
    list_free(int, l2);
}

void test_pop(void) {
    List_str *l = list_new_fromArray(str, strs, 10);

    list_pop_front(str, l);
    assert(list_size(l) == 9);
    char **ptr = list_front(l);
    assert(streq(*ptr, "Nine"));

    list_pop_back(str, l);
    assert(list_size(l) == 8);
    ptr = list_back(l);
    assert(streq(*ptr, "Two"));

    while (list_size(l) > 1) {
        list_pop_front(str, l);
    }
    assert(list_size(l) == 1);
    ptr = list_front(l);
    char **bptr = list_back(l);
    assert(bptr != NULL && ptr != NULL);
    assert(bptr == ptr);

    list_pop_front(str, l);
    assert(list_empty(l));

    ptr = list_front(l);
    assert(ptr == NULL);
    bptr = list_back(l);
    assert(bptr == NULL);
    list_free(str, l);
}

void test_insert_sorted(List_int *l) {
    for (int i = 10; i > 0; --i) {
        list_insert_sorted(int, l, i);
    }
    assert(l->size == 10);

    ListEntry_int *curr = NULL;
    ListEntry_int *prev = NULL;
    int count = 0;

    list_iter(int, l, curr) {
        if (prev == NULL) {
            prev = curr;
            count++;
            continue;
        }
        assert(iter_deref(LIST, prev) <= iter_deref(LIST, curr));
        prev = curr;
        count++;
    }
    assert(count == 10);
}

void test_reverse(List_int *l) {
    list_reverse(int, l);
    int i = 10;
    ListEntry_int *it;

    list_iter(int, l, it) {
        assert(i-- == iter_deref(LIST, it));
    }
    assert(i == 0);
}

void test_utility_funcs(void) {
    List_int *l = list_new_fromArray(int, ((int []){1, 2, 2, 2, 3, 3}), 6);

    list_unique(int, l);
    assert(list_size(l) == 3);

    int i = 1;
    ListEntry_int *it;
    list_iter(int, l, it) {
        assert(i++ == iter_deref(LIST, it));
    }

    i = 2;
    list_remove_value(int, l, i);
    assert(list_size(l) == 2);

    int *ptr = list_front(l);
    assert(*ptr == 1);

    ptr = list_back(l);
    assert(*ptr == 3);

    list_remove_if(int, l, testCond);
    assert(list_size(l) == 1);

    ptr = list_front(l);
    assert(*ptr == 3);

    list_free(int, l);
}

void test_sort(List_str *l) {
    assert(list_size(l) == 23);
    list_sort(str, l);

    ListEntry_str *curr = NULL;
    ListEntry_str *prev = NULL;

    list_iter(str, l, curr) {
        if (prev == NULL) {
            prev = curr;
            continue;
        }
        assert(strcmp(prev->data, curr->data) <= 0);
        prev = curr;
    }
}

void test_find_erase(List_str *l) {
    char *str = "nobody";
    ListEntry_str *res = list_find(str, l, str);
    assert(res != NULL);

    ListEntry_str *polkit = list_erase(str, l, res, res->next);
    assert(polkit != NULL);
    assert(list_size(l) == 22);

    list_erase(str, l, polkit, l->back);
    assert(list_size(l) == 13);
}

void test_insert_list(List_str *l) {
    List_str *l2 = list_new(str);
    ListEntry_str *it = NULL;
    for (int i = 0; i < 10; ++i) {
        it = list_insert(str, l2, it, strs[i]);
    }

    char *str = "chrisray";
    ListEntry_str *pos = list_find(str, l, str);
    str = "Four";
    ListEntry_str *start2 = list_find(str, l2, str);
    list_insert_fromList(str, l, pos, false, start2, NULL);
    assert(list_size(l) == 20);
    assert(list_size(l2) == 10);

    list_free(str, l2);
}

void test_string(void) {
    char mystr[] = "Hello there";
    List_char *l = list_new_fromArray(char, mystr, 11);
    assert(list_size(l) == 11);
    int i = 0;
    ListEntry_char *it;

    list_iter(char, l, it) {
        assert(iter_deref(LIST, it) == mystr[i++]);
    }
    
    list_free(char, l);
}

void test_merge(void) {
    const int len_sorted = 37;
    List_int *l = list_new_fromArray(int, ints, 30);
    List_int *l2 = list_new_fromArray(int, ((int[]){35000, 30000, 25000, 20000, 15000, 10000, 5000}), 7);

    list_sort(int, l);
    list_sort(int, l2);

    list_merge(int, l, l2);
    assert(list_size(l) == len_sorted);
    assert(list_size(l2) == 0);
    int i = 0;
    ListEntry_int *it;

    list_iter(int, l, it) {
        assert(iter_deref(LIST, it) == ints_sorted[i++]);
    }

    i = len_sorted - 1;
    list_riter(int, l, it) {
        assert(iter_deref(LIST, it) == ints_sorted[i--]);
    }
    list_free(int, l);
    list_free(int, l2);
}

void test_sublist(void) {
    List_int *l = list_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);

    List_int *l2 = list_sublist(int, l, NULL, NULL);
    assert(l2 == NULL);

    l2 = list_sublist(int, l, l->front, l->front);
    assert(l2 == NULL);

    l2 = list_sublist(int, l, l->front, NULL);
    assert(l2 && l2->size == l->size);
    int i = 0;
    ListEntry_int *it;
    list_iter(int, l2, it) {
        assert(iter_deref(LIST, it) == i++);
    }
    list_free(int, l2);

    ListEntry_int *e = list_find(int, l, 2);
    assert(e != NULL);

    l2 = list_sublist(int, l, e, NULL);
    assert(l2 && l2->size == 8);
    i = 2;
    list_iter(int, l2, it) {
        assert(iter_deref(LIST, it) == i++);
    }
    list_free(int, l2);
    list_free(int, l);
}

void test_splice(void) {
    int comparison1[] = {1, 10, 20, 30, 2, 3, 4};
    int comparison2[] = {1, 10, 20, 30, 3, 4};
    int comparison3[] = {2, 8, 12, 16, 1, 10, 20, 30, 3, 4};
    int comparison4[] = {2, 2, 8, 12, 16, 8, 12, 16, 1, 10, 20, 30, 3, 4};

    List_int *l1 = list_new(int);
    List_int *l2 = list_new(int);
    ListEntry_int *it;
    for (int i = 1; i<=4; ++i) {
        list_push_back(int, l1, i);
    }

    for (int i=1; i<=3; ++i) {
        list_push_back(int, l2, i * 10);
    }

    ListEntry_int *e = l1->front->next;
    list_splice(int, l1, e, l2);

    assert(list_empty(l2));
    assert(l1->size == 7);
    int i = 0;
    list_iter(int, l1, it) {
        assert(iter_deref(LIST, it) == comparison1[i++]);
    }

    list_splice_element(int, l2, l2->front, l1, e);
    assert(l1->size == 6);
    i = 0;
    list_iter(int, l1, it) {
        assert(iter_deref(LIST, it) == comparison2[i++]);
    }

    e = l1->front;
    iter_advance(LIST, int, e, 3);                      
    //e = l1->front->next->next->next;

    list_push_back(int, l2, 8);
    list_push_back(int, l2, 12);
    list_push_back(int, l2, 16);
    list_push_back(int, l2, 18);

    list_splice_range(int, l1, l1->front, l2, l2->front, l2->back);

    assert(l2->front == l2->back);
    assert(l2->size == 1);
    assert(l1->size == 10);
    int *ptr = list_front(l2);
    assert(*ptr == 18);

    i = 0;
    list_iter(int, l1, it) {
        assert(iter_deref(LIST, it) == comparison3[i++]);
    }
    list_push_back(int, l2, 2);
    list_push_back(int, l2, 8);
    list_push_back(int, l2, 12);
    list_push_back(int, l2, 16);

    list_splice_range(int, l1, l1->front->next, l2, l2->front->next, NULL);
    assert(l2->front == l2->back);
    assert(l2->size == 1);
    ptr = list_front(l2);
    assert(*ptr == 18);
    assert(l1->size == 14);
    i = 0;
    list_iter(int, l1, it) {
        assert(iter_deref(LIST, it) == comparison4[i++]);
    }
    assert(i == 14);
    i = 13;
    list_riter(int, l1, it) {
        assert(iter_deref(LIST, it) == comparison4[i--]);
    }
    assert(i == -1);
    list_free(int, l1);
    list_free(int, l2);
}

void test_alg_funcs(void) {
    List_int *first = list_new_fromArray(int, ((int[]){5,10,15,20,25}), 5);
    List_int *second = list_new_fromArray(int, ((int[]){10, 20, 30, 40, 50}), 5);
    ListEntry_int *it;

    List_int *res = set_union_list(int, first, second);
    int comparison2[] = {5, 10, 15, 20, 25, 30, 40, 50};
    int i = 0;
    list_iter(int, res, it) {
        assert(iter_deref(LIST, it) == comparison2[i++]);
    }
    assert(i == 8);
    list_free(int, res);

    res = set_intersection_list(int, first, second);
    int comparison3[] = {10, 20};
    i = 0;
    list_iter(int, res, it) {
        assert(iter_deref(LIST, it) == comparison3[i++]);
    }
    assert(i == 2);
    list_free(int, res);

    res = set_difference_list(int, first, second);
    int comparison4[] = {5, 15, 25};
    i = 0;
    list_iter(int, res, it) {
        assert(iter_deref(LIST, it) == comparison4[i++]);
    }
    assert(i == 3);
    list_free(int, res);

    res = set_symmetric_difference_list(int, first, second);
    int comparison5[] = {5, 15, 25, 30, 40, 50};
    i = 0;
    list_iter(int, res, it) {
        assert(iter_deref(LIST, it) == comparison5[i++]);
    }
    assert(i == 6);
    list_free(int, res);

    int container_arr[] = {5,10,15,20,25,30,35,40,45,50};
    int continent_arr[] = {10, 20, 30, 40};

    List_int *container = list_new_fromArray(int, container_arr, 10);
    List_int *continent = list_new_fromArray(int, continent_arr, 4);

    assert(includes_list(int, container, continent));
    list_free(int, container);
    list_free(int, continent);
    list_free(int, first);
    list_free(int, second);
}

int main(void) {
    test_macros_insertion();
    test_custom_init();
    test_pop();

    List_int *l = list_new(int);
    test_insert_sorted(l);
    test_reverse(l);
    list_free(int, l);

    test_utility_funcs();

    List_str *l2 = list_new_fromArray(str, names, 23);
    test_sort(l2);
    test_find_erase(l2);
    test_insert_list(l2);
    list_free(str, l2);
    test_string();
    test_merge();
    test_sublist();
    test_splice();
    test_alg_funcs();
    return 0;
}
