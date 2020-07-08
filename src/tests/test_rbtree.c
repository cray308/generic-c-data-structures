#include "defaults.h"
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

Tree *t1 = NULL;
Tree *t2 = NULL;

void test_01(void) {
    int int_counter = 0;
    int str_counter = 0;
    for (int i = 0; i < LEN_INTS; ++i) {
        tree_insert(t1, &ints[i]);
    }
    int *iptr;
    char **sptr;

    tree_inorder(t1, iptr) {
        assert(*iptr == ints_sorted[int_counter++]);
    }

    for (int i = 0; i < LEN_STRS; ++i) {
        tree_insert(t2, &strs[i]);
    }

    tree_inorder(t2, sptr) {
        assert(streq(*sptr, strs_sorted[str_counter++]));
    }
}

void test_02(void) {
    int test_int = 20384;
    char *test_str = "mail";

    RBNode *n1 = tree_find(t1, &test_int);
    assert(n1 != NULL);
    tree_delete_node(t1, n1);
    n1 = tree_find(t1, &test_int);
    assert(n1 == NULL);

    test_int = 3098;
    tree_delete_by_val(t1, &test_int);
    n1 = tree_find(t1, &test_int);
    assert(n1 == NULL);

    RBNode *n2 = tree_find(t2, &test_str);
    assert(n2 != NULL);
    tree_delete_node(t2, n2);
    n2 = tree_find(t2, &test_str);
    assert(n2 == NULL);

    test_str = "git";
    tree_delete_by_val(t2, &test_str);
    n2 = tree_find(t2, &test_str);
    assert(n2 == NULL);
}

int main(void) {
    t1 = tree_new(&int_helper);
    assert(t1->root == NULL);

    t2 = tree_new(&str_val_helper);
    assert(t2->root == NULL);

    test_01();
    test_02();

    tree_free(t1);
    tree_free(t2);
    
    return 0;
}
