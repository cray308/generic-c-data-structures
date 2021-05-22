#include "list.h"
#include <assert.h>

gen_list_withalg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_list_withalg(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

static inline int testCond(int *val) { return (*val == 1); }

static inline int strTestCond(char **val) { return streq(*val, "1"); }

void compare_ints(List_int *l, int *comparison, int size) {
    assert(list_size(l) == size);
    if (size) {
        assert(list_front(l) && *list_front(l) == comparison[0]);
        assert(list_back(l) && *list_back(l) == comparison[size-1]);
    } else {
        assert(list_front(l) == NULL && list_back(l) == NULL);
    }
    int i = 0;
    ListEntry_int *it;
    list_iter(l, it) {
        assert(it->data == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    list_riter(l, it) {
        assert(it->data == comparison[i--]);
    }
    assert(i == -1);
}

void compare_strs(List_str *l, char **comparison, int size) {
    assert(list_size(l) == size);
    if (size) {
        assert(list_front(l) && streq(*list_front(l), comparison[0]));
        assert(list_back(l) && streq(*list_back(l), comparison[size-1]));
    } else {
        assert(list_front(l) == NULL && list_back(l) == NULL);
    }
    int i = 0;
    ListEntry_str *it;
    list_iter(l, it) {
        assert(streq(it->data, comparison[i++]));
    }
    assert(i == size);
    i = size - 1;
    list_riter(l, it) {
        assert(streq(it->data, comparison[i--]));
    }
    assert(i == -1);
}

void test_push_pop(void) {
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    assert(list_size(ls) == 0 && list_size(li) == 0);
    assert(ls->front == NULL && ls->back == NULL);
    assert(li->front == NULL && li->back == NULL);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);

    //push front, pop front
    list_push_front(str, ls, "1"); list_push_front(int, li, 1);
    compare_ints(li, (int[]){1}, 1);
    compare_strs(ls, (char*[]){"1"}, 1);
    list_push_front(str, ls, "0"); list_push_front(int, li, 0);
    compare_ints(li, (int[]){0, 1}, 2);
    compare_strs(ls, (char*[]){"0", "1"}, 2);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, (int[]){1}, 1);
    compare_strs(ls, (char*[]){"1"}, 1);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);

    //push back, pop back
    list_push_back(str, ls, "0"); list_push_back(int, li, 0);
    compare_ints(li, (int[]){0}, 1);
    compare_strs(ls, (char*[]){"0"}, 1);
    list_push_back(str, ls, "1"); list_push_back(int, li, 1);
    compare_ints(li, (int[]){0, 1}, 2);
    compare_strs(ls, (char*[]){"0", "1"}, 2);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, (int[]){0}, 1);
    compare_strs(ls, (char*[]){"0"}, 1);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);

    //push front, pop back
    list_push_front(str, ls, "1"); list_push_front(int, li, 1);
    compare_ints(li, (int[]){1}, 1);
    compare_strs(ls, (char*[]){"1"}, 1);
    list_push_front(str, ls, "0"); list_push_front(int, li, 0);
    compare_ints(li, (int[]){0, 1}, 2);
    compare_strs(ls, (char*[]){"0", "1"}, 2);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, (int[]){0}, 1);
    compare_strs(ls, (char*[]){"0"}, 1);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);

    //push back, pop front
    list_push_back(str, ls, "0"); list_push_back(int, li, 0);
    compare_ints(li, (int[]){0}, 1);
    compare_strs(ls, (char*[]){"0"}, 1);
    list_push_back(str, ls, "1"); list_push_back(int, li, 1);
    compare_ints(li, (int[]){0, 1}, 2);
    compare_strs(ls, (char*[]){"0", "1"}, 2);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, (int[]){1}, 1);
    compare_strs(ls, (char*[]){"1"}, 1);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);
    list_free(str, ls); list_free(int, li);
}

void test_custom_init(void) {
    List_int *li = list_new_fromArray(int, ((int[]){}), 0);
    List_str *ls = list_new_fromArray(str, ((char*[]){}), 0);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);
    list_free(str, ls); list_free(int, li);

    li = list_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    ls = list_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);
    compare_ints(li, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
    compare_strs(ls, (char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10);

    List_int *x = list_createCopy(int, li);
    List_str *y = list_createCopy(str, ls);
    compare_ints(x, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, 10);
    compare_strs(y, (char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10);
    list_free(str, y); list_free(int, x);
    list_free(str, ls); list_free(int, li);
}

void test_insert(void) {
    List_int *li = list_new(int);
    List_str *ls = list_new(str);

    // Single element
    ListEntry_int *x = list_insert(int, li, NULL, 3);
    ListEntry_str *y = list_insert(str, ls, NULL, "3"); //3
    assert(x->data == 3 && streq(y->data, "3"));
    list_insert(int, li, NULL, 4); list_insert(str, ls, NULL, "4"); //3, 4
    list_insert(int, li, x, 2); list_insert(str, ls, y, "2"); //2, 3, 4
    list_insert(int, li, li->front, 1); list_insert(str, ls, ls->front, "1"); //1, 2, 3, 4
    list_insert(int, li, NULL, 5); list_insert(str, ls, NULL, "5"); //1, 2, 3, 4, 5

    compare_ints(li, (int[]){1, 2, 3, 4, 5}, 5);
    compare_strs(ls, (char*[]){"1", "2", "3", "4", "5"}, 5);

    // From array
    list_insert_fromArray(int, li, NULL, ((int[]){}), 0);
    list_insert_fromArray(str, ls, NULL, ((char*[]){}), 0);
    compare_ints(li, (int[]){1, 2, 3, 4, 5}, 5);
    compare_strs(ls, (char*[]){"1", "2", "3", "4", "5"}, 5);

    ListEntry_int *frontI = list_insert_fromArray(int, li, li->front, ((int[]){-1, 0}), 2);
    ListEntry_str *frontS = list_insert_fromArray(str, ls, ls->front, ((char*[]){"-1", "0"}), 2);
    assert(frontI->data == -1 && streq(frontS->data, "-1"));
    compare_ints(li, (int[]){-1, 0, 1, 2, 3, 4, 5}, 7);
    compare_strs(ls, (char*[]){"-1", "0", "1", "2", "3", "4", "5"}, 7);

    ListEntry_int *backI = list_insert_fromArray(int, li, NULL, ((int[]){6, 7}), 2);
    ListEntry_str *backS = list_insert_fromArray(str, ls, NULL, ((char*[]){"6", "7"}), 2);
    assert(backI->data == 6 && streq(backS->data, "6"));
    compare_ints(li, (int[]){-1, 0, 1, 2, 3, 4, 5, 6, 7}, 9);
    compare_strs(ls, (char*[]){"-1", "0", "1", "2", "3", "4", "5", "6", "7"}, 9);

    List_int *li2 = list_new_fromArray(int, ((int[]){1, 2, 3, 4, 5}), 5);
    List_str *ls2 = list_new_fromArray(str, ((char*[]){"1", "2", "3", "4", "5"}), 5);

    // From list
    frontI = list_insert_fromList(int, li2, li2->front, frontI, frontI->next->next);
    frontS = list_insert_fromList(str, ls2, ls2->front, frontS, frontS->next->next);
    assert(frontI->data == -1 && streq(frontS->data, "-1"));
    compare_ints(li2, (int[]){-1, 0, 1, 2, 3, 4, 5}, 7);
    compare_strs(ls2, (char*[]){"-1", "0", "1", "2", "3", "4", "5"}, 7);

    backI = list_insert_fromList(int, li2, NULL, backI, NULL);
    backS = list_insert_fromList(str, ls2, NULL, backS, NULL);
    assert(backI->data == 6 && streq(backS->data, "6"));
    compare_ints(li2, (int[]){-1, 0, 1, 2, 3, 4, 5, 6, 7}, 9);
    compare_strs(ls2, (char*[]){"-1", "0", "1", "2", "3", "4", "5", "6", "7"}, 9);

    list_free(str, ls2); list_free(int, li2);
    list_free(str, ls); list_free(int, li);
}

void test_insert_sorted(void) {
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    list_insert_sorted(int, li, 2); list_insert_sorted(str, ls, "c");
    list_insert_sorted(int, li, 1); list_insert_sorted(str, ls, "b");
    list_insert_sorted(int, li, 0); list_insert_sorted(str, ls, "a");
    compare_ints(li, (int[]){0, 1, 2}, 3);
    compare_strs(ls, (char*[]){"a", "b", "c"}, 3);

    List_int *tempI = list_new_fromArray(int, ((int[]){2, 1, 0}), 3);
    List_str *tempS = list_new_fromArray(str, ((char*[]){"c", "b", "a"}), 3);

    list_insert_fromArray_sorted(int, li, ((int[]){2, 1, 0}), 3);
    list_insert_fromArray_sorted(str, ls, ((char*[]){"c", "b", "a"}), 3);
    compare_ints(li, (int[]){0, 0, 1, 1, 2, 2}, 6);
    compare_strs(ls, (char*[]){"a", "a", "b", "b", "c", "c"}, 6);

    list_insert_fromList_sorted(int, li, tempI->front, NULL);
    list_insert_fromList_sorted(str, ls, tempS->front, NULL);
    compare_ints(li, (int[]){0, 0, 0, 1, 1, 1, 2, 2, 2}, 9);
    compare_strs(ls, (char*[]){"a", "a", "a", "b", "b", "b", "c", "c", "c"}, 9);
    list_free(str, tempS); list_free(int, tempI);
    list_free(str, ls); list_free(int, li);
}

void test_erase(void) {
    List_int *li = list_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    List_str *ls = list_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);

    assert(list_erase(int, li, NULL, li->back) == NULL && list_erase(str, ls, NULL, ls->back) == NULL);
    assert(list_erase(int, li, li->front, li->front) == NULL && list_erase(str, ls, ls->front, ls->front) == NULL);
    ListEntry_int *x = list_erase(int, li, li->front, li->front->next->next);
    ListEntry_str *y = list_erase(str, ls, ls->front, ls->front->next->next); //2, 3, 4, 5, 6, 7, 8, 9
    assert(x == li->front && y == ls->front);
    assert(list_erase(int, li, li->back->prev, NULL) == NULL && list_erase(str, ls, ls->back->prev, NULL) == NULL); //2, 3, 4, 5, 6, 7
    x = list_erase(int, li, li->front->next->next, li->back->prev);
    y = list_erase(str, ls, ls->front->next->next, ls->back->prev); //2, 3, 6, 7
    assert(x && x->data == 6 && y && streq(y->data, "6"));
    compare_ints(li, (int[]){2, 3, 6, 7}, 4);
    compare_strs(ls, (char*[]){"2", "3", "6", "7"}, 4);

    assert(list_erase(int, li, li->front, NULL) == NULL && list_erase(str, ls, ls->front, NULL) == NULL);
    compare_ints(li, (int[]){}, 0);
    compare_strs(ls, (char*[]){}, 0);
    list_free(str, ls); list_free(int, li);
}

void test_reverse(void) {
    List_int *l = list_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    list_reverse(int, l);
    compare_ints(l, (int[]){9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, 10);
    list_free(int, l);
}

void test_utility_funcs(void) {
    List_int *li = list_new_fromArray(int, ((int []){1, 2, 2, 2, 3, 3}), 6);
    List_str *ls = list_new_fromArray(str, ((char*[]){"1", "2", "2", "2", "3", "3"}), 6);

    list_unique(int, li); list_unique(str, ls);
    compare_ints(li, (int[]){1, 2, 3}, 3);
    compare_strs(ls, (char*[]){"1", "2", "3"}, 3);

    list_remove_value(int, li, 2); list_remove_value(str, ls, "2");
    compare_ints(li, (int[]){1, 3}, 2);
    compare_strs(ls, (char*[]){"1", "3"}, 2);

    list_remove_if(int, li, testCond); list_remove_if(str, ls, strTestCond);
    compare_ints(li, (int[]){3}, 1);
    compare_strs(ls, (char*[]){"3"}, 1);
    list_free(str, ls); list_free(int, li);
}

void test_sort(void) {
    char *strs[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
        "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd","polkitd",
        "chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};
    char *strs_sorted[] = {"avahi","bin","chrisray","colord","daemon","dbus","ftp","geoclue","git",
        "http","lightdm","mail","nobody","polkitd","root","rtkit","systemd-coredump","systemd-journal-remote",
        "systemd-network","systemd-resolve","systemd-timesync","usbmux","uuidd"};

    List_str *l = list_new_fromArray(str, strs, 23);
    list_sort(str, l);
    compare_strs(l, strs_sorted, 23);
    list_free(str, l);
}

void test_find(void) {
    List_int *li = list_new_fromArray(int, ((int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), 10);
    List_str *ls = list_new_fromArray(str, ((char*[]){"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}), 10);
    ListEntry_int *x = list_find(int, li, 6);
    ListEntry_str *y = list_find(str, ls, "6");
    assert(x && x->data == 6);
    assert(y && streq(y->data, "6"));
    assert(list_find(int, li, 15) == NULL && list_find(str, ls, "15") == NULL);
    list_free(str, ls); list_free(int, li);
}

void test_merge(void) {
    List_str *ls1 = list_new_fromArray(str, ((char*[]){"a5", "b0", "b5", "c0", "c5"}), 5);
    List_str *ls2 = list_new_fromArray(str, ((char*[]){"f0", "e0", "d0", "c0", "b0"}), 5);
    List_int *li1 = list_new_fromArray(int, ((int[]){5, 10, 15, 20, 25}), 5);
    List_int *li2 = list_new_fromArray(int, ((int[]){50, 40, 30, 20, 10}), 5);

    list_sort(int, li2); list_merge(int, li1, li2);
    list_sort(str, ls2); list_merge(str, ls1, ls2);

    compare_ints(li1, (int[]){5, 10, 10, 15, 20, 20, 25, 30, 40, 50}, 10);
    compare_ints(li2, (int[]){}, 0);
    compare_strs(ls1, (char*[]){"a5", "b0", "b0", "b5", "c0", "c0", "c5", "d0", "e0", "f0"}, 10);
    compare_strs(ls2, (char*[]){}, 0);
    list_free(int, li1); list_free(int, li2);
    list_free(str, ls1); list_free(str, ls2);
}

void test_splice(void) {
    List_str *ls1 = list_new_fromArray(str, ((char*[]){"b0", "c0", "d0"}), 3);
    List_str *ls2 = list_new_fromArray(str, ((char*[]){"a1", "a2", "a3", "a4"}), 4);
    List_int *li1 = list_new_fromArray(int, ((int[]){10, 20, 30}), 3);
    List_int *li2 = list_new_fromArray(int, ((int[]){1, 2, 3, 4}), 4);

    list_splice(int, li1, li1->front, li2); list_splice(str, ls1, ls1->front, ls2);
    compare_ints(li1, (int[]){1, 2, 3, 4, 10, 20, 30}, 7);
    compare_ints(li2, (int[]){}, 0);
    compare_strs(ls1, (char*[]){"a1", "a2", "a3", "a4", "b0", "c0", "d0"}, 7);
    compare_strs(ls2, (char*[]){}, 0);

    list_splice_element(int, li2, NULL, li1, li1->front->next);
    list_splice_element(str, ls2, NULL, ls1, ls1->front->next);
    compare_ints(li1, (int[]){1, 3, 4, 10, 20, 30}, 6);
    compare_ints(li2, (int[]){2}, 1);
    compare_strs(ls1, (char*[]){"a1", "a3", "a4", "b0", "c0", "d0"}, 6);
    compare_strs(ls2, (char*[]){"a2"}, 1);

    list_insert_fromArray(int, li2, NULL, ((int[]){40, 50}), 2);
    list_insert_fromArray(str, ls2, NULL, ((char*[]){"e0", "f0"}), 2);
    list_splice_range(int, li1, NULL, li2, li2->front->next, NULL);
    list_splice_range(str, ls1, NULL, ls2, ls2->front->next, NULL);
    compare_ints(li1, (int[]){1, 3, 4, 10, 20, 30, 40, 50}, 8);
    compare_ints(li2, (int[]){2}, 1);
    compare_strs(ls1, (char*[]){"a1", "a3", "a4", "b0", "c0", "d0", "e0", "f0"}, 8);
    compare_strs(ls2, (char*[]){"a2"}, 1);

    list_insert_fromArray(int, li2, li2->front, ((int[]){5, 6, 7}), 3);
    list_insert_fromArray(str, ls2, ls2->front, ((char*[]){"a5", "a6", "a7"}), 3);
    ListEntry_int *x = li1->front;
    ListEntry_str *y = ls1->front;
    iter_advance(LIST, int, x, 3); iter_advance(LIST, str, y, 3);
    list_splice_range(int, li1, x, li2, li2->front, li2->back);
    list_splice_range(str, ls1, y, ls2, ls2->front, ls2->back);
    compare_ints(li1, (int[]){1, 3, 4, 5, 6, 7, 10, 20, 30, 40, 50}, 11);
    compare_ints(li2, (int[]){2}, 1);
    compare_strs(ls1, (char*[]){"a1", "a3", "a4", "a5", "a6", "a7", "b0", "c0", "d0", "e0", "f0"}, 11);
    compare_strs(ls2, (char*[]){"a2"}, 1);
    list_free(int, li1); list_free(int, li2);
    list_free(str, ls1); list_free(str, ls2);
}

void test_alg_funcs(void) {
    List_str *ls1 = list_new_fromArray(str, ((char*[]){"a5", "b0", "b5", "c0", "c5"}), 5);
    List_str *ls2 = list_new_fromArray(str, ((char*[]){"b0", "c0", "d0", "e0", "f0"}), 5);
    List_int *li1 = list_new_fromArray(int, ((int[]){5, 10, 15, 20, 25}), 5);
    List_int *li2 = list_new_fromArray(int, ((int[]){10, 20, 30, 40, 50}), 5);

    List_int *ri = set_union_list(int, li1, li2);
    List_str *rs = set_union_list(str, ls1, ls2);
    compare_ints(ri, (int[]){5, 10, 15, 20, 25, 30, 40, 50}, 8);
    compare_strs(rs, (char*[]){"a5", "b0", "b5", "c0", "c5", "d0", "e0", "f0"}, 8);
    list_free(int, ri); list_free(str, rs);

    ri = set_intersection_list(int, li1, li2);
    rs = set_intersection_list(str, ls1, ls2);
    compare_ints(ri, (int[]){10, 20}, 2);
    compare_strs(rs, (char*[]){"b0", "c0"}, 2);
    list_free(int, ri); list_free(str, rs);

    ri = set_difference_list(int, li1, li2);
    rs = set_difference_list(str, ls1, ls2);
    compare_ints(ri, (int[]){5, 15, 25}, 3);
    compare_strs(rs, (char*[]){"a5", "b5", "c5"}, 3);
    list_free(int, ri); list_free(str, rs);

    ri = set_symmetric_difference_list(int, li1, li2);
    rs = set_symmetric_difference_list(str, ls1, ls2);
    compare_ints(ri, (int[]){5, 15, 25, 30, 40, 50}, 6);
    compare_strs(rs, (char*[]){"a5", "b5", "c5", "d0", "e0", "f0"}, 6);
    list_free(int, ri); list_free(str, rs);

    List_int *container = list_new_fromArray(int, ((int[]){5,10,15,20,25,30,35,40,45,50}), 10);
    List_int *continent = list_new_fromArray(int, ((int[]){10, 20, 30, 40}), 4);
    assert(includes_list(int, container, continent));
    list_free(int, container); list_free(int, continent);
    list_free(int, li1); list_free(int, li2);
    list_free(str, ls1); list_free(str, ls2);
}

int main(void) {
    test_push_pop();
    test_custom_init();
    test_insert();
    test_insert_sorted();
    test_erase();
    test_reverse();
    test_utility_funcs();
    test_sort();
    test_find();
    test_merge();
    test_splice();
    test_alg_funcs();
    return 0;
}
