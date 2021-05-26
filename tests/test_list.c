#include "list.h"
#include <assert.h>

gen_list_withalg(int, int, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_list_withalg(str, char *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete)

__DS_FUNC_PREFIX_INL int testCond(int *val) { return (*val == 1); }
__DS_FUNC_PREFIX_INL int strTestCond(char **val) { return streq(*val, "1"); }

int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

void compare_ints(List_int *l, int *comparison, int size) {
    int i = 0;
    ListEntry_int *it;
    assert(list_size(l) == size);
    if (size) {
        assert(list_front(l) && *list_front(l) == comparison[0]);
        assert(list_back(l) && *list_back(l) == comparison[size-1]);
    } else {
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
    assert(i == -1);
}

void compare_strs(List_str *l, char **comparison, int size) {
    int i = 0;
    ListEntry_str *it;
    assert(list_size(l) == size);
    if (size) {
        assert(list_front(l) && streq(*list_front(l), comparison[0]));
        assert(list_back(l) && streq(*list_back(l), comparison[size-1]));
    } else {
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
    assert(i == -1);
}

void test_push_pop(void) {
    int c1[] = {0,1};
    char *c2[] = {"0", "1"};
    List_int *li = list_new(int);
    List_str *ls = list_new(str);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);

    /* push front, pop front */
    list_push_front(str, ls, "1"); list_push_front(int, li, 1);
    compare_ints(li, &c1[1], 1);
    compare_strs(ls, &c2[1], 1);
    list_push_front(str, ls, "0"); list_push_front(int, li, 0);
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, &c1[1], 1);
    compare_strs(ls, &c2[1], 1);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);

    /* push back, pop back */
    list_push_back(str, ls, "0"); list_push_back(int, li, 0);
    compare_ints(li, c1, 1);
    compare_strs(ls, c2, 1);
    list_push_back(str, ls, "1"); list_push_back(int, li, 1);
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, c1, 1);
    compare_strs(ls, c2, 1);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);

    /* push front, pop back */
    list_push_front(str, ls, "1"); list_push_front(int, li, 1);
    compare_ints(li, &c1[1], 1);
    compare_strs(ls, &c2[1], 1);
    list_push_front(str, ls, "0"); list_push_front(int, li, 0);
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, c1, 1);
    compare_strs(ls, c2, 1);
    list_pop_back(str, ls); list_pop_back(int, li);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);

    /* push back, pop front */
    list_push_back(str, ls, "0"); list_push_back(int, li, 0);
    compare_ints(li, c1, 1);
    compare_strs(ls, c2, 1);
    list_push_back(str, ls, "1"); list_push_back(int, li, 1);
    compare_ints(li, c1, 2);
    compare_strs(ls, c2, 2);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, &c1[1], 1);
    compare_strs(ls, &c2[1], 1);
    list_pop_front(str, ls); list_pop_front(int, li);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(str, ls); list_free(int, li);
}

void test_custom_init(void) {
    List_int *li = list_new_fromArray(int, NULL, 0), *x;
    List_str *ls = list_new_fromArray(str, NULL, 0), *y;
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(str, ls); list_free(int, li);

    li = list_new_fromArray(int, ints, 10);
    ls = list_new_fromArray(str, strs, 10);
    compare_ints(li, ints, 10);
    compare_strs(ls, strs, 10);

    x = list_createCopy(int, li);
    y = list_createCopy(str, ls);
    compare_ints(x, ints, 10);
    compare_strs(y, strs, 10);
    list_free(str, y); list_free(int, x);
    list_free(str, ls); list_free(int, li);
}

void test_insert(void) {
    int c1[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7};
    char *c2[] = {"-1", "0", "1", "2", "3", "4", "5", "6", "7"};
    List_int *li = list_new(int), *li2;
    List_str *ls = list_new(str), *ls2;
    ListEntry_int *frontI, *backI;
    ListEntry_str *frontS, *backS;

    /* Single element */
    frontI = list_insert(int, li, NULL, 3);
    frontS = list_insert(str, ls, NULL, "3"); /* 3 */
    assert(frontI->data == 3 && streq(frontS->data, "3"));
    frontI = list_insert(int, li, NULL, 4);
    frontS = list_insert(str, ls, NULL, "4"); /* 3, 4 */
    assert(frontI->data == 4 && streq(frontS->data, "4"));
    frontI = list_insert(int, li, li->front, 2);
    frontS = list_insert(str, ls, ls->front, "2"); /* 2, 3, 4 */
    assert(frontI->data == 2 && streq(frontS->data, "2"));
    frontI = list_insert(int, li, li->front, 1);
    frontS = list_insert(str, ls, ls->front, "1"); /* 1, 2, 3, 4 */
    assert(frontI->data == 1 && streq(frontS->data, "1"));
    frontI = list_insert(int, li, NULL, 5);
    frontS = list_insert(str, ls, NULL, "5"); /* 1, 2, 3, 4, 5 */
    assert(frontI->data == 5 && streq(frontS->data, "5"));
    compare_ints(li, &c1[2], 5);
    compare_strs(ls, &c2[2], 5);

    /* From array */
    assert(list_insert_fromArray(int, li, NULL, NULL, 0) == NULL);
    assert(list_insert_fromArray(str, ls, NULL, NULL, 0) == NULL);

    frontI = list_insert_fromArray(int, li, li->front, c1, 2);
    frontS = list_insert_fromArray(str, ls, ls->front, c2, 2);
    assert(frontI->data == -1 && streq(frontS->data, "-1"));
    backI = list_insert_fromArray(int, li, NULL, &c1[7], 2);
    backS = list_insert_fromArray(str, ls, NULL, &c2[7], 2);
    assert(backI->data == 6 && streq(backS->data, "6"));
    compare_ints(li, c1, 9);
    compare_strs(ls, c2, 9);

    li2 = list_new_fromArray(int, &c1[2], 5);
    ls2 = list_new_fromArray(str, &c2[2], 5);

    /* From list */
    frontI = list_insert_fromList(int, li2, li2->front, frontI, frontI->next->next);
    frontS = list_insert_fromList(str, ls2, ls2->front, frontS, frontS->next->next);
    assert(frontI->data == -1 && streq(frontS->data, "-1"));
    backI = list_insert_fromList(int, li2, NULL, backI, NULL);
    backS = list_insert_fromList(str, ls2, NULL, backS, NULL);
    assert(backI->data == 6 && streq(backS->data, "6"));
    compare_ints(li2, c1, 9);
    compare_strs(ls2, c2, 9);

    list_free(str, ls2); list_free(int, li2);
    list_free(str, ls); list_free(int, li);
}

void test_insert_sorted(void) {
    int revInts[] = {2, 1, 0};
    char *revStrs[] = {"2", "1", "0"};
    int c1[3][9] = {{0,1,2},{0,0,1,1,2,2},{0,0,0,1,1,1,2,2,2}};
    char *c2[3][9] = {{"0","1","2"},{"0","0","1","1","2","2"},{"0","0","0","1","1","1","2","2","2"}};
    List_int *li = list_new(int), *tempI;
    List_str *ls = list_new(str), *tempS;
    list_insert_sorted(int, li, 2); list_insert_sorted(str, ls, "2");
    list_insert_sorted(int, li, 1); list_insert_sorted(str, ls, "1");
    list_insert_sorted(int, li, 0); list_insert_sorted(str, ls, "0");
    compare_ints(li, c1[0], 3);
    compare_strs(ls, c2[0], 3);

    tempI = list_new_fromArray(int, revInts, 3);
    tempS = list_new_fromArray(str, revStrs, 3);

    list_insert_fromArray_sorted(int, li, revInts, 3);
    list_insert_fromArray_sorted(str, ls, revStrs, 3);
    compare_ints(li, c1[1], 6);
    compare_strs(ls, c2[1], 6);

    list_insert_fromList_sorted(int, li, tempI->front, NULL);
    list_insert_fromList_sorted(str, ls, tempS->front, NULL);
    compare_ints(li, c1[2], 9);
    compare_strs(ls, c2[2], 9);
    list_free(str, tempS); list_free(int, tempI);
    list_free(str, ls); list_free(int, li);
}

void test_erase(void) {
    int c1[] = {10, 15, 30, 35};
    char *c2[] = {"010","015","030","035"};
    List_int *li = list_new_fromArray(int, ints, 10);
    List_str *ls = list_new_fromArray(str, strs, 10);
    ListEntry_int *x;
    ListEntry_str *y;

    assert(list_erase(int, li, NULL, li->back) == NULL && list_erase(str, ls, NULL, ls->back) == NULL);
    assert(list_erase(int, li, li->front, li->front) == NULL && list_erase(str, ls, ls->front, ls->front) == NULL);
    x = list_erase(int, li, li->front, li->front->next->next);
    y = list_erase(str, ls, ls->front, ls->front->next->next); /* 10, 15, 20, 25, 30, 35, 40, 45 */
    assert(x == li->front && y == ls->front);
    assert(list_erase(int, li, li->back->prev, NULL) == LIST_END && list_erase(str, ls, ls->back->prev, NULL) == LIST_END); /* 10, 15, 20, 25, 30, 35 */
    x = list_erase(int, li, li->front->next->next, li->back->prev);
    y = list_erase(str, ls, ls->front->next->next, ls->back->prev); /* 10, 15, 30, 35 */
    assert(x && x->data == 30 && y && streq(y->data, "030"));
    compare_ints(li, c1, 4);
    compare_strs(ls, c2, 4);

    assert(list_erase(int, li, li->front, NULL) == LIST_END && list_erase(str, ls, ls->front, NULL) == LIST_END);
    compare_ints(li, ints, 0);
    compare_strs(ls, strs, 0);
    list_free(str, ls); list_free(int, li);
}

void test_reverse(void) {
    int comparison[] = {45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
    List_int *l = list_new_fromArray(int, ints, 10);
    list_reverse(int, l);
    compare_ints(l, comparison, 10);
    list_free(int, l);
}

void test_utility_funcs(void) {
    int c1[3][3] = {{1,2,3},{1,3},{3}};
    char *c2[3][3] = {{"1","2","3"},{"1","3"},{"3"}};
    int start1[] = {1,2,2,2,3,3};
    char *start2[] = {"1","2","2","2","3","3"};
    List_int *li = list_new_fromArray(int, start1, 6);
    List_str *ls = list_new_fromArray(str, start2, 6);

    list_unique(int, li); list_unique(str, ls);
    compare_ints(li, c1[0], 3);
    compare_strs(ls, c2[0], 3);

    list_remove_value(int, li, 2); list_remove_value(str, ls, "2");
    compare_ints(li, c1[1], 2);
    compare_strs(ls, c2[1], 2);

    list_remove_if(int, li, testCond); list_remove_if(str, ls, strTestCond);
    compare_ints(li, c1[2], 1);
    compare_strs(ls, c2[2], 1);
    list_free(str, ls); list_free(int, li);
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
    list_sort(str, ls); list_sort(int, li);
    compare_ints(li, ints, 50);
    compare_strs(ls, strs, 50);
    list_free(str, ls); list_free(int, li);
}

void test_find(void) {
    List_int *li = list_new_fromArray(int, ints, 10);
    List_str *ls = list_new_fromArray(str, strs, 10);
    ListEntry_int *x = list_find(int, li, 30);
    ListEntry_str *y = list_find(str, ls, "030");
    assert(x && x->data == 30);
    assert(y && streq(y->data, "030"));
    assert(list_find(int, li, 11) == NULL && list_find(str, ls, "011") == NULL);
    list_free(str, ls); list_free(int, li);
}

void test_merge(void) {
    int a1[] = {50,40,30,20,10}; char *a2[] = {"050","040","030","020","010"};
    int c1[] = {5, 10, 10, 15, 20, 20, 25, 30, 40, 50};
    char *c2[] = {"005","010","010","015","020","020","025","030","040","050"};
    List_str *ls1 = list_new_fromArray(str, &strs[1], 5);
    List_str *ls2 = list_new_fromArray(str, a2, 5);
    List_int *li1 = list_new_fromArray(int, &ints[1], 5);
    List_int *li2 = list_new_fromArray(int, a1, 5);

    list_sort(int, li2); list_merge(int, li1, li2);
    list_sort(str, ls2); list_merge(str, ls1, ls2);

    compare_ints(li1, c1, 10);
    compare_ints(li2, ints, 0);
    compare_strs(ls1, c2, 10);
    compare_strs(ls2, strs, 0);
    list_free(int, li1); list_free(int, li2);
    list_free(str, ls1); list_free(str, ls2);
}

void test_splice(void) {
    int secondCmpInt[] = {2};
    char *secondCmpStr[] = {"02"};
    int c1[4][11] = {{1,2,3,4,10,20,30},{1,3,4,10,20,30},{1,3,4,10,20,30,40,50},{1,3,4,5,6,7,10,20,30,40,50}};
    char *c2[4][11] = {{"01","02","03","04","10","20","30"},{"01","03","04","10","20","30"},
    {"01","03","04","10","20","30","40","50"},{"01","03","04","05","06","07","10","20","30","40","50"}};
    List_str *ls1, *ls2;
    List_int *li1, *li2;
    {
        int a1[] = {10,20,30}, a2[] = {1,2,3,4};
        char *a3[] = {"10","20","30"}, *a4[] = {"01","02","03","04"};
        li1 = list_new_fromArray(int, a1, 3);
        li2 = list_new_fromArray(int, a2, 4);
        ls1 = list_new_fromArray(str, a3, 3);
        ls2 = list_new_fromArray(str, a4, 4);
    }

    list_splice(int, li1, li1->front, li2); list_splice(str, ls1, ls1->front, ls2);
    compare_ints(li1, c1[0], 7);
    compare_ints(li2, secondCmpInt, 0);
    compare_strs(ls1, c2[0], 7);
    compare_strs(ls2, c2[0], 0);

    list_splice_element(int, li2, NULL, li1, li1->front->next);
    list_splice_element(str, ls2, NULL, ls1, ls1->front->next);
    compare_ints(li1, c1[1], 6);
    compare_ints(li2, secondCmpInt, 1);
    compare_strs(ls1, c2[1], 6);
    compare_strs(ls2, secondCmpStr, 1);

    {
        int a1[] = {40,50}; char *a2[] = {"40","50"};
        list_insert_fromArray(int, li2, NULL, a1, 2);
        list_insert_fromArray(str, ls2, NULL, a2, 2);
    }
    list_splice_range(int, li1, NULL, li2, li2->front->next, NULL);
    list_splice_range(str, ls1, NULL, ls2, ls2->front->next, NULL);
    compare_ints(li1, c1[2], 8);
    compare_ints(li2, secondCmpInt, 1);
    compare_strs(ls1, c2[2], 8);
    compare_strs(ls2, secondCmpStr, 1);

    {
        int a1[] = {5,6,7}; char *a2[] = {"05","06","07"};
        ListEntry_int *x = li1->front;
        ListEntry_str *y = ls1->front;
        iter_advance(LIST, int, x, 3); iter_advance(LIST, str, y, 3);
        list_insert_fromArray(int, li2, li2->front, a1, 3);
        list_insert_fromArray(str, ls2, ls2->front, a2, 3);
        list_splice_range(int, li1, x, li2, li2->front, li2->back);
        list_splice_range(str, ls1, y, ls2, ls2->front, ls2->back);
    }
    compare_ints(li1, c1[3], 11);
    compare_ints(li2, secondCmpInt, 1);
    compare_strs(ls1, c2[3], 11);
    compare_strs(ls2, secondCmpStr, 1);
    list_free(int, li1); list_free(int, li2);
    list_free(str, ls1); list_free(str, ls2);
}

void test_alg_funcs(void) {
    int c1[4][5] = {{0,5,10,15,20},{10},{0,5},{0,5,15,20}};
    char *c2[4][5] = {{"000","005","010","015","020"},{"010"},{"000","005"},{"000","005","015","020"}};
    List_str *ls1 = list_new_fromArray(str, strs, 3);
    List_str *ls2 = list_new_fromArray(str, &strs[2], 3), *rs;
    List_int *li1 = list_new_fromArray(int, ints, 3);
    List_int *li2 = list_new_fromArray(int, &ints[2], 3), *ri;

    ri = set_union_list(int, li1, li2);
    rs = set_union_list(str, ls1, ls2);
    compare_ints(ri, c1[0], 5);
    compare_strs(rs, c2[0], 5);
    list_free(int, ri); list_free(str, rs);

    ri = set_intersection_list(int, li1, li2);
    rs = set_intersection_list(str, ls1, ls2);
    compare_ints(ri, c1[1], 1);
    compare_strs(rs, c2[1], 1);
    list_free(int, ri); list_free(str, rs);

    ri = set_difference_list(int, li1, li2);
    rs = set_difference_list(str, ls1, ls2);
    compare_ints(ri, c1[2], 2);
    compare_strs(rs, c2[2], 2);
    list_free(int, ri); list_free(str, rs);

    ri = set_symmetric_difference_list(int, li1, li2);
    rs = set_symmetric_difference_list(str, ls1, ls2);
    compare_ints(ri, c1[3], 4);
    compare_strs(rs, c2[3], 4);
    list_free(int, ri); list_free(str, rs);

    {
        int a1[2][10] = {{5,10,15,20,25,30,35,40,45,50},{10,20,30,40}};
        char *a2[2][10] = {{"05","10","15","20","25","30","35","40","45","50"},{"10","20","30","40"}};
        List_int *container1 = list_new_fromArray(int, a1[0], 10), *continent1 = list_new_fromArray(int, a1[1], 4);
        List_str *container2 = list_new_fromArray(str, a2[0], 10), *continent2 = list_new_fromArray(str, a2[1], 4);
        assert(includes_list(int, container1, continent1));
        assert(includes_list(str, container2, continent2));
        list_free(int, container1); list_free(int, continent1);
        list_free(str, container2); list_free(str, continent2);
    }
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
