#include "list.h"
#include <assert.h>

gen_list_headers_withAlg(int, int)
gen_list_headers_withAlg(str, char *)
gen_list_source_withAlg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_list_source_withAlg(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

int testCond(int *val) { return (*val == 0); }
int strTestCond(char **val) { return streq(*val, "000"); }

int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

void compare_ints(List_int *l, int *comparison, unsigned size) {
    unsigned i = 0;
    ListEntry_int *it;
    assert(list_size(l) == size);
    if (size) {
        assert(!list_empty(l));
        assert(list_front(l) && *list_front(l) == comparison[0]);
        assert(list_back(l) && *list_back(l) == comparison[size-1]);
    } else {
        assert(list_empty(l));
        assert(list_front(l) == NULL && list_back(l) == NULL);
    }
    list_iter(l, it) {
        assert(it->data == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    list_riter(l, it) {
        assert(it->data == comparison[i--]);
    }
    assert(i == UINT_MAX);
}

void compare_strs(List_str *l, char **comparison, unsigned size) {
    unsigned i = 0;
    ListEntry_str *it;
    assert(list_size(l) == size);
    if (size) {
        assert(!list_empty(l));
        assert(list_front(l) && streq(*list_front(l), comparison[0]));
        assert(list_back(l) && streq(*list_back(l), comparison[size-1]));
    } else {
        assert(list_empty(l));
        assert(list_front(l) == NULL && list_back(l) == NULL);
    }
    list_iter(l, it) {
        assert(streq(it->data, comparison[i++]));
    }
    assert(i == size);
    i = size - 1;
    list_riter(l, it) {
        assert(streq(it->data, comparison[i--]));
    }
    assert(i == UINT_MAX);
}

void test_empty_init(void) {
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(int, li);
    list_free(str, ls);
}

void test_init_repeatingValue(void) {
    int c1[] = {5, 5};
    char *c2[] = {"005", "005"};
    List_int *li = list_new_repeatingValue(int, 0, 5);
    List_str *ls;
    compare_ints(li, c1, 0);
    list_free(int, li);

    li = list_new_repeatingValue(int, 1, 5);
    ls = list_new_repeatingValue(str, 1, "005");
    compare_ints(li, c1, 1);
    compare_strs(ls, c2, 1);
    list_free(int, li);
    list_free(str, ls);

    li = list_new_repeatingValue(int, 2, 5);
    ls = list_new_repeatingValue(str, 2, "005");
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_free(int, li);
    list_free(str, ls);
}

void test_init_fromArray(void) {
    List_int *li = list_new_fromArray(int, NULL, 5);
    List_str *ls;
    compare_ints(li, ints, 0);
    list_free(int, li);
    li = list_new_fromArray(int, ints, 0);
    compare_ints(li, ints, 0);
    list_free(int, li);

    li = list_new_fromArray(int, ints, 1);
    ls = list_new_fromArray(str, strs, 1);
    compare_ints(li, ints, 1);
    compare_strs(ls, strs, 1);
    list_free(int, li);
    list_free(str, ls);

    li = list_new_fromArray(int, ints, 2);
    ls = list_new_fromArray(str, strs, 2);
    compare_ints(li, ints, 2);
    compare_strs(ls, strs, 2);
    list_free(int, li);
    list_free(str, ls);
}

void test_init_copy(void) {
    int i;
    List_int *arr1[3] = {0}, *li;
    List_str *arr2[3] = {0}, *ls;
    arr1[0] = list_new(int);
    arr1[1] = list_new_fromArray(int, ints, 1);
    arr1[2] = list_new_fromArray(int, ints, 2);
    arr2[0] = list_new(str);
    arr2[1] = list_new_fromArray(str, strs, 1);
    arr2[2] = list_new_fromArray(str, strs, 2);
    
    li = list_createCopy(int, arr1[0]);
    compare_ints(li, ints, 0);
    list_free(int, li);

    li = list_createCopy(int, arr1[1]);
    ls = list_createCopy(str, arr2[1]);
    compare_ints(li, ints, 1);
    compare_strs(ls, strs, 1);
    list_free(int, li);
    list_free(str, ls);

    li = list_createCopy(int, arr1[2]);
    ls = list_createCopy(str, arr2[2]);
    compare_ints(li, ints, 2);
    compare_strs(ls, strs, 2);
    list_free(int, li);
    list_free(str, ls);

    for (i = 0; i < 3; ++i) {
        list_free(int, arr1[i]);
        list_free(str, arr2[i]);
    }
}

void test_push_front(void) {
    int c1[] = {10, 5, 0};
    char *c2[] = {"010", "005", "000"};
    int i;
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    for (i = 0; i < 3; ++i) {
        list_push_front(int, li, ints[i]);
        list_push_front(str, ls, strs[i]);
    }
    compare_ints(li, c1, 3);
    compare_strs(ls, c2, 3);
    list_free(int, li);
    list_free(str, ls);
}

void test_push_back(void) {
    int i;
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    for (i = 0; i < 3; ++i) {
        list_push_back(int, li, ints[i]);
        list_push_back(str, ls, strs[i]);
    }
    compare_ints(li, ints, 3);
    compare_strs(ls, strs, 3);
    list_free(int, li);
    list_free(str, ls);
}

void test_pop_front(void) {
    unsigned i = 1, j = 2;
    List_int *li = list_new_fromArray(int, ints, 3);
    List_str *ls = list_new_fromArray(str, strs, 3);
    while (!list_empty(li)) {
        list_pop_front(int, li);
        compare_ints(li, &ints[i++], j--);
    }
    list_pop_front(int, li);
    i = 1;
    j = 2;
    while (!list_empty(ls)) {
        list_pop_front(str, ls);
        compare_strs(ls, &strs[i++], j--);
    }
    list_pop_front(str, ls);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(int, li);
    list_free(str, ls);
}

void test_pop_back(void) {
    unsigned i = 2;
    List_int *li = list_new_fromArray(int, ints, 3);
    List_str *ls = list_new_fromArray(str, strs, 3);
    while (!list_empty(li)) {
        list_pop_back(int, li);
        compare_ints(li, ints, i--);
    }
    list_pop_back(int, li);
    i = 2;
    while (!list_empty(ls)) {
        list_pop_back(str, ls);
        compare_strs(ls, strs, i--);
    }
    list_pop_back(str, ls);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(int, li);
    list_free(str, ls);
}

void test_resize(void) {
    int c1[] = {15,15,15,20,20};
    char *c2[] = {"015","015","015","020","020"};
    unsigned i = 2;
    List_int *li = list_new_fromArray(int, ints, 3);
    List_str *ls = list_new_fromArray(str, strs, 3);
    while (!list_empty(li)) {
        list_resize(int, li, i);
        compare_ints(li, ints, i--);
    }
    list_resize(int, li, 0);
    i = 2;
    while (!list_empty(ls)) {
        list_resize(str, ls, i);
        compare_strs(ls, strs, i--);
    }
    list_resize(str, ls, 0);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);

    list_resize_usingValue(int, li, 3, 15);
    list_resize_usingValue(str, ls, 3, "015");
    list_resize_usingValue(int, li, 5, 20);
    list_resize_usingValue(str, ls, 5, "020");
    list_resize(int, li, 5);
    list_resize(str, ls, 5);
    compare_ints(li, c1, 5);
    compare_strs(ls, c2, 5);
    list_free(int, li);
    list_free(str, ls);
}

void test_insert_element(void) {
    List_int *li = list_new(int);
    List_str *ls = list_new(str);

    assert(list_insert(int, li, NULL, 10)->data == 10);
    assert(streq(list_insert(str, ls, NULL, "010")->data, "010"));
    assert(list_insert(int, li, li->front, 0)->data == 0);
    assert(streq(list_insert(str, ls, ls->front, "000")->data, "000"));
    assert(list_insert(int, li, li->back, 5)->data == 5);
    assert(streq(list_insert(str, ls, ls->back, "005")->data, "005"));
    assert(list_insert(int, li, NULL, 15)->data == 15);
    assert(streq(list_insert(str, ls, NULL, "015")->data, "015"));
    compare_ints(li, ints, 4);
    compare_strs(ls, strs, 4);
    list_free(int, li);
    list_free(str, ls);
}

void test_insert_repeatedValue(void) {
    int c1[] = {0,0,5,5,10,15,15};
    char *c2[] = {"000","000","005","005","010","015","015"};
    List_int *li = list_new(int);
    List_str *ls = list_new(str);

    assert(list_insert_repeatingValue(int, li, NULL, 0, 1) == NULL);
    assert(list_insert_repeatingValue(int, li, NULL, 1, 10)->data == 10);
    assert(streq(list_insert_repeatingValue(str, ls, NULL, 1, "010")->data, "010"));
    assert(list_insert_repeatingValue(int, li, li->front, 2, 0)->data == 0);
    assert(streq(list_insert_repeatingValue(str, ls, ls->front, 2, "000")->data, "000"));
    assert(list_insert_repeatingValue(int, li, li->back, 2, 5)->data == 5);
    assert(streq(list_insert_repeatingValue(str, ls, ls->back, 2, "005")->data, "005"));
    assert(list_insert_repeatingValue(int, li, NULL, 2, 15)->data == 15);
    assert(streq(list_insert_repeatingValue(str, ls, NULL, 2, "015")->data, "015"));
    compare_ints(li, c1, 7);
    compare_strs(ls, c2, 7);
    list_free(int, li);
    list_free(str, ls);
}

void test_insert_fromArray(void) {
    int arr1[4][2] = {{10,10},{0,0},{5,5},{15,15}};
    char *arr2[4][2] = {{"010","010"},{"000","000"},{"005","005"},{"015","015"}};
    int c1[] = {0,0,5,5,10,15,15};
    char *c2[] = {"000","000","005","005","010","015","015"};
    List_int *li = list_new(int);
    List_str *ls = list_new(str);

    assert(list_insert_fromArray(int, li, NULL, ints, 0) == NULL);
    assert(list_insert_fromArray(int, li, NULL, arr1[0], 1)->data == 10);
    assert(streq(list_insert_fromArray(str, ls, NULL, arr2[0], 1)->data, "010"));
    assert(list_insert_fromArray(int, li, li->front, arr1[1], 2)->data == 0);
    assert(streq(list_insert_fromArray(str, ls, ls->front, arr2[1], 2)->data, "000"));
    assert(list_insert_fromArray(int, li, li->back, arr1[2], 2)->data == 5);
    assert(streq(list_insert_fromArray(str, ls, ls->back, arr2[2], 2)->data, "005"));
    assert(list_insert_fromArray(int, li, NULL, arr1[3], 2)->data == 15);
    assert(streq(list_insert_fromArray(str, ls, NULL, arr2[3], 2)->data, "015"));
    compare_ints(li, c1, 7);
    compare_strs(ls, c2, 7);
    list_free(int, li);
    list_free(str, ls);
}

void test_insert_fromList(void) {
    int c1[] = {0,0,5,5,10,15,15};
    char *c2[] = {"000","000","005","005","010","015","015"};
    List_int *from1, *li = list_new(int);
    List_str *from2, *ls = list_new(str);
    ListEntry_int *p1 = NULL;
    ListEntry_str *p3 = NULL;
    {
       int arr1[] = {10,0,0,5,5,15,15};
       char *arr2[] = {"010","000","000","005","005","015","015"};
       from1 = list_new_fromArray(int, arr1, 7);
       from2 = list_new_fromArray(str, arr2, 7);
    }

    assert(list_insert_fromList(int, li, NULL, p1, p1) == NULL);
    p1 = from1->front;
    p3 = from2->front;
    assert(list_insert_fromList(int, li, NULL, p1, p1) == NULL);
    assert(list_insert_fromList(int, li, NULL, p1, p1->next)->data == 10);
    assert(streq(list_insert_fromList(str, ls, NULL, p3, p3->next)->data, "010"));
    p1 = p1->next;
    p3 = p3->next;
    assert(list_insert_fromList(int, li, li->front, p1, p1->next->next)->data == 0);
    assert(streq(list_insert_fromList(str, ls, ls->front, p3, p3->next->next)->data, "000"));
    p1 = p1->next->next;
    p3 = p3->next->next;
    assert(list_insert_fromList(int, li, li->back, p1, p1->next->next)->data == 5);
    assert(streq(list_insert_fromList(str, ls, ls->back, p3, p3->next->next)->data, "005"));
    p1 = p1->next->next;
    p3 = p3->next->next;
    assert(list_insert_fromList(int, li, NULL, p1, NULL)->data == 15);
    assert(streq(list_insert_fromList(str, ls, NULL, p3, NULL)->data, "015"));
    compare_ints(li, c1, 7);
    compare_strs(ls, c2, 7);
    list_free(int, li);
    list_free(str, ls);
    list_free(int, from1);
    list_free(str, from2);
}

void test_remove_element(void) {
    int c1[] = {5,15};
    char *c2[] = {"005","015"};
    List_int *li = list_new_fromArray(int, ints, 5);
    List_str *ls = list_new_fromArray(str, strs, 5);

    list_remove(int, li, li->back->next);
    list_remove(int, li, li->front);
    list_remove(str, ls, ls->front);
    list_remove(int, li, li->back);
    list_remove(str, ls, ls->back);
    list_remove(int, li, li->front->next);
    list_remove(str, ls, ls->front->next);
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_free(int, li);
    list_free(str, ls);
}

void test_erase_elements(void) {
    int c1[] = {10, 15, 30, 35};
    char *c2[] = {"010","015","030","035"};
    List_int *li = list_new_fromArray(int, ints, 10);
    List_str *ls = list_new_fromArray(str, strs, 10);

    assert(list_erase(int, li, NULL, li->back) == NULL);
    assert(list_erase(int, li, li->front, li->front) == NULL);
    assert(list_erase(int, li, li->front, li->front->next->next) == li->front);
    assert(list_erase(str, ls, ls->front, ls->front->next->next) == ls->front);
    assert(list_erase(int, li, li->back->prev, NULL) == LIST_END);
    assert(list_erase(str, ls, ls->back->prev, NULL) == LIST_END);
    assert(list_erase(int, li, li->front->next->next, li->back->prev)->data == 30);
    assert(streq(list_erase(str, ls, ls->front->next->next, ls->back->prev)->data, "030"));
    compare_ints(li, c1, 4);
    compare_strs(ls, c2, 4);
    list_free(int, li);
    list_free(str, ls);
}

void test_splice_all(void) {
    int i;
    List_int *arr1[4] = {0}, *li = list_new(int);
    List_str *arr2[4] = {0}, *ls = list_new(str);
    arr1[0] = list_new(int);
    arr1[1] = list_new_fromArray(int, ints, 2);
    arr1[2] = list_new_fromArray(int, &ints[2], 3);
    arr1[3] = list_new_fromArray(int, &ints[5], 2);
    arr2[0] = list_new(str);
    arr2[1] = list_new_fromArray(str, strs, 2);
    arr2[2] = list_new_fromArray(str, &strs[2], 3);
    arr2[3] = list_new_fromArray(str, &strs[5], 2);

    list_splice(int, li, li->front, arr1[0]);
    compare_ints(li, ints, 0);
    list_splice(int, li, li->front, arr1[1]);
    list_splice(str, ls, ls->front, arr2[1]);
    list_splice(int, li, NULL, arr1[3]);
    list_splice(str, ls, NULL, arr2[3]);
    list_splice(int, li, li->back->prev, arr1[2]);
    list_splice(str, ls, ls->back->prev, arr2[2]);
    for (i = 0; i < 4; ++i) {
        compare_ints(arr1[i], ints, 0);
        compare_strs(arr2[i], strs, 0);
        list_free(int, arr1[i]);
        list_free(str, arr2[i]);
    }
    compare_ints(li, ints, 7);
    compare_strs(ls, strs, 7);
    list_free(int, li);
    list_free(str, ls);
}

void test_splice_element(void) {
    int c1[2][3] = {{5,15},{0,10,20}};
    char *c2[2][3] = {{"005","015"},{"000","010","020"}};
    List_int *from1 = list_new_fromArray(int, ints, 5), *li = list_new(int);
    List_str *from2 = list_new_fromArray(str, strs, 5), *ls = list_new(str);

    list_splice_element(int, li, li->front, from1, from1->back->next);
    compare_ints(li, ints, 0);
    list_splice_element(int, li, li->front, from1, from1->back->prev->prev);
    list_splice_element(str, ls, ls->front, from2, from2->back->prev->prev);
    list_splice_element(int, li, li->front, from1, from1->front);
    list_splice_element(str, ls, ls->front, from2, from2->front);
    list_splice_element(int, li, NULL, from1, from1->back);
    list_splice_element(str, ls, NULL, from2, from2->back);
    compare_ints(from1, c1[0], 2);
    compare_strs(from2, c2[0], 2);
    compare_ints(li, c1[1], 3);
    compare_strs(ls, c2[1], 3);
    list_splice_element(int, li, li->front->next, from1, from1->front);
    list_splice_element(str, ls, ls->front->next, from2, from2->front);
    list_splice_element(int, li, li->back, from1, from1->back);
    list_splice_element(str, ls, ls->back, from2, from2->back);
    compare_ints(from1, ints, 0);
    compare_strs(from2, strs, 0);
    compare_ints(li, ints, 5);
    compare_strs(ls, strs, 5);
    list_free(int, from1);
    list_free(str, from2);

    list_free(int, li);
    list_free(str, ls);
}

void test_splice_range(void) {
    int c1[2][6] = {{10,15,30,35},{0,5,20,25,40,45}};
    char *c2[2][6] = {{"010","015","030","035"},{"000","005","020","025","040","045"}};
    List_int *from1 = list_new_fromArray(int, ints, 10), *li = list_new(int);
    List_str *from2 = list_new_fromArray(str, strs, 10), *ls = list_new(str);
    ListEntry_int *p1;
    ListEntry_str *p2;

    list_splice_range(int, li, li->front, from1, from1->back->next, NULL);
    compare_ints(li, ints, 0);
    list_splice_range(int, li, li->front, from1, from1->back, from1->back);
    compare_ints(li, ints, 0);
    p1 = from1->front;
    p2 = from2->front;
    listEntry_advance(int, &p1, 4);
    listEntry_advance(str, &p2, 4);
    list_splice_range(int, li, li->front, from1, p1, p1->next->next);
    list_splice_range(str, ls, ls->front, from2, p2, p2->next->next);
    list_splice_range(int, li, li->front, from1, from1->front, from1->front->next->next);
    list_splice_range(str, ls, ls->front, from2, from2->front, from2->front->next->next);
    list_splice_range(int, li, NULL, from1, from1->back->prev, NULL);
    list_splice_range(str, ls, NULL, from2, from2->back->prev, NULL);
    compare_ints(from1, c1[0], 4);
    compare_strs(from2, c2[0], 4);
    compare_ints(li, c1[1], 6);
    compare_strs(ls, c2[1], 6);
    list_splice_range(int, li, li->front->next->next, from1, from1->front, from1->front->next->next);
    list_splice_range(str, ls, ls->front->next->next, from2, from2->front, from2->front->next->next);
    list_splice_range(int, li, li->back->prev, from1, from1->front, NULL);
    list_splice_range(str, ls, ls->back->prev, from2, from2->front, NULL);
    compare_ints(from1, ints, 0);
    compare_strs(from2, strs, 0);
    compare_ints(li, ints, 10);
    compare_strs(ls, strs, 10);
    list_free(int, from1);
    list_free(str, from2);
    list_free(int, li);
    list_free(str, ls);
}

void test_reverse(void) {
    int comparison[] = {45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
    List_int *l = list_new_fromArray(int, ints, 10);
    list_reverse(int, l);
    compare_ints(l, comparison, 10);
    list_free(int, l);
}

void test_remove_if(void) {
    int a1[] = {0,5,0,10,0};
    char *a2[] = {"000","005","000","010","000"};
    List_int *li = list_new_fromArray(int, a1, 5);
    List_str *ls = list_new_fromArray(str, a2, 5);
    list_remove_if(int, li, testCond);
    list_remove_if(str, ls, strTestCond);
    compare_ints(li, &ints[1], 2);
    compare_strs(ls, &strs[1], 2);
    list_free(str, ls);
    list_free(int, li);
}

void test_unique(void) {
    int a1[] = {0,0,5,5,5,10,10,10,15,15};
    char *a2[] = {"000","000","005","005","005","010","010","010","015","015"};
    List_int *li = list_new_fromArray(int, a1, 10);
    List_str *ls = list_new_fromArray(str, a2, 10);
    list_unique(int, li);
    list_unique(str, ls);
    compare_ints(li, ints, 4);
    compare_strs(ls, strs, 4);
    list_free(str, ls);
    list_free(int, li);
}

void test_remove_value(void) {
    int a1[] = {2,5,2,10,2};
    char *a2[] = {"002","005","002","010","002"};
    List_int *li = list_new_fromArray(int, a1, 5);
    List_str *ls = list_new_fromArray(str, a2, 5);
    list_remove_value(int, li, 2);
    list_remove_value(str, ls, "002");
    compare_ints(li, &ints[1], 2);
    compare_strs(ls, &strs[1], 2);
    list_free(str, ls);
    list_free(int, li);
}

void test_find(void) {
    List_int *li = list_new_fromArray(int, ints, 10);
    List_str *ls = list_new_fromArray(str, strs, 10);
    assert(list_find(int, li, 30)->data == 30);
    assert(streq(list_find(str, ls, "030")->data, "030"));
    assert(list_find(int, li, 11) == NULL);
    assert(list_find(str, ls, "011") == NULL);
    list_free(str, ls);
    list_free(int, li);
}

void test_merge(void) {
    int a1[] = {10,20,30,40,50}, c1[] = {5,10,10,15,20,20,25,30,40,50};
    char *a2[] = {"010","020","030","040","050"}, *c2[] = {"005","010","010","015","020","020","025","030","040","050"};
    List_str *ls1 = list_new_fromArray(str, &strs[1], 5), *ls2 = list_new_fromArray(str, a2, 5);
    List_int *li1 = list_new_fromArray(int, &ints[1], 5), *li2 = list_new_fromArray(int, a1, 5), *li3 = list_new(int);

    list_merge(int, li1, li3);
    compare_ints(li1, &ints[1], 5);
    compare_ints(li3, ints, 0);
    list_merge(int, li3, li1);
    compare_ints(li3, &ints[1], 5);
    compare_ints(li1, ints, 0);
    list_merge(int, li3, li2);
    list_merge(str, ls1, ls2);
    compare_ints(li3, c1, 10);
    compare_ints(li2, ints, 0);
    compare_strs(ls1, c2, 10);
    compare_strs(ls2, strs, 0);
    list_free(int, li1);
    list_free(int, li2);
    list_free(int, li3);
    list_free(str, ls1);
    list_free(str, ls2);
}

void test_sort(void) {
    int ints_rand[] = {200,25,220,120,5,205,50,15,60,235,10,70,130,105,185,225,90,30,155,100,150,0,95,170,190,
125,210,75,45,160,175,145,55,230,35,65,110,140,115,20,215,85,195,240,245,135,80,180,40,165};
    char *strs_rand[] = {"200","025","220","120","005","205","050","015","060","235","010","070","130","105",
"185","225","090","030","155","100","150","000","095","170","190","125","210","075","045","160","175",
"145","055","230","035","065","110","140","115","020","215","085","195","240","245","135","080","180",
"040","165"};
    List_int *li = list_new_fromArray(int, ints_rand, 50);
    List_str *ls = list_new_fromArray(str, strs_rand, 50);
    list_sort(str, ls);
    list_sort(int, li);
    compare_ints(li, ints, 50);
    compare_strs(ls, strs, 50);
    list_free(str, ls);
    list_free(int, li);
}

void test_union(void) {
    int c1[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060","065","070"};
    List_str *ls1 = list_new_fromArray(str, strs, 10), *ls2 = list_new_fromArray(str, &strs[5], 10), *rs;
    List_int *li1 = list_new_fromArray(int, ints, 10), *li2 = list_new(int), *ri;

    ri = list_union(int, li1, li2);
    compare_ints(ri, ints, 10);
    list_free(int, ri);
    ri = list_union(int, li2, li1);
    compare_ints(ri, ints, 10);
    list_free(int, ri);
    ri = list_union(int, li2, li2);
    compare_ints(ri, ints, 0);
    list_free(int, ri);

    list_insert_fromArray(int, li2, NULL, &ints[5], 10);
    ri = list_union(int, li1, li2);
    rs = list_union(str, ls1, ls2);
    compare_ints(ri, c1, 15);
    compare_strs(rs, c2, 15);
    list_free(int, ri);
    list_free(str, rs);
    list_free(int, li1);
    list_free(int, li2);
    list_free(str, ls1);
    list_free(str, ls2);
}

void test_intersection(void) {
    int c1[] = {25,30,35,40,45};
    char *c2[] = {"025","030","035","040","045"};
    List_str *ls1 = list_new_fromArray(str, strs, 10), *ls2 = list_new_fromArray(str, &strs[5], 10), *rs;
    List_int *li1 = list_new_fromArray(int, ints, 10), *li2 = list_new(int), *ri;

    ri = list_intersection(int, li1, li2);
    compare_ints(ri, ints, 0);
    list_free(int, ri);
    ri = list_intersection(int, li2, li1);
    compare_ints(ri, ints, 0);
    list_free(int, ri);
    ri = list_intersection(int, li2, li2);
    compare_ints(ri, ints, 0);
    list_free(int, ri);

    list_insert_fromArray(int, li2, NULL, &ints[5], 10);
    ri = list_intersection(int, li1, li2);
    rs = list_intersection(str, ls1, ls2);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    list_free(int, ri);
    list_free(str, rs);
    list_free(int, li1);
    list_free(int, li2);
    list_free(str, ls1);
    list_free(str, ls2);
}

void test_difference(void) {
    int c1[] = {0,5,10,15,20};
    char *c2[] = {"000","005","010","015","020"};
    List_str *ls1 = list_new_fromArray(str, strs, 10), *ls2 = list_new_fromArray(str, &strs[5], 10), *rs;
    List_int *li1 = list_new_fromArray(int, ints, 10), *li2 = list_new(int), *ri;

    ri = list_difference(int, li1, li2);
    compare_ints(ri, ints, 10);
    list_free(int, ri);
    ri = list_difference(int, li2, li1);
    compare_ints(ri, ints, 0);
    list_free(int, ri);
    ri = list_difference(int, li2, li2);
    compare_ints(ri, ints, 0);
    list_free(int, ri);

    list_insert_fromArray(int, li2, NULL, &ints[5], 10);
    ri = list_difference(int, li1, li2);
    rs = list_difference(str, ls1, ls2);
    compare_ints(ri, c1, 5);
    compare_strs(rs, c2, 5);
    list_free(int, ri);
    list_free(str, rs);
    list_free(int, li1);
    list_free(int, li2);
    list_free(str, ls1);
    list_free(str, ls2);
}

void test_symmetric_difference(void) {
    int c1[] = {0,5,10,15,20,50,55,60,65,70};
    char *c2[] = {"000","005","010","015","020","050","055","060","065","070"};
    List_str *ls1 = list_new_fromArray(str, strs, 10), *ls2 = list_new_fromArray(str, &strs[5], 10), *rs;
    List_int *li1 = list_new_fromArray(int, ints, 10), *li2 = list_new(int), *ri;

    ri = list_symmetric_difference(int, li1, li2);
    compare_ints(ri, ints, 10);
    list_free(int, ri);
    ri = list_symmetric_difference(int, li2, li1);
    compare_ints(ri, ints, 10);
    list_free(int, ri);
    ri = list_symmetric_difference(int, li2, li2);
    compare_ints(ri, ints, 0);
    list_free(int, ri);

    list_insert_fromArray(int, li2, NULL, &ints[5], 10);
    ri = list_symmetric_difference(int, li1, li2);
    rs = list_symmetric_difference(str, ls1, ls2);
    compare_ints(ri, c1, 10);
    compare_strs(rs, c2, 10);
    list_free(int, ri);
    list_free(str, rs);
    list_free(int, li1);
    list_free(int, li2);
    list_free(str, ls1);
    list_free(str, ls2);
}

void test_includes(void) {
    int i;
    List_int *a[3];
    List_str *b[3];
    a[0] = list_new(int);
    a[1] = list_new_fromArray(int, &ints[5], 6);
    a[2] = list_new_fromArray(int, ints, 11);
    b[0] = list_new_fromArray(str, strs, 6);
    b[1] = list_new_fromArray(str, &strs[5], 6);
    b[2] = list_new_fromArray(str, strs, 11);

    assert(list_includes(int, a[0], a[0]));
    assert(!list_includes(int, a[0], a[1]));
    assert(list_includes(int, a[1], a[0]));
    list_insert_fromArray(int, a[0], NULL, ints, 6);

    for (i = 0; i < 3; ++i) {
        assert(list_includes(int, a[i], a[i]));
        assert(list_includes(str, b[i], b[i]));
    }

    for (i = 0; i < 2; ++i) {
        assert(!list_includes(int, a[i], a[2]));
        assert(list_includes(int, a[2], a[i]));
        assert(!list_includes(str, b[i], b[2]));
        assert(list_includes(str, b[2], b[i]));
    }

    assert(!list_includes(int, a[0], a[1]));
    assert(!list_includes(int, a[1], a[0]));
    assert(!list_includes(str, b[0], b[1]));
    assert(!list_includes(str, b[1], b[0]));

    for (i = 0; i < 3; ++i) {
        list_free(int, a[i]);
        list_free(str, b[i]);
    }
}

int main(void) {
    test_empty_init();
    test_init_repeatingValue();
    test_init_fromArray();
    test_init_copy();
    test_push_front();
    test_push_back();
    test_pop_front();
    test_pop_back();
    test_resize();
    test_insert_element();
    test_insert_repeatedValue();
    test_insert_fromArray();
    test_insert_fromList();
    test_remove_element();
    test_erase_elements();
    test_splice_all();
    test_splice_element();
    test_splice_range();
    test_reverse();
    test_remove_if();
    test_unique();
    test_remove_value();
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
