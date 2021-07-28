#include "map.h"
#include <assert.h>

#define freeNestedMap(x) map_free(strv_int, x)

gen_map_headers(int_str, int, char *)
gen_map_headers(strv_int, char *, int)
gen_map_headers(strp_int, char *, int)
gen_map_headers(nested, char *, Map_strv_int *)

gen_map_source(int_str, int, char *, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map_source(strv_int, char *, int, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map_source(strp_int, char *, int, ds_cmp_str_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map_source(nested, char *, Map_strv_int *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, freeNestedMap)

int ints_rand[] = {200,25,220,120,5,205,50,15,60,235,10,70,130,105,185,225,90,30,155,100,150,0,95,170,190,
125,210,75,45,160,175,145,55,230,35,65,110,140,115,20,215,85,195,240,245,135,80,180,40,165};
int ints[] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,
130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
char *strs_rand[] = {"200","025","220","120","005","205","050","015","060","235","010","070","130","105",
"185","225","090","030","155","100","150","000","095","170","190","125","210","075","045","160","175",
"145","055","230","035","065","110","140","115","020","215","085","195","240","245","135","080","180",
"040","165"};
char *strs[] = {"000","005","010","015","020","025","030","035","040","045","050","055","060",
"065","070","075","080","085","090","095","100","105","110","115","120","125","130","135","140","145",
"150","155","160","165","170","175","180","185","190","195","200","205","210","215","220","225","230",
"235","240","245"};

#define __compare_str_int_body(id)                                                                           \
    int i = 0;                                                                                               \
    int *at;                                                                                                 \
    MapEntry_##id *it;                                                                                       \
    assert(map_size(m) == size);                                                                             \
    if (size) {                                                                                              \
        assert(!map_empty(m));                                                                               \
    } else {                                                                                                 \
        assert(map_empty(m));                                                                                \
    }                                                                                                        \
    map_iter(id, m, it) {                                                                                    \
        assert(streq(it->data.first, keys[i]));                                                              \
        at = map_at(id, m, it->data.first);                                                                  \
        assert(at && *at == values[i]);                                                                      \
        assert(it->data.second == values[i++]);                                                              \
    }                                                                                                        \
    assert(i == size);                                                                                       \
    i = size - 1;                                                                                            \
    map_riter(id, m, it) {                                                                                   \
        assert(streq(it->data.first, keys[i]));                                                              \
        at = map_at(id, m, it->data.first);                                                                  \
        assert(at && *at == values[i]);                                                                      \
        assert(it->data.second == values[i--]);                                                              \
    }                                                                                                        \
    assert(i == -1);

void compare_int_str(Map_int_str *m, int *keys, char **values, int size) {
    int i = 0;
    char **at;
    MapEntry_int_str *it;
    assert(map_size(m) == size);
    if (size) {
        assert(!map_empty(m));
    } else {
        assert(map_empty(m));
    }
    map_iter(int_str, m, it) {
        assert(it->data.first == keys[i]);
        at = map_at(int_str, m, it->data.first);
        assert(at && streq(*at, values[i]));
        assert(streq(it->data.second, values[i++]));
    }
    assert(i == size);
    i = size - 1;
    map_riter(int_str, m, it) {
        assert(it->data.first == keys[i]);
        at = map_at(int_str, m, it->data.first);
        assert(at && streq(*at, values[i]));
        assert(streq(it->data.second, values[i--]));
    }
    assert(i == -1);
}

void compare_strv_int(Map_strv_int *m, char **keys, int *values, int size) {
    __compare_str_int_body(strv_int)
}

void compare_strp_int(Map_strp_int *m, char **keys, int *values, int size) {
    __compare_str_int_body(strp_int)
}

void test_empty_init(void) {
    Map_int_str *m1 = map_new(int_str);
    Map_strv_int *m2 = map_new(strv_int);
    Map_strp_int *m3 = map_new(strp_int);
    compare_int_str(m1, ints, strs, 0);
    compare_strv_int(m2, strs, ints, 0);
    compare_strp_int(m3, strs, ints, 0);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_init_fromArray(void) {
    int c1[] = {200};
    char *c2[] = {"200"};
    Map_int_str *m1 = map_new_fromArray(int_str, NULL, 5);
    Map_strv_int *m2;
    Map_strp_int *m3;
    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = ints_rand[i], arrInt[i].second = strs_rand[i];
        arrStr1[i].first = strs_rand[i], arrStr1[i].second = ints_rand[i];
        arrStr2[i].first = strs_rand[i], arrStr2[i].second = ints_rand[i];
    }
    compare_int_str(m1, ints, strs, 0);
    map_free(int_str, m1);
    m1 = map_new_fromArray(int_str, arrInt, 0);
    compare_int_str(m1, ints, strs, 0);
    map_free(int_str, m1);

    m1 = map_new_fromArray(int_str, arrInt, 1);
    m2 = map_new_fromArray(strv_int, arrStr1, 1);
    m3 = map_new_fromArray(strp_int, arrStr2, 1);
    compare_int_str(m1, c1, c2, 1);
    compare_strv_int(m2, c2, c1, 1);
    compare_strp_int(m3, c2, c1, 1);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);

    m1 = map_new_fromArray(int_str, arrInt, 50);
    m2 = map_new_fromArray(strv_int, arrStr1, 50);
    m3 = map_new_fromArray(strp_int, arrStr2, 50);
    compare_int_str(m1, ints, strs, 50);
    compare_strv_int(m2, strs, ints, 50);
    compare_strp_int(m3, strs, ints, 50);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_createCopy(void) {
    Map_int_str *b = map_new_fromArray(int_str, NULL, 0), *m1;
    Map_strv_int *c, *m2;
    Map_strp_int *d, *m3;
    Pair_int_str arrInt[50];
    Pair_strv_int arrStr1[50];
    Pair_strp_int arrStr2[50];
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = ints_rand[i], arrInt[i].second = strs_rand[i];
        arrStr1[i].first = strs_rand[i], arrStr1[i].second = ints_rand[i];
        arrStr2[i].first = strs_rand[i], arrStr2[i].second = ints_rand[i];
    }
    m1 = map_createCopy(int_str, b);
    compare_int_str(m1, ints, strs, 0);
    map_free(int_str, m1);
    map_free(int_str, b);
    b = map_new_fromArray(int_str, arrInt, 50);
    c = map_new_fromArray(strv_int, arrStr1, 50);
    d = map_new_fromArray(strp_int, arrStr2, 50);
    m1 = map_createCopy(int_str, b);
    m2 = map_createCopy(strv_int, c);
    m3 = map_createCopy(strp_int, d);
    map_free(int_str, b);
    map_free(strv_int, c);
    map_free(strp_int, d);
    compare_int_str(m1, ints, strs, 50);
    compare_strv_int(m2, strs, ints, 50);
    compare_strp_int(m3, strs, ints, 50);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_insert_element(void) {
    Map_int_str *m1 = map_new(int_str);
    Map_strv_int *m2 = map_new(strv_int);
    Map_strp_int *m3 = map_new(strp_int);
    Pair_int_str x;
    Pair_strv_int y;
    Pair_strp_int z;
    int a1[] = {50,40,30,20,10}, c1[] = {10,20,30,40,50}, c3[] = {100,20,30,400,500}, i, inserted = -1;
    char *a2[] = {"050","040","030","020","010"}, *c2[] = {"010","020","030","040","050"}, *c4[] = {"100","020","030","400","500"};

    for (i = 0; i < 5; ++i) {
        x.first = a1[i], x.second = a2[i], y.first = a2[i], y.second = a1[i], z.first = a2[i], z.second = a1[i];
        map_insert_withResult(int_str, m1, x, &inserted);
        assert(inserted);
        inserted = -1;
        map_insert_withResult(strv_int, m2, y, &inserted);
        assert(inserted);
        inserted = -1;
        map_insert_withResult(strp_int, m3, z, &inserted);
        assert(inserted);
        inserted = -1;
    }
    compare_int_str(m1, c1, c2, 5);
    compare_strv_int(m2, c2, c1, 5);
    compare_strp_int(m3, c2, c1, 5);

    x.first = 40, x.second = "400", y.first = "040", y.second = 400, z.first = "040", z.second = 400;
    map_insert_withResult(int_str, m1, x, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strv_int, m2, y, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strp_int, m3, z, &inserted);
    assert(!inserted);
    inserted = -1;
    x.first = 10, x.second = "100", y.first = "010", y.second = 100, z.first = "010", z.second = 100;
    map_insert_withResult(int_str, m1, x, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strv_int, m2, y, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strp_int, m3, z, &inserted);
    assert(!inserted);
    inserted = -1;
    x.first = 50, x.second = "500", y.first = "050", y.second = 500, z.first = "050", z.second = 500;
    map_insert_withResult(int_str, m1, x, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strv_int, m2, y, &inserted);
    assert(!inserted);
    inserted = -1;
    map_insert_withResult(strp_int, m3, z, &inserted);
    assert(!inserted);
    compare_int_str(m1, c1, c4, 5);
    compare_strv_int(m2, c2, c3, 5);
    compare_strp_int(m3, c2, c3, 5);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_insert_fromArray(void) {
    Pair_int_str arrInt[7] = {{50,"050"},{50,"500"},{40,"040"},{30,"030"},{20,"020"},{10,"010"},{30,"300"}};
    Pair_strv_int arrStr1[7] = {{"050",50},{"050",500},{"040",40},{"030",30},{"020",20},{"010",10},{"030",300}};
    Pair_strp_int arrStr2[7] = {{"050",50},{"050",500},{"040",40},{"030",30},{"020",20},{"010",10},{"030",300}};
    int k1[] = {10,20,30,40,50}, v2[] = {10,20,300,40,500};
    char *k2[] = {"010","020","030","040","050"}, *v1[] = {"010","020","300","040","500"};
    Map_int_str *m1 = map_new(int_str);
    Map_strv_int *m2 = map_new(strv_int);
    Map_strp_int *m3 = map_new(strp_int);

    map_insert_fromArray(int_str, m1, NULL, 5);
    map_insert_fromArray(int_str, m1, arrInt, 0);
    compare_int_str(m1, k1, v1, 0);
    map_insert_fromArray(int_str, m1, arrInt, 7);
    map_insert_fromArray(strv_int, m2, arrStr1, 7);
    map_insert_fromArray(strp_int, m3, arrStr2, 7);
    compare_int_str(m1, k1, v1, 5);
    compare_strv_int(m2, k2, v2, 5);
    compare_strp_int(m3, k2, v2, 5);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_insert_fromMap(void) {
    int c1[] = {10,20,30,40,50};
    char *c2[] = {"010","020","030","040","050"};
    Pair_int_str arrInt[5] = {{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Map_int_str *b = map_new_fromArray(int_str, arrInt, 5), *m1 = map_new(int_str);
    Map_strv_int *c = map_new_fromArray(strv_int, arrStr1, 5), *m2 = map_new(strv_int);
    Map_strp_int *d = map_new_fromArray(strp_int, arrStr2, 5), *m3 = map_new(strp_int);

    map_insert_fromMap(int_str, m1, NULL, b->root);
    map_insert_fromMap(int_str, m1, b->root, b->root);
    compare_int_str(m1, c1, c2, 0);

    map_insert_fromMap(int_str, m1, b->root->left->left, b->root->left->left->parent);
    map_insert_fromMap(strv_int, m2, c->root->left->left, c->root->left->left->parent);
    map_insert_fromMap(strp_int, m3, d->root->left->left, d->root->left->left->parent);
    map_insert_fromMap(int_str, m1, b->root->right, NULL);
    map_insert_fromMap(strv_int, m2, c->root->right, NULL);
    map_insert_fromMap(strp_int, m3, d->root->right, NULL);
    map_insert_fromMap(int_str, m1, b->root->left, b->root->right);
    map_insert_fromMap(strv_int, m2, c->root->left, c->root->right);
    map_insert_fromMap(strp_int, m3, d->root->left, d->root->right);
    map_free(int_str, b);
    map_free(strv_int, c);
    map_free(strp_int, d);
    compare_int_str(m1, c1, c2, 5);
    compare_strv_int(m2, c2, c1, 5);
    compare_strp_int(m3, c2, c1, 5);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_remove_key(void) {
    int c1[] = {20,30}, removedInts[] = {0,60,39,41,40,10,50,40}, i;
    char *c2[] = {"020","030"}, *removedStrs[] = {"000","060","039","041","040","010","050","040"};
    Pair_int_str arrInt[5] = {{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Map_int_str *m1 = map_new_fromArray(int_str, arrInt, 5);
    Map_strv_int *m2 = map_new_fromArray(strv_int, arrStr1, 5);
    Map_strp_int *m3 = map_new_fromArray(strp_int, arrStr2, 5);

    for (i = 0; i < 8; ++i) {
        map_remove_key(int_str, m1, removedInts[i]);
        map_remove_key(strv_int, m2, removedStrs[i]);
        map_remove_key(strp_int, m3, removedStrs[i]);
    }
    compare_int_str(m1, c1, c2, 2);
    compare_strv_int(m2, c2, c1, 2);
    compare_strp_int(m3, c2, c1, 2);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_remove_entry(void) {
    int c1[] = {20,30};
    char *c2[] = {"020","030"};
    Pair_int_str arrInt[5] = {{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[5] = {{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Map_int_str *m1 = map_new_fromArray(int_str, arrInt, 5);
    Map_strv_int *m2 = map_new_fromArray(strv_int, arrStr1, 5);
    Map_strp_int *m3 = map_new_fromArray(strp_int, arrStr2, 5);

    map_remove_entry(int_str, m1, NULL);
    map_remove_entry(int_str, m1, m1->root);
    map_remove_entry(strv_int, m2, m2->root);
    map_remove_entry(strp_int, m3, m3->root);
    map_remove_entry(int_str, m1, iter_begin_AVLTREE(int_str, m1, 0));
    map_remove_entry(strv_int, m2, iter_begin_AVLTREE(strv_int, m2, 0));
    map_remove_entry(strp_int, m3, iter_begin_AVLTREE(strp_int, m3, 0));
    map_remove_entry(int_str, m1, iter_rbegin_AVLTREE(int_str, m1, 0));
    map_remove_entry(strv_int, m2, iter_rbegin_AVLTREE(strv_int, m2, 0));
    map_remove_entry(strp_int, m3, iter_rbegin_AVLTREE(strp_int, m3, 0));
    compare_int_str(m1, c1, c2, 2);
    compare_strv_int(m2, c2, c1, 2);
    compare_strp_int(m3, c2, c1, 2);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_erase_entries(void) {
    int c1[] = {30,40,50,80};
    char *c2[] = {"030","040","050","080"};
    Pair_int_str arrInt[10] = {{100,"100"},{90,"090"},{80,"080"},{70,"070"},{60,"060"},{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Map_int_str *m1 = map_new_fromArray(int_str, arrInt, 10);
    Map_strv_int *m2 = map_new_fromArray(strv_int, arrStr1, 10);
    Map_strp_int *m3 = map_new_fromArray(strp_int, arrStr2, 10);
    MapEntry_int_str *p1 = m1->root, *p2 = m1->root;
    MapEntry_strv_int *p3 = m2->root, *p4 = m2->root;
    MapEntry_strp_int *p5 = m3->root, *p6 = m3->root;

    iter_prev_AVLTREE(int_str, p1);
    iter_next_AVLTREE(int_str, p2);
    iter_prev_AVLTREE(strv_int, p3);
    iter_next_AVLTREE(strv_int, p4);
    iter_prev_AVLTREE(strp_int, p5);
    iter_next_AVLTREE(strp_int, p6);
    map_erase(int_str, m1, NULL, p2);
    map_erase(int_str, m1, p1, p1);
    map_erase(int_str, m1, p1, p2);
    map_erase(strv_int, m2, p3, p4);
    map_erase(strp_int, m3, p5, p6);
    p1 = iter_rbegin_AVLTREE(int_str, m1, 0);
    iter_prev_AVLTREE(int_str, p1);
    p3 = iter_rbegin_AVLTREE(strv_int, m2, 0);
    iter_prev_AVLTREE(strv_int, p3);
    p5 = iter_rbegin_AVLTREE(strp_int, m3, 0);
    iter_prev_AVLTREE(strp_int, p5);
    map_erase(int_str, m1, p1, NULL);
    map_erase(strv_int, m2, p3, NULL);
    map_erase(strp_int, m3, p5, NULL);
    p1 = iter_begin_AVLTREE(int_str, m1, 0), p2 = p1;
    iter_advance_AVLTREE(int_str, p2, 2);
    p3 = iter_begin_AVLTREE(strv_int, m2, 0), p4 = p3;
    iter_advance_AVLTREE(strv_int, p4, 2);
    p5 = iter_begin_AVLTREE(strp_int, m3, 0), p6 = p5;
    iter_advance_AVLTREE(strp_int, p6, 2);
    map_erase(int_str, m1, p1, p2);
    map_erase(strv_int, m2, p3, p4);
    map_erase(strp_int, m3, p5, p6);
    compare_int_str(m1, c1, c2, 4);
    compare_strv_int(m2, c2, c1, 4);
    compare_strp_int(m3, c2, c1, 4);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_find(void) {
    Pair_int_str arrInt[10] = {{100,"100"},{90,"090"},{80,"080"},{70,"070"},{60,"060"},{50,"050"},{40,"040"},{30,"030"},{20,"020"},{10,"010"}};
    Pair_strv_int arrStr1[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Pair_strp_int arrStr2[10] = {{"100",100},{"090",90},{"080",80},{"070",70},{"060",60},{"050",50},{"040",40},{"030",30},{"020",20},{"010",10}};
    Map_int_str *m1 = map_new(int_str);
    Map_strv_int *m2 = map_new_fromArray(strv_int, arrStr1, 10);
    Map_strp_int *m3 = map_new_fromArray(strp_int, arrStr2, 10);
    Pair_int_str p1;
    Pair_strv_int p2;
    Pair_strp_int p3;

    assert(!map_find(int_str, m1, 0));
    map_insert_fromArray(int_str, m1, arrInt, 10);
    assert(!map_find(int_str, m1, 9));
    assert(!map_find(strv_int, m2, "009"));
    assert(!map_find(strp_int, m3, "009"));
    assert(!map_find(int_str, m1, 101));
    assert(!map_find(strv_int, m2, "101"));
    assert(!map_find(strp_int, m3, "101"));
    assert(!map_find(int_str, m1, 69));
    assert(!map_find(strv_int, m2, "069"));
    assert(!map_find(strp_int, m3, "069"));
    assert(!map_find(int_str, m1, 71));
    assert(!map_find(strv_int, m2, "071"));
    assert(!map_find(strp_int, m3, "071"));

    p1 = map_find(int_str, m1, 70)->data;
    p2 = map_find(strv_int, m2, "070")->data;
    p3 = map_find(strp_int, m3, "070")->data;
    assert(p1.first == 70 && streq(p1.second, "070"));
    assert(streq(p2.first, "070") && p2.second == 70);
    assert(streq(p3.first, "070") && p3.second == 70);
    p1 = map_find(int_str, m1, 60)->data;
    p2 = map_find(strv_int, m2, "060")->data;
    p3 = map_find(strp_int, m3, "060")->data;
    assert(p1.first == 60 && streq(p1.second, "060"));
    assert(streq(p2.first, "060") && p2.second == 60);
    assert(streq(p3.first, "060") && p3.second == 60);
    p1 = map_find(int_str, m1, 80)->data;
    p2 = map_find(strv_int, m2, "080")->data;
    p3 = map_find(strp_int, m3, "080")->data;
    assert(p1.first == 80 && streq(p1.second, "080"));
    assert(streq(p2.first, "080") && p2.second == 80);
    assert(streq(p3.first, "080") && p3.second == 80);
    p1 = map_find(int_str, m1, 10)->data;
    p2 = map_find(strv_int, m2, "010")->data;
    p3 = map_find(strp_int, m3, "010")->data;
    assert(p1.first == 10 && streq(p1.second, "010"));
    assert(streq(p2.first, "010") && p2.second == 10);
    assert(streq(p3.first, "010") && p3.second == 10);
    p1 = map_find(int_str, m1, 100)->data;
    p2 = map_find(strv_int, m2, "100")->data;
    p3 = map_find(strp_int, m3, "100")->data;
    assert(p1.first == 100 && streq(p1.second, "100"));
    assert(streq(p2.first, "100") && p2.second == 100);
    assert(streq(p3.first, "100") && p3.second == 100);
    map_free(int_str, m1);
    map_free(strv_int, m2);
    map_free(strp_int, m3);
}

void test_nested_dicts(void) {
    Pair_strv_int arrStr[50];
    Map_nested *m = map_new(nested);
    MapEntry_nested *it;
    int i;
    for (i = 0; i < 50; ++i) {
        arrStr[i].first = strs[i], arrStr[i].second = ints[i];
    }

    for (i = 0; i < 5; ++i) {
        int success = 0;
        Pair_nested p;
        Map_strv_int *inner = map_new_fromArray(strv_int, &arrStr[10 * i], 10);
        p.first = arrStr[10 * i].first, p.second = inner;
        map_insert_withResult(nested, m, p, &success);
        assert(success);
        assert(map_size(m) == i + 1);
    }

    for (i = 0; i < 5; ++i) {
        Map_strv_int *inner;
        int j = 0;
        assert((it = map_find(nested, m, arrStr[10 * i].first)));
        inner = it->data.second;
        for (; j < 10; ++j) {
            int value = (i * 10) + j;
            MapEntry_strv_int *ptr = map_find(strv_int, inner, arrStr[value].first);
            assert(ptr && ptr->data.second == arrStr[value].second);
        }
    }

    i = 0;
    map_iter(nested, m, it) {
        assert(streq(it->data.first, arrStr[10 * i].first));
        compare_strv_int(it->data.second, &strs[10 * i], &ints[10 * i], 10);
        ++i;
    }
    assert(i == 5);
    i = 4;
    map_riter(nested, m, it) {
        --i;
    }
    assert(i == -1);
    map_free(nested, m);
}

int main(void) {
    test_empty_init();
    test_init_fromArray();
    test_createCopy();
    test_insert_element();
    test_insert_fromArray();
    test_insert_fromMap();
    test_remove_key();
    test_remove_entry();
    test_erase_entries();
    test_find();
    test_nested_dicts();
    return 0;
}
