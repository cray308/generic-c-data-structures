#include "rbtree.h"
#include <assert.h>

int ints[] = {58490, 13829, 44828, 35725, 20384, 46283, 56369, 21807, 15590, 1520, 60285, 11387,
              34372, 49312, 52170, 30455, 52223, 41564, 53018, 3098, 25875, 45077, 36617, 58448,
              42128, 13655, 7077, 62936, 46937, 14427};
char *strs[] = {"root","bin","daemon","mail","ftp","http","nobody","dbus","systemd-journal-remote",
                "systemd-network","systemd-resolve","systemd-timesync","systemd-coredump","uuidd",
                "polkitd","chrisray","avahi","colord","lightdm","git","rtkit","usbmux","geoclue"};

int ints_sorted[] = {1520, 3098, 7077, 11387, 13655, 13829, 14427, 15590, 20384, 21807, 25875,
                    30455, 34372, 35725, 36617, 41564, 42128, 44828, 45077, 46283, 46937, 49312,
                    52170, 52223, 53018, 56369, 58448, 58490, 60285, 62936};

char *strs_sorted[] = {"avahi", "bin", "chrisray", "colord", "daemon", "dbus", "ftp", "geoclue",
                    "git", "http", "lightdm", "mail", "nobody", "polkitd", "root", "rtkit",
                    "systemd-coredump", "systemd-journal-remote", "systemd-network",
                    "systemd-resolve", "systemd-timesync", "usbmux", "uuidd"};

#define LEN_INTS 30
#define LEN_STRS 23

gen_rbtree(int, int, ds_cmp_num_lt)
gen_rbtree(str, char *, ds_cmp_str_lt)

void test_insert_ints(Tree_int *t) {
    size_t counter = 0;
    TreeEntry_int *ptr;

    for (int i = 0; i < LEN_INTS; ++i) {
        tree_insert(int, t, ints[i]);
    }
    tree_iter(int, t, ptr) {
        assert(iter_deref(TREE, ptr) == ints_sorted[counter++]);
    }
    assert(counter == LEN_INTS);
    assert(counter == t->size);

    counter = LEN_INTS - 1;
    tree_riter(int, t, ptr) {
        assert(iter_deref(TREE, ptr) == ints_sorted[counter--]);
    }
}

void test_delete_ints(Tree_int *t) {
    int test_int = 20384;

    TreeEntry_int *e = tree_find(int, t, test_int);
    assert(e != NULL);
    tree_delete_node(int, t, e);
    e = tree_find(int, t, test_int);
    assert(e == NULL);

    test_int = 3098;
    tree_delete_by_val(int, t, test_int);
    e = tree_find(int, t, test_int);
    assert(e == NULL);
}

void test_ints(void) {
    Tree_int *t = tree_new(int);
    assert(t->root == NULL);
    test_insert_ints(t);
    test_delete_ints(t);
    tree_free(int, t);
}

void test_insert_strs(Tree_str *t) {
    size_t counter = 0;
    TreeEntry_str *ptr;

    for (int i = 0; i < LEN_STRS; ++i) {
        tree_insert(str, t, strs[i]);
    }
    tree_iter(str, t, ptr) {
        assert(streq(iter_deref(TREE, ptr), strs_sorted[counter++]));
    }
    assert(counter == LEN_STRS);
    assert(counter == t->size);

    counter = LEN_STRS - 1;
    tree_riter(str, t, ptr) {
        assert(streq(iter_deref(TREE, ptr), strs_sorted[counter--]));
    }
}

void test_delete_strs(Tree_str *t) {
    char *test_str = "mail";

    TreeEntry_str *e = tree_find(str, t, test_str);
    assert(e != NULL);
    tree_delete_node(str, t, e);
    e = tree_find(str, t, test_str);
    assert(e == NULL);

    test_str = "git";
    tree_delete_by_val(str, t, test_str);
    e = tree_find(str, t, test_str);
    assert(e == NULL);
}

void test_strs(void) {
    Tree_str *t = tree_new(str);
    test_insert_strs(t);
    test_delete_strs(t);
    tree_free(str, t);
}

int main(void) {
    test_ints();
    test_strs();
    return 0;
}
