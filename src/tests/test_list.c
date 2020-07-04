#include "defaults.h"
#include "list.h"

List *l = NULL;

char *names[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
                 "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd",
                 "polkitd","chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};

char *strs[] = {"Ten", "Nine", "Eight", "Seven", "Six", "Five",
                "Four", "Three", "Two", "One"};

int testCond(const void *_val) {
    int *val = (int *) _val;
    return (*val == 1);
}

void test_macros_insertion(void) {
    List *l = list_new(&str_val_helper);
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

    ListEntry e = l->front;
    int i = 0;
    for (; i < 10; ++i) {
        e = list_insert_after(l, e, &strs[i]);
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

void test_pop(void) {
    List *l = list_new(&str_val_helper);

    for (int i = 0; i < 10; ++i) {
        list_push_back(l, &strs[i]);
    }

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
        list_insert_sorted(l, &i);
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
    List *l = list_new(&int_helper);
    
    int i = 1;
    list_push_back(l, &i);

    i = 2;
    list_push_back(l, &i);
    list_push_back(l, &i);
    list_push_back(l, &i);

    i = 3;
    list_push_back(l, &i);
    list_push_back(l, &i);

    list_unique(l);
    assert(list_size(l) == 3);

    i = 1;
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
    for (int i = 0; i < 23; ++i) {
        list_push_back(l, &names[i]);
    }

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
    ListEntry res = list_find(l, &str);
    assert(res != NULL);

    ListEntry polkit = list_erase(l, res, res->next);
    assert(polkit != NULL);
    assert(list_size(l) == 22);

    list_erase(l, polkit, l->back);
    assert(list_size(l) == 13);
}

void test_insert_list(List *l) {
    List *l2 = list_new(&str_val_helper);
    ListEntry iter = NULL;
    for (int i = 0; i < 10; ++i) {
        iter = list_insert_before(l2, iter, &strs[i]);
    }

    char *str = "chrisray";
    ListEntry pos = list_find(l, &str);
    str = "Four";
    ListEntry start2 = list_find(l2, &str);
    list_insert_list(l, pos, l2, start2, NULL);
    assert(list_size(l) == 20);
    assert(list_size(l2) == 3);

    list_free(l2);
}

int main(int argc, char *argv[]) {
    test_macros_insertion();
    test_pop();

    List *l = list_new(&int_helper);
    test_insert_sorted(l);
    test_reverse(l);
    list_free(l);

    test_utility_funcs();

    l = list_new(&str_val_helper);
    test_sort(l);
    test_find_erase(l);
    test_insert_list(l);
    list_free(l);

    return 0;
}
