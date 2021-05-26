#include <assert.h>
#include "avltree.h"

#define __tree_entry_get_key(e) ((e)->data)
#define __tree_data_get_key(d)  (d)
#define __tree_copy_value(x, y)
#define __tree_delete_value(x)

#define tree_iter(id, s, it) for (it = iter_begin(AVLTREE, id, s, 0); it != iter_end(AVLTREE, id, s, 0); iter_next(AVLTREE, id, it))
#define tree_riter(id, s, it) for (it = iter_rbegin(AVLTREE, id, s, 0); it != iter_rend(AVLTREE, id, s, 0); iter_prev(AVLTREE, id, it))
#define tree_new(id) __ds_calloc(1, sizeof(AVLTree_##id))
#define tree_free(id, s) __avltree_free_##id(s)
#define tree_find(id, s, value) __avltree_find_key_##id(s, value, false)
#define tree_insert(id, s, value) __avltree_insert_##id(s, value, NULL)
#define tree_remove_value(id, s, value) __avltree_remove_key_##id(s, value)
#define tree_remove_entry(id, s, entry) __avltree_remove_entry_##id(s, entry)

__gen_avltree(int, int, ds_cmp_num_lt, AVLTree_int, int, AVLNode_int, __tree_entry_get_key, __tree_data_get_key, DSDefault_shallowCopy, DSDefault_shallowDelete, __tree_copy_value, __tree_delete_value)
__gen_avltree(str, char *, ds_cmp_str_lt, AVLTree_str, char *, AVLNode_str, __tree_entry_get_key, __tree_data_get_key, DSDefault_deepCopyStr, DSDefault_deepDelete, __tree_copy_value, __tree_delete_value)

void test_increasing_ints(void) {
    AVLTree_int *t = tree_new(int);
    assert(t->size == 0);
    assert(t->root == NULL);

    tree_insert(int, t, 1);
    assert(t->root != NULL);
    assert(t->root->data == 1);

    tree_insert(int, t, 2);
    assert(t->root->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 2);

    tree_insert(int, t, 3);
    assert(t->root != NULL);
    assert(t->size == 3);
    assert(t->root->data == 2);
    assert(t->root->left);
    assert(t->root->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 3);

    tree_insert(int, t, 4);
    assert(t->root != NULL);
    assert(t->size == 4);
    assert(t->root->data == 2);
    assert(t->root->left);
    assert(t->root->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 3);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 4);

    tree_insert(int, t, 5);
    assert(t->root != NULL);
    assert(t->size == 5);
    assert(t->root->data == 2);
    assert(t->root->left);
    assert(t->root->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 4);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 3);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 5);

    tree_insert(int, t, 6);
    assert(t->root != NULL);
    assert(t->size == 6);
    assert(t->root->data == 4);
    assert(t->root->left);
    assert(t->root->left->data == 2);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 3);
    assert(t->root->right);
    assert(t->root->right->data == 5);
    assert(t->root->right->left == NULL);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 6);

    tree_insert(int, t, 7);
    assert(t->root != NULL);
    assert(t->size == 7);
    assert(t->root->data == 4);
    assert(t->root->left);
    assert(t->root->left->data == 2);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 3);
    assert(t->root->right);
    assert(t->root->right->data == 6);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 5);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 7);

    tree_remove_entry(int, t, t->root); // remove node 4
    assert(t->root != NULL);
    assert(t->size == 6);
    assert(t->root->data == 5);
    assert(t->root->right);
    assert(t->root->right->data == 6);
    assert(t->root->right->left == NULL);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 7);

    tree_remove_value(int, t, 2); // remove node 2
    assert(t->root != NULL);
    assert(t->size == 5);
    assert(t->root->data == 5);
    assert(t->root->left);
    assert(t->root->left->data == 3);
    assert(t->root->left->right == NULL);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 6);
    assert(t->root->right->left == NULL);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 7);

    tree_remove_value(int, t, 2);
    assert(t->size == 5);

    tree_remove_entry(int, t, t->root->left->left); // remove node 1
    assert(t->root != NULL);
    assert(t->size == 4);
    assert(t->root->data == 5);
    assert(t->root->left);
    assert(t->root->left->data == 3);
    assert(t->root->left->right == NULL);
    assert(t->root->left->left == NULL);
    assert(t->root->right);
    assert(t->root->right->data == 6);
    assert(t->root->right->left == NULL);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 7);

    tree_remove_entry(int, t, t->root->left); // remove node 3
    assert(t->root != NULL);
    assert(t->size == 3);
    assert(t->root->data == 6);
    assert(t->root->left);
    assert(t->root->left->data == 5);
    assert(t->root->right);
    assert(t->root->right->data == 7);

    tree_remove_entry(int, t, t->root); // remove node 6
    assert(t->root != NULL);
    assert(t->size == 2);
    assert(t->root->data == 7);
    assert(t->root->left);
    assert(t->root->left->data == 5);
    assert(t->root->right == NULL);

    tree_remove_entry(int, t, t->root); // remove node 7
    assert(t->root != NULL);
    assert(t->size == 1);
    assert(t->root->data == 5);
    assert(t->root->left == NULL);
    assert(t->root->right == NULL);

    tree_remove_entry(int, t, t->root); // remove node 5
    assert(t->root == NULL);
    assert(t->size == 0);

    tree_free(int, t);
}

void test_decreasing_ints(void) {
    AVLTree_int *t = tree_new(int);
    assert(t->size == 0);
    assert(t->root == NULL);

    tree_insert(int, t, 10);
    assert(t->root != NULL);
    assert(t->root->data == 10);

    tree_insert(int, t, 9);
    assert(t->root->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 9);

    tree_insert(int, t, 8);
    assert(t->root != NULL);
    assert(t->size == 3);
    assert(t->root->data == 9);
    assert(t->root->left);
    assert(t->root->left->data == 8);
    assert(t->root->right);
    assert(t->root->right->data == 10);

    tree_insert(int, t, 7);
    assert(t->root != NULL);
    assert(t->size == 4);
    assert(t->root->data == 9);
    assert(t->root->left);
    assert(t->root->left->data == 8);
    assert(t->root->right);
    assert(t->root->right->data == 10);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 7);

    tree_insert(int, t, 6);
    assert(t->root != NULL);
    assert(t->size == 5);
    assert(t->root->data == 9);
    assert(t->root->right);
    assert(t->root->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 7);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 8);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 6);

    tree_insert(int, t, 5);
    assert(t->root != NULL);
    assert(t->size == 6);
    assert(t->root->data == 7);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 6);
    assert(t->root->left->right == NULL);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 5);

    tree_insert(int, t, 4);
    assert(t->root != NULL);
    assert(t->size == 7);
    assert(t->root->data == 7);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 5);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 6);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 4);

    tree_insert(int, t, 3);
    assert(t->root != NULL);
    assert(t->size == 8);
    assert(t->root->data == 7);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 5);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 6);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 4);
    assert(t->root->left->left->left);
    assert(t->root->left->left->left->data == 3);

    tree_insert(int, t, 2);
    assert(t->root != NULL);
    assert(t->size == 9);
    assert(t->root->data == 7);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 5);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 6);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 3);
    assert(t->root->left->left->left);
    assert(t->root->left->left->left->data == 2);
    assert(t->root->left->left->right);
    assert(t->root->left->left->right->data == 4);

    tree_insert(int, t, 1);
    assert(t->root != NULL);
    assert(t->size == 10);
    assert(t->root->data == 7);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 3);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 5);
    assert(t->root->left->right->left);
    assert(t->root->left->right->left->data == 4);
    assert(t->root->left->right->right);
    assert(t->root->left->right->right->data == 6);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 2);
    assert(t->root->left->left->right == NULL);
    assert(t->root->left->left->left);
    assert(t->root->left->left->left->data == 1);

    tree_remove_entry(int, t, t->root); // remove node 7
    assert(t->root != NULL);
    assert(t->size == 9);
    assert(t->root->data == 8);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->left == NULL);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 10);
    assert(t->root->left);
    assert(t->root->left->data == 3);

    tree_remove_entry(int, t, t->root->left); // remove node 3
    assert(t->root != NULL);
    assert(t->size == 8);
    assert(t->root->data == 8);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 2);
    assert(t->root->left->left->left);
    assert(t->root->left->left->left->data == 1);
    assert(t->root->left->right);
    assert(t->root->left->right->data == 5);
    assert(t->root->left->right->right);
    assert(t->root->left->right->right->data == 6);

    tree_remove_entry(int, t, t->root->right->right); // remove node 10
    assert(t->root != NULL);
    assert(t->size == 7);
    assert(t->root->data == 4);
    assert(t->root->left);
    assert(t->root->left->data == 2);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 5);
    assert(t->root->right->left->right);
    assert(t->root->right->left->right->data == 6);

    tree_remove_entry(int, t, t->root->left); // remove node 2
    assert(t->root != NULL);
    assert(t->size == 6);
    assert(t->root->data == 5);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 8);
    assert(t->root->right->right);
    assert(t->root->right->right->data == 9);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 6);

    tree_remove_entry(int, t, t->root->right); // remove node 8
    assert(t->root != NULL);
    assert(t->size == 5);
    assert(t->root->data == 5);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->right == NULL);
    assert(t->root->right->left);
    assert(t->root->right->left->data == 6);

    tree_remove_entry(int, t, t->root); // remove node 5
    assert(t->root != NULL);
    assert(t->size == 4);
    assert(t->root->data == 6);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->left->left);
    assert(t->root->left->left->data == 1);
    assert(t->root->right);
    assert(t->root->right->data == 9);
    assert(t->root->right->right == NULL);
    assert(t->root->right->left == NULL);

    tree_remove_entry(int, t, t->root->left->left); // remove node 1
    assert(t->root != NULL);
    assert(t->size == 3);
    assert(t->root->data == 6);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->right);
    assert(t->root->right->data == 9);

    tree_remove_entry(int, t, t->root); // remove node 6
    assert(t->root != NULL);
    assert(t->size == 2);
    assert(t->root->data == 9);
    assert(t->root->left);
    assert(t->root->left->data == 4);
    assert(t->root->right == NULL);

    tree_remove_entry(int, t, t->root); // remove node 9
    assert(t->root != NULL);
    assert(t->size == 1);
    assert(t->root->data == 4);
    assert(t->root->left == NULL);
    assert(t->root->right == NULL);

    tree_remove_entry(int, t, t->root); // remove node 4
    assert(t->root == NULL);
    assert(t->size == 0);
    tree_remove_value(int, t, 10);
    tree_free(int, t);
}

void test_iter(void) {
    AVLTree_int *t = tree_new(int);
    AVLNode_int *it;
    for (int i = 0; i <= 20; ++i) {
        tree_insert(int, t, i);
        assert((int) t->size == i + 1);
        int x = 0;
        tree_iter(int, t, it) {
            assert(it && (it->data == x++));
        }
        assert(x == i + 1);
        x = i;
        tree_riter(int, t, it) {
            assert(it && (it->data == x--));
        }
        assert(x == -1);
    }

    for (int i = 20; i >= 0; --i) {
        tree_remove_value(int, t, i);
        assert((int) t->size == i);
        int x = 0;
        tree_iter(int, t, it) {
            assert(it && (it->data == x++));
        }
        assert(x == i);
        x = i - 1;
        tree_riter(int, t, it) {
            assert(it && (it->data == x--));
        }
        assert(x == -1);
    }

    for (int i = 0; i <= 20; ++i) {
        tree_insert(int, t, i);
        assert((int) t->size == i + 1);
        int x = 0;
        tree_iter(int, t, it) {
            assert(it && (it->data == x++));
        }
        assert(x == i + 1);
        x = i;
        tree_riter(int, t, it) {
            assert(it && (it->data == x--));
        }
        assert(x == -1);
    }

    for (int i = 0; i <= 20; ++i) {
        tree_remove_value(int, t, i);
        assert((int) t->size == 20 - i);
        int x = i + 1;
        tree_iter(int, t, it) {
            assert(it && (it->data == x++));
        }
        assert(x == 21);
        x = 20;
        tree_riter(int, t, it) {
            assert(it && (it->data == x--));
        }
        assert(x == i);
    }
    assert(t->root == NULL);
    int i = 0;
    tree_iter(int, t, it) {
        ++i;
    }
    assert(i == 0);
    tree_free(int, t);
}

void test_strings(void) {
    //char *strs_rev[] = {"20","19","18","17","16","15","14","13","12","11","10","09","08","07","06","05","04","03","02","01","00"};
    char *sortedStrs[] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20"};

    AVLTree_str *t = tree_new(str);
    AVLNode_str *it;
    for (int i = 0; i <= 20; ++i) {
        tree_insert(str, t, sortedStrs[20 - i]);
        assert((int) t->size == i + 1);
        int x = 20 - i;
        tree_iter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x++]));
        }
        assert(x == 21);
        x = 20;
        tree_riter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x--]));
        }
        assert(x == 19 - i);
    }

    for (int i = 20; i >= 0; --i) {
        tree_remove_value(str, t, sortedStrs[i]);
        assert((int) t->size == i);
        int x = 0;
        tree_iter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x++]));
        }
        assert(x == i);
        x = i - 1;
        tree_riter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x--]));
        }
        assert(x == -1);
    }

    for (int i = 0; i <= 20; ++i) {
        tree_insert(str, t, sortedStrs[20 - i]);
        assert((int) t->size == i + 1);
        int x = 20 - i;
        tree_iter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x++]));
        }
        assert(x == 21);
        x = 20;
        tree_riter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x--]));
        }
        assert(x == 19 - i);
    }

    for (int i = 0; i <= 20; ++i) {
        it = tree_find(str, t, sortedStrs[i]);
        assert(it && streq(it->data, sortedStrs[i]));
        tree_remove_entry(str, t, it);
        assert((int) t->size == 20 - i);
        int x = i + 1;
        tree_iter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x++]));
        }
        assert(x == 21);
        x = 20;
        tree_riter(str, t, it) {
            assert(it && streq(it->data, sortedStrs[x--]));
        }
        assert(x == i);
    }
    assert(t->root == NULL);
    int i = 0;
    tree_iter(str, t, it) {
        ++i;
    }
    assert(i == 0);
    tree_free(str, t);
}

int main(void) {
    test_increasing_ints();
    test_decreasing_ints();
    test_iter();
    test_strings();
    return 0;
}
