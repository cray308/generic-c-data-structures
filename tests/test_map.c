#include "map.h"
#include <assert.h>

gen_map(int_str, int, char *, ds_cmp_num_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(strv_int, char *, int, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(strp_int, char *, int, ds_cmp_str_lt, DSDefault_shallowCopy, DSDefault_shallowDelete, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_map(nested, char *, Map_strv_int *, ds_cmp_str_lt, DSDefault_deepCopyStr, DSDefault_deepDelete, DSDefault_shallowCopy, __avltree_free_strv_int)

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

void compare_int_str(Map_int_str *m, int *keys, char **values, int size) {
    int i = 0;
    MapEntry_int_str *it;
    assert(map_size(m) == size);
    map_iter(int_str, m, it) {
        assert(it->data.first == keys[i]);
        assert(streq(it->data.second, values[i++]));
    }
    assert(i == size);
    i = size - 1;
    map_riter(int_str, m, it) {
        assert(it->data.first == keys[i]);
        assert(streq(it->data.second, values[i--]));
    }
    assert(i == -1);
}

void compare_str_int(Map_strv_int *m, char **keys, int *values, int size) {
    int i = 0;
    MapEntry_strv_int *it;
    assert(map_size(m) == size);

    map_iter(strv_int, m, it) {
        assert(streq(it->data.first, keys[i]));
        assert(it->data.second == values[i++]);
    }
    assert(i == size);
    i = size - 1;
    map_riter(strv_int, m, it) {
        assert(streq(it->data.first, keys[i]));
        assert(it->data.second == values[i--]);
    }
    assert(i == -1);
}

void test_basic_ints(void) {
    Map_int_str *m = map_new(int_str);
    MapEntry_int_str *e; int i; Pair_int_str p;
    assert(map_empty(m));
    compare_int_str(m, ints, strs, 0);

    for (i = 0; i < 50; ++i) {
        p.first = ints_rand[i], p.second = strs_rand[i];
        map_insert(int_str, m, p);
    }
    p.first = ints[0], p.second = strs[0];
    map_insert(int_str, m, p);
    p.first = ints[49], p.second = strs[49];
    map_insert(int_str, m, p);
    assert(!map_empty(m));
    compare_int_str(m, ints, strs, 50);

    assert(map_find(int_str, m, 121) == NULL);
    e = map_find(int_str, m, 120);
    assert(e != NULL);
    map_remove_entry(int_str, m, e);
    assert(map_find(int_str, m, 120) == NULL);
    assert(map_size(m) == 49);
    assert(m->root->data.first == 125);

    map_remove_key(int_str, m, 60);
    assert(map_find(int_str, m, 60) == NULL);
    map_remove_key(int_str, m, 60);
    assert(map_size(m) == 48);
    map_free(int_str, m);
}

void test_basic_strs(void) {
    Map_strp_int *m = map_new(strp_int);
    MapEntry_strp_int *it; int i; Pair_strp_int p;
    assert(map_empty(m));
    assert(map_size(m) == 0);

    i = 0;
    map_iter(strp_int, m, it) {
        i++;
    }
    assert(it == NULL);
    assert(i == 0);
    i = -1;
    map_riter(strp_int, m, it) {
        i--;
    }
    assert(i == -1);

    for (i = 0; i < 50; ++i) {
        p.first = strs_rand[i], p.second = ints_rand[i];
        map_insert(strp_int, m, p);
    }
    p.first = strs[0], p.second = ints[0];
    map_insert(strp_int, m, p);
    p.first = strs[49], p.second = ints[49];
    map_insert(strp_int, m, p);
    assert(!map_empty(m));

    i = 0;
    map_iter(strp_int, m, it) {
        assert(streq(it->data.first, strs[i]));
        assert(it->data.second == ints[i++]);
    }
    assert(i == 50);
    i = 49;
    map_riter(strp_int, m, it) {
        assert(streq(it->data.first, strs[i]));
        assert(it->data.second == ints[i--]);
    }
    assert(i == -1);

    assert(map_find(strp_int, m, "121") == NULL);
    it = map_find(strp_int, m, "120");
    assert(it != NULL);
    map_remove_entry(strp_int, m, it);
    assert(map_find(strp_int, m, "120") == NULL);
    assert(map_size(m) == 49);
    assert(streq(m->root->data.first, "125"));

    map_remove_key(strp_int, m, "060");
    assert(map_find(strp_int, m, "060") == NULL);
    map_remove_key(strp_int, m, "060");
    assert(map_size(m) == 48);
    map_free(strp_int, m);
}

void test_init_clear(void) {
    Map_int_str *m1, *m3; Map_strv_int *m2, *m4;
    Pair_int_str arrInt[50]; Pair_strv_int arrStr[50];
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = ints_rand[i], arrInt[i].second = strs_rand[i];
        arrStr[i].first = strs_rand[i], arrStr[i].second = ints_rand[i];
    }
    m1 = map_new_fromArray(int_str, arrInt, 50);
    m2 = map_new_fromArray(strv_int, arrStr, 50);
    compare_int_str(m1, ints, strs, 50);
    compare_str_int(m2, strs, ints, 50);

    m3 = map_createCopy(int_str, m1);
    m4 = map_createCopy(strv_int, m2);
    compare_int_str(m1, ints, strs, 50);
    compare_str_int(m2, strs, ints, 50);

    map_clear(int_str, m1); map_clear(strv_int, m2);
    compare_int_str(m1, ints, strs, 0);
    compare_str_int(m2, strs, ints, 0);

    map_free(int_str, m1); map_free(strv_int, m2);
    map_free(int_str, m3); map_free(strv_int, m4);
}

void test_membership(void) {
    Map_int_str *m1; Map_strv_int *m2;
    MapEntry_int_str *p1; MapEntry_strv_int *p2;
    Pair_int_str arrInt[50]; Pair_strv_int arrStr[50];
    int i;
    for (i = 0; i < 50; ++i) {
        arrInt[i].first = ints_rand[i], arrInt[i].second = strs_rand[i];
        arrStr[i].first = strs_rand[i], arrStr[i].second = ints_rand[i];
    }
    m1 = map_new_fromArray(int_str, arrInt, 50);
    m2 = map_new_fromArray(strv_int, arrStr, 50);
    assert(!map_find(int_str, m1, 64) && !map_find(strv_int, m2, "064"));
    assert(!map_find(int_str, m1, 121) && !map_find(strv_int, m2, "121"));
    assert(map_find(int_str, m1, 0) && map_find(strv_int, m2, "000"));
    assert(map_find(int_str, m1, 245) && map_find(strv_int, m2, "245"));
    assert(!map_find(int_str, m1, -1) && !map_find(strv_int, m2, "..."));
    assert(!map_find(int_str, m1, 246) && !map_find(strv_int, m2, "246"));

    *map_at(int_str, m1, 120) = "500";
    p1 = map_find(int_str, m1, 120);
    assert(p1);
    assert(p1->data.first == 120 && streq(p1->data.second, "500"));
    *map_at(strv_int, m2, "120") = 500;
    p2 = map_find(strv_int, m2, "120");
    assert(p2);
    assert(streq(p2->data.first, "120") && p2->data.second == 500);
    map_free(int_str, m1); map_free(strv_int, m2);
}

void test_remove(void) {
    int c1[] = {10,20,35,40};
    char *c2[] = {"010","020","035","040"};
    Map_int_str *m1; Map_strv_int *m2;
    MapEntry_int_str *begin1 = NULL, *end1 = NULL;
    MapEntry_strv_int *begin2 = NULL, *end2 = NULL;
    Pair_int_str arrInt[10]; Pair_strv_int arrStr[10];
    int i;
    for (i = 0; i < 10; ++i) {
        arrInt[i].first = ints[i], arrInt[i].second = strs[i];
        arrStr[i].first = strs[i], arrStr[i].second = ints[i];
    }
    m1 = map_new_fromArray(int_str, arrInt, 10);
    m2 = map_new_fromArray(strv_int, arrStr, 10);

    map_remove_entry(int_str, m1, m1->root); map_remove_entry(strv_int, m2, m2->root);
    map_remove_key(int_str, m1, 5); map_remove_key(strv_int, m2, "005");

    map_erase(int_str, m1, begin1, end1); map_erase(strv_int, m2, begin2, end2);
    begin1 = map_find(int_str, m1, 0), end1 = begin1;
    begin2 = map_find(strv_int, m2, "000"), end2 = begin2;
    map_erase(int_str, m1, begin1, end1); map_erase(strv_int, m2, begin2, end2);
    assert(map_size(m1) == 8 && map_size(m2) == 8);

    end1 = map_find(int_str, m1, 10), end2 = map_find(strv_int, m2, "010");
    map_erase(int_str, m1, begin1, end1); map_erase(strv_int, m2, begin2, end2);

    begin1 = map_find(int_str, m1, 45), end1 = NULL;
    begin2 = map_find(strv_int, m2, "045"), end2 = NULL;
    map_erase(int_str, m1, begin1, end1); map_erase(strv_int, m2, begin2, end2);

    begin1 = map_find(int_str, m1, 25), end1 = map_find(int_str, m1, 35);
    begin2 = map_find(strv_int, m2, "025"), end2 = map_find(strv_int, m2, "035");
    map_erase(int_str, m1, begin1, end1); map_erase(strv_int, m2, begin2, end2);

    compare_int_str(m1, c1, c2, 4);
    compare_str_int(m2, c2, c1, 4);
    map_free(int_str, m1); map_free(strv_int, m2);
}

void test_insert(void) {
    int c1[3][8] = {{0,4,5,10,14,15,16,20},{0,4,500,10,14,15,16,20},{0,4,16,20}};
    char *c2[3][8] = {{"000","004","500","010","014","015","016","020"},{"000","004","005","010","014","015","016","020"},
    {"000","004","016","020"}};
    Map_int_str *m1, *m3; Map_strv_int *m2, *m4;
    MapEntry_int_str *begin1, *end1; MapEntry_strv_int *begin2, *end2;
    Pair_int_str arrInt[5], p1; Pair_strv_int arrStr[5], p2;
    int i, inserted = -1;
    for (i = 0; i < 5; ++i) {
        arrInt[i].first = ints[i], arrInt[i].second = strs[i];
        arrStr[i].first = strs[i], arrStr[i].second = ints[i];
    }
    m1 = map_new_fromArray(int_str, arrInt, 5);
    m2 = map_new_fromArray(strv_int, arrStr, 5);

    p1.first = 5, p1.second = "500";
    map_insert_withResult(int_str, m1, p1, &inserted);
    assert(!inserted);
    inserted = -1;
    p2.first = "005", p2.second = 500;
    map_insert_withResult(strv_int, m2, p2, &inserted);
    assert(!inserted);
    inserted = -1;
    p1.first = 4, p1.second = "004";
    map_insert_withResult(int_str, m1, p1, &inserted);
    assert(inserted);
    inserted = -1;
    p2.first = "004", p2.second = 4;
    map_insert_withResult(strv_int, m2, p2, &inserted);
    assert(inserted);

    {
        Pair_int_str arr1[3] = {{14, "014"}, {15, "015"}, {16, "016"}};
        Pair_strv_int arr2[3] = {{"014", 14}, {"015", 15}, {"016", 16}};
        map_insert_fromArray(int_str, m1, arr1, 0);
        map_insert_fromArray(strv_int, m2, arr2, 0);
        map_insert_fromArray(int_str, m1, arr1, 3);
        map_insert_fromArray(strv_int, m2, arr2, 3);
    }
    compare_int_str(m1, c1[0], c2[0], 8);
    compare_str_int(m2, c2[1], c1[1], 8);

    m3 = map_new(int_str);
    m4 = map_new(strv_int);
    begin1 = map_find(int_str, m1, 0), end1 = begin1;
    begin2 = map_find(strv_int, m2, "000"), end2 = begin2;
    map_insert_fromMap(int_str, m3, begin1, end1); map_insert_fromMap(strv_int, m4, begin2, end2);
    assert(map_empty(m3) && map_empty(m4));

    end1 = map_find(int_str, m1, 5);
    end2 = map_find(strv_int, m2, "005");
    map_insert_fromMap(int_str, m3, begin1, end1); map_insert_fromMap(strv_int, m4, begin2, end2);

    begin1 = map_find(int_str, m1, 16), end1 = NULL;
    begin2 = map_find(strv_int, m2, "016"), end2 = NULL;
    map_insert_fromMap(int_str, m3, begin1, end1); map_insert_fromMap(strv_int, m4, begin2, end2);
    compare_int_str(m3, c1[2], c2[2], 4);
    compare_str_int(m4, c2[2], c1[2], 4);
    map_free(int_str, m3); map_free(strv_int, m4);
    map_free(int_str, m1); map_free(strv_int, m2);
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
        int success = 0; Pair_nested p;
        Map_strv_int *inner = map_new_fromArray(strv_int, &arrStr[10 * i], 10);
        p.first = arrStr[10 * i].first, p.second = inner;
        map_insert_withResult(nested, m, p, &success);
        assert(success);
        assert(map_size(m) == i + 1);
    }

    for (i = 0; i < 5; ++i) {
        Map_strv_int *inner; int j = 0;
        assert(it = map_find(nested, m, arrStr[10 * i].first));
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
        compare_str_int(it->data.second, &strs[10 * i], &ints[10 * i], 10);
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
    test_basic_ints();
    test_basic_strs();
    test_init_clear();
    test_membership();
    test_remove();
    test_insert();
    test_nested_dicts();
    return 0;
}
