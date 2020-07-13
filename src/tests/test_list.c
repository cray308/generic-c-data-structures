#include "list.h"
#include <assert.h>

List *l = NULL;

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

int testCond(const void *_val) {
    int *val = (int *) _val;
    return (*val == 1);
}

void test_macros_insertion(void) {
    List *l = list_new(&str_val_helper, LIST_INIT_EMPTY);
    assert(list_size(l) == 0);
    assert(l->front == NULL);
    assert(l->back == NULL);

    list_push_front(l, &names[0]);
    assert(list_size(l) == 1);
    char **ptr = list_front(l);
    assert(strcmp(*ptr, names[0]) == 0);

    list_push_back(l, &names[1]);
    assert(list_size(l) == 2);
    ptr = list_back(l);
    assert(strcmp(*ptr, names[1]) == 0);

    ListEntry *e = l->front;
    int i = 0;
    for (; i < 10; ++i) {
        e = list_insert(l, e->next, false, LIST_INSERT_SINGLE, &strs[i]);
        assert(list_size(l) == i + 3);
    }

    assert(list_size(l) == 12);
    i = 0;
    list_iter(l, ptr) {
        if (i == 0) {
            assert(strcmp(*ptr, names[0]) == 0);
        } else if (i == 11) {
            assert(strcmp(*ptr, names[1]) == 0);
        } else {
            assert(strcmp(*ptr, strs[i - 1]) == 0);
        }
        ++i;
    }

    i = 11;
    list_riter(l, ptr) {
        if (i == 0) {
            assert(strcmp(*ptr, names[0]) == 0);
        } else if (i == 11) {
            assert(strcmp(*ptr, names[1]) == 0);
        } else {
            assert(strcmp(*ptr, strs[i - 1]) == 0);
        }
        --i;
    }

    list_clear(l);
    assert(list_size(l) == 0);
    list_free(l);
}

void test_custom_init(void) {
    int ints[] = {0, 1, 2, 3, 4};
    List *l = list_new(&int_helper, LIST_INIT_BUILTIN, &ints, 5);
    assert(list_size(l) == 5);
    int *ptr;
    int i = 0;
    list_iter(l, ptr) {
        assert(*ptr == ints[i++]);
    }

    List *l2 = list_new(&int_helper, LIST_INIT_LIST, l, l->front, LIST_END);
    assert(list_size(l2) == 5);
    i = 0;
    list_iter(l2, ptr) {
        assert(*ptr == ints[i++]);
    }
    list_free(l);
    list_free(l2);
}

void test_pop(void) {
    List *l = list_new(&str_val_helper, LIST_INIT_BUILTIN, &strs, 10);

    list_pop_front(l);
    assert(list_size(l) == 9);
    char **ptr = list_front(l);
    assert(streq(*ptr, "Nine"));

    list_pop_back(l);
    assert(list_size(l) == 8);
    ptr = list_back(l);
    assert(streq(*ptr, "Two"));

    while (list_size(l) > 1) {
        list_pop_front(l);
    }
    assert(list_size(l) == 1);
    ptr = list_front(l);
    char **bptr = list_back(l);
    assert(bptr != NULL && ptr != NULL);
    assert(bptr == ptr);

    list_pop_front(l);
    assert(list_empty(l));

    ptr = list_front(l);
    assert(ptr == NULL);
    bptr = list_back(l);
    assert(bptr == NULL);
    list_free(l);
}

void test_insert_sorted(List *l) {
    for (int i = 10; i > 0; --i) {
        list_insert(l, NULL, true, LIST_INSERT_SINGLE, &i);
    }
    assert(l->size == 10);

    int *curr = NULL;
    int *prev = NULL;
    int count = 0;

    list_iter(l, curr) {
        if (prev == NULL) {
            prev = curr;
            count++;
            continue;
        }
        assert(*prev <= *curr);
        prev = curr;
        count++;
    }
    assert(count == 10);
}

void test_reverse(List *l) {
    list_reverse(l);
    int i = 10;
    int *ptr;

    list_iter(l, ptr) {
        assert(i-- == *ptr);
    }
    assert(i == 0);
}

void test_utility_funcs(void) {
    int ints[] = {1, 2, 2, 2, 3, 3};
    List *l = list_new(&int_helper, LIST_INIT_BUILTIN, &ints, 6);

    list_unique(l);
    assert(list_size(l) == 3);

    int i = 1;
    int *ptr;
    list_iter(l, ptr) {
        assert(i++ == *ptr);
    }

    i = 2;
    list_remove_value(l, &i);
    assert(list_size(l) == 2);

    ptr = list_front(l);
    assert(*ptr == 1);

    ptr = list_back(l);
    assert(*ptr == 3);

    list_remove_if(l, testCond);
    assert(list_size(l) == 1);

    ptr = list_front(l);
    assert(*ptr == 3);

    list_free(l);
}

void test_sort(List *l) {
    assert(list_size(l) == 23);
    list_sort(l);

    char **curr = NULL;
    char **prev = NULL;

    list_iter(l, curr) {
        if (prev == NULL) {
            prev = curr;
            continue;
        }
        assert(strcmp(*prev, *curr) <= 0);
        prev = curr;
    }
}

void test_find_erase(List *l) {
    char *str = "nobody";
    ListEntry *res = list_find(l, &str);
    assert(res != NULL);

    ListEntry *polkit = list_erase(l, res, res->next);
    assert(polkit != NULL);
    assert(list_size(l) == 22);

    list_erase(l, polkit, l->back);
    assert(list_size(l) == 13);
}

void test_insert_list(List *l) {
    List *l2 = list_new(&str_val_helper, LIST_INIT_EMPTY);
    ListEntry *iter = LIST_END;
    for (int i = 0; i < 10; ++i) {
        iter = list_insert(l2, iter, false, LIST_INSERT_SINGLE, &strs[i]);
    }

    char *str = "chrisray";
    ListEntry *pos = list_find(l, &str);
    str = "Four";
    ListEntry *start2 = list_find(l2, &str);
    list_insert(l, pos, false, LIST_INSERT_LIST, l2, start2, LIST_END);
    assert(list_size(l) == 20);
    assert(list_size(l2) == 10);

    list_free(l2);
}

void test_string(void) {
    DSHelper myhelper = {sizeof(char), NULL, NULL, NULL};
    const char mystr[] = "Hello there";
    List *l = list_new(&myhelper, LIST_INIT_BUILTIN, &mystr, 11);
    assert(list_size(l) == 11);
    int i = 0;
    char *ptr;

    list_iter(l, ptr) {
        assert(*ptr == mystr[i++]);
    }
    
    list_free(l);
}

void test_merge(void) {
    const int len_sorted = 37;
    int ints_2[] = {35000, 30000, 25000, 20000, 15000, 10000, 5000};
    List *l = list_new(&int_helper, LIST_INIT_BUILTIN, &ints, 30);
    List *l2 = list_new(&int_helper, LIST_INIT_BUILTIN, &ints_2, 7);

    list_sort(l);
    list_sort(l2);

    list_merge(l, l2);
    assert(list_size(l) == len_sorted);
    assert(list_size(l2) == 0);
    int i = 0;
    int *ptr;

    list_iter(l, ptr) {
        assert(*ptr == ints_sorted[i++]);
    }

    i = len_sorted - 1;
    list_riter(l, ptr) {
        assert(*ptr == ints_sorted[i--]);
    }
    list_free(l);
    list_free(l2);
}

void test_sublist(void) {
    int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    List *l = list_new(&int_helper, LIST_INIT_BUILTIN, &vals, 10);

    List *l2 = list_sublist(l, NULL, LIST_END);
    assert(l2 == NULL);

    l2 = list_sublist(l, l->front, l->front);
    assert(l2 == NULL);

    l2 = list_sublist(l, l->front, LIST_END);
    assert(l2 && l2->size == l->size);
    int i = 0;
    int *ptr;
    list_iter(l2, ptr) {
        assert(*ptr == i++);
    }
    list_free(l2);

    ListEntry *e = list_find(l, &(int){2});
    assert(e != NULL);

    l2 = list_sublist(l, e, LIST_END);
    assert(l2 && l2->size == 8);
    i = 2;
    list_iter(l2, ptr) {
        assert(*ptr == i++);
    }
    list_free(l2);
    list_free(l);
}

void test_splice(void) {
    int comparison1[] = {1, 10, 20, 30, 2, 3, 4};
    int comparison2[] = {1, 10, 20, 30, 3, 4};
    int comparison3[] = {2, 8, 12, 16, 1, 10, 20, 30, 3, 4};
    int comparison4[] = {2, 2, 8, 12, 16, 8, 12, 16, 1, 10, 20, 30, 3, 4};

    List *l1 = list_new(&int_helper, LIST_INIT_EMPTY);
    List *l2 = list_new(&int_helper, LIST_INIT_EMPTY);
    int *ptr;
    for (int i = 1; i<=4; ++i) {
        list_push_back(l1, &i);
    }

    for (int i=1; i<=3; ++i) {
        list_push_back(l2, &(int){i * 10});
    }

    ListEntry *e = l1->front->next;
    list_splice(l1, e, l2, LIST_SPLICE_ALL);

    assert(list_empty(l2));
    assert(l1->size == 7);
    int i = 0;
    list_iter(l1, ptr) {
        assert(*ptr == comparison1[i++]);
    }

    list_splice(l2, l2->front, l1, LIST_SPLICE_SINGLE, e);
    assert(l1->size == 6);
    i = 0;
    list_iter(l1, ptr) {
        assert(*ptr == comparison2[i++]);
    }
                                          
    e = l1->front->next->next->next;

    list_push_back(l2, &(int){8});
    list_push_back(l2, &(int){12});
    list_push_back(l2, &(int){16});
    list_push_back(l2, &(int){18});

    list_splice(l1, l1->front, l2, LIST_SPLICE_RANGE, l2->front, l2->back);

    assert(l2->front = l2->back);
    assert(l2->size == 1);
    assert(l1->size == 10);
    ptr = list_front(l2);
    assert(*ptr == 18);

    i = 0;
    list_iter(l1, ptr) {
        assert(*ptr == comparison3[i++]);
    }
    list_push_back(l2, &(int){2});
    list_push_back(l2, &(int){8});
    list_push_back(l2, &(int){12});
    list_push_back(l2, &(int){16});

    list_splice(l1, l1->front->next, l2, LIST_SPLICE_RANGE, l2->front->next, LIST_END);
    assert(l2->front = l2->back);
    assert(l2->size == 1);
    ptr = list_front(l2);
    assert(*ptr == 18);
    assert(l1->size == 14);
    i = 0;
    list_iter(l1, ptr) {
        assert(*ptr == comparison4[i++]);
    }
    assert(i == 14);
    i = 13;
    list_riter(l1, ptr) {
        assert(*ptr == comparison4[i--]);
    }
    assert(i == -1);
    list_free(l1);
    list_free(l2);
}

int main(void) {
    test_macros_insertion();
    test_custom_init();
    test_pop();

    List *l = list_new(&int_helper, LIST_INIT_EMPTY);
    test_insert_sorted(l);
    test_reverse(l);
    list_free(l);

    test_utility_funcs();

    l = list_new(&str_val_helper, LIST_INIT_BUILTIN, &names, 23);
    test_sort(l);
    test_find_erase(l);
    test_insert_list(l);
    list_free(l);
    test_string();
    test_merge();
    test_sublist();
    test_splice();

    return 0;
}
