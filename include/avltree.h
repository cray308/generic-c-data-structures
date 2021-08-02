#ifndef DS_AVL_TREE_H
#define DS_AVL_TREE_H

#include "ds.h"

#define __setup_avltree_headers(id, kt, TreeType, DataType, EntryType)                                       \
                                                                                                             \
typedef struct EntryType EntryType;                                                                          \
struct EntryType {                                                                                           \
    EntryType *parent;                                                                                       \
    EntryType *left;                                                                                         \
    EntryType *right;                                                                                        \
    signed char bf;                                                                                          \
    DataType data;                                                                                           \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    EntryType *root;                                                                                         \
    unsigned size;                                                                                           \
} TreeType;                                                                                                  \
                                                                                                             \
EntryType *__avl_successor_##id(EntryType *x);                                                               \
EntryType *__avl_predecessor_##id(EntryType *x);                                                             \
EntryType *__avl_inorder_successor_##id(EntryType *x);                                                       \
EntryType *__avl_inorder_predecessor_##id(EntryType *x);                                                     \
void __avlEntry_advance_##id(EntryType **p1, long n);                                                        \
unsigned __avlEntry_distance_##id(EntryType *p1, EntryType *p2);                                             \
                                                                                                             \
EntryType *__avltree_find_key_##id(TreeType *this, const kt key, unsigned char candidate);                   \
EntryType *__avltree_insert_##id(TreeType *this, DataType data, int *inserted);                              \
unsigned char __avltree_insert_fromArray_##id(TreeType *this, DataType *arr, unsigned n);                    \
unsigned char __avltree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end);              \
TreeType *__avltree_new_fromArray_##id(DataType *arr, unsigned n);                                           \
TreeType *__avltree_createCopy_##id(TreeType *other);                                                        \
void __avltree_remove_entry_##id(TreeType *this, EntryType *v);                                              \
void __avltree_erase_##id(TreeType *this, EntryType *begin, EntryType *end);                                 \

#define __setup_avltree_source(id, kt, TreeType, DataType, EntryType, cmp_lt, entry_get_key, data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
EntryType *__avl_successor_##id(EntryType *x) {                                                              \
    if (!x) return NULL;                                                                                     \
    while (x->left) x = x->left;                                                                             \
    return x;                                                                                                \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_predecessor_##id(EntryType *x) {                                                            \
    if (!x) return NULL;                                                                                     \
    while (x->right) x = x->right;                                                                           \
    return x;                                                                                                \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_inorder_successor_##id(EntryType *x) {                                                      \
    EntryType *parent;                                                                                       \
    if (!x) return NULL;                                                                                     \
    else if (x->right) return __avl_successor_##id(x->right);                                                \
                                                                                                             \
    for (parent = x->parent; parent && x == parent->right; x = parent, parent = parent->parent);             \
    return parent;                                                                                           \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_inorder_predecessor_##id(EntryType *x) {                                                    \
    EntryType *parent;                                                                                       \
    if (!x) return NULL;                                                                                     \
    else if (x->left) return __avl_predecessor_##id(x->left);                                                \
                                                                                                             \
    for (parent = x->parent; parent && x == parent->left; x = parent, parent = parent->parent);              \
    return parent;                                                                                           \
}                                                                                                            \
                                                                                                             \
void __avlEntry_advance_##id(EntryType **p1, long n) {                                                       \
    long count = 0;                                                                                          \
    EntryType *curr = *p1;                                                                                   \
    if (n >= 0) {                                                                                            \
        for (; count != n && curr != NULL; ++count) {                                                        \
            curr = __avl_inorder_successor_##id(curr);                                                       \
        }                                                                                                    \
    } else {                                                                                                 \
        for (; count != n && curr != NULL; --count) {                                                        \
            curr = __avl_inorder_predecessor_##id(curr);                                                     \
        }                                                                                                    \
    }                                                                                                        \
    *p1 = curr;                                                                                              \
}                                                                                                            \
                                                                                                             \
unsigned __avlEntry_distance_##id(EntryType *p1, EntryType *p2) {                                            \
    unsigned dist = 0;                                                                                       \
    for (; p1 && p1 != p2; p1 = __avl_inorder_successor_##id(p1)) ++dist;                                    \
    if (!p1 || p1 != p2) return 4294967295;                                                                  \
    return dist;                                                                                             \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_leftRotate_##id(TreeType *this, EntryType *x) {                                             \
    EntryType *nParent = x->right;                                                                           \
    if (x == this->root) {                                                                                   \
        this->root = nParent;                                                                                \
    }                                                                                                        \
    x->right = nParent->left;                                                                                \
    if (x->right) {                                                                                          \
        x->right->parent = x;                                                                                \
    }                                                                                                        \
    nParent->parent = x->parent;                                                                             \
    if (x->parent) {                                                                                         \
        if (x == x->parent->left) {                                                                          \
            x->parent->left = nParent;                                                                       \
        } else {                                                                                             \
            x->parent->right = nParent;                                                                      \
        }                                                                                                    \
    }                                                                                                        \
    nParent->left = x;                                                                                       \
    x->parent = nParent;                                                                                     \
    return nParent;                                                                                          \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_rightRotate_##id(TreeType *this, EntryType *x) {                                            \
    EntryType *nParent = x->left;                                                                            \
    if (x == this->root) {                                                                                   \
        this->root = nParent;                                                                                \
    }                                                                                                        \
    x->left = nParent->right;                                                                                \
    if (x->left) {                                                                                           \
        x->left->parent = x;                                                                                 \
    }                                                                                                        \
    nParent->parent = x->parent;                                                                             \
    if (x->parent) {                                                                                         \
        if (x == x->parent->left) {                                                                          \
            x->parent->left = nParent;                                                                       \
        } else {                                                                                             \
            x->parent->right = nParent;                                                                      \
        }                                                                                                    \
    }                                                                                                        \
    nParent->right = x;                                                                                      \
    x->parent = nParent;                                                                                     \
    return nParent;                                                                                          \
}                                                                                                            \
                                                                                                             \
EntryType *__avltree_find_key_##id(TreeType *this, const kt key, unsigned char candidate) {                  \
    EntryType *curr = this->root;                                                                            \
    while (curr) {                                                                                           \
        if (cmp_lt(key, entry_get_key(curr))) {                                                              \
            if (!curr->left) {                                                                               \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                curr = curr->left;                                                                           \
            }                                                                                                \
        } else if (cmp_lt(entry_get_key(curr), key)) {                                                       \
            if (!curr->right) {                                                                              \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                curr = curr->right;                                                                          \
            }                                                                                                \
        } else {                                                                                             \
            break;                                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    return curr;                                                                                             \
}                                                                                                            \
                                                                                                             \
EntryType *__avltree_insert_##id(TreeType *this, DataType data, int *inserted) {                             \
    EntryType *curr = __avltree_find_key_##id(this, data_get_key(data), 1);                                  \
    EntryType *new, *parent;                                                                                 \
    if (this->size == 2147483648) return NULL;                                                               \
    if (curr && ds_cmp_eq(cmp_lt, entry_get_key(curr), data_get_key(data))) {                                \
        deleteValue(curr->data.second);                                                                      \
        copyValue(curr->data.second, data.second);                                                           \
        if (inserted) *inserted = 0;                                                                         \
        return curr;                                                                                         \
    }                                                                                                        \
    new = calloc(1, sizeof(EntryType));                                                                      \
    if (!new) return NULL;                                                                                   \
    copyKey(entry_get_key(new), data_get_key(data));                                                         \
    copyValue(new->data.second, data.second);                                                                \
    new->parent = curr;                                                                                      \
                                                                                                             \
    if (!this->root) {                                                                                       \
        this->root = new;                                                                                    \
        ++this->size;                                                                                        \
        if (inserted) *inserted = 1;                                                                         \
        return new;                                                                                          \
    }                                                                                                        \
                                                                                                             \
    if (cmp_lt(data_get_key(data), entry_get_key(curr))) {                                                   \
        curr->left = new;                                                                                    \
    } else {                                                                                                 \
        curr->right = new;                                                                                   \
    }                                                                                                        \
    curr = new, parent = curr->parent;                                                                       \
                                                                                                             \
    for (; curr && parent; curr = parent, parent = curr->parent) {                                           \
        if (curr == parent->left) {                                                                          \
            if (parent->bf == 1) {                                                                           \
                parent->bf = 0; break;                                                                       \
            } else if (parent->bf == 0) {                                                                    \
                parent->bf = -1;                                                                             \
            } else if (parent->bf == -1) {                                                                   \
                if (parent->left->bf == parent->bf) {                                                        \
                    parent = __avl_rightRotate_##id(this, parent);                                           \
                    parent->bf = parent->right->bf = 0;                                                      \
                } else {                                                                                     \
                    signed char old = parent->left->right->bf;                                               \
                    __avl_leftRotate_##id(this, parent->left);                                               \
                    parent = __avl_rightRotate_##id(this, parent);                                           \
                    parent->bf = 0;                                                                          \
                    if (old == -1) {                                                                         \
                        parent->left->bf = 0;                                                                \
                        parent->right->bf = 1;                                                               \
                    } else if (old == 1) {                                                                   \
                        parent->left->bf = -1;                                                               \
                        parent->right->bf = 0;                                                               \
                    } else if (old == 0) {                                                                   \
                        parent->left->bf = parent->right->bf = 0;                                            \
                    }                                                                                        \
                }                                                                                            \
                break;                                                                                       \
            }                                                                                                \
        } else {                                                                                             \
            if (parent->bf == -1) {                                                                          \
                parent->bf = 0; break;                                                                       \
            } else if (parent->bf == 0) {                                                                    \
                parent->bf = 1;                                                                              \
            } else if (parent->bf == 1) {                                                                    \
                if (parent->right->bf == parent->bf) {                                                       \
                    parent = __avl_leftRotate_##id(this, parent);                                            \
                    parent->bf = parent->left->bf = 0;                                                       \
                } else {                                                                                     \
                    signed char old = parent->right->left->bf;                                               \
                    __avl_rightRotate_##id(this, parent->right);                                             \
                    parent = __avl_leftRotate_##id(this, parent);                                            \
                    parent->bf = 0;                                                                          \
                    if (old == -1) {                                                                         \
                        parent->left->bf = 0;                                                                \
                        parent->right->bf = 1;                                                               \
                    } else if (old == 1) {                                                                   \
                        parent->left->bf = -1;                                                               \
                        parent->right->bf = 0;                                                               \
                    } else if (old == 0) {                                                                   \
                        parent->left->bf = parent->right->bf = 0;                                            \
                    }                                                                                        \
                }                                                                                            \
                break;                                                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
    ++this->size;                                                                                            \
    if (inserted) *inserted = 1;                                                                             \
    return new;                                                                                              \
}                                                                                                            \
                                                                                                             \
unsigned char __avltree_insert_fromArray_##id(TreeType *this, DataType *arr, unsigned n) {                   \
    unsigned i;                                                                                              \
    if (!(arr && n)) return 1;                                                                               \
    for (i = 0; i < n; ++i) {                                                                                \
        if (!__avltree_insert_##id(this, arr[i], NULL)) return 0;                                            \
    }                                                                                                        \
    return 1;                                                                                                \
}                                                                                                            \
                                                                                                             \
unsigned char __avltree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end) {             \
    if (!start) return 1;                                                                                    \
    for (; start != end; start = __avl_inorder_successor_##id(start)) {                                      \
        if (!__avltree_insert_##id(this, start->data, NULL)) return 0;                                       \
    }                                                                                                        \
    return 1;                                                                                                \
}                                                                                                            \
                                                                                                             \
TreeType *__avltree_new_fromArray_##id(DataType *arr, unsigned n) {                                          \
    TreeType *t = calloc(1, sizeof(TreeType));                                                               \
    if (t) __avltree_insert_fromArray_##id(t, arr, n);                                                       \
    return t;                                                                                                \
}                                                                                                            \
                                                                                                             \
TreeType *__avltree_createCopy_##id(TreeType *other) {                                                       \
    TreeType *t = __avltree_new_fromArray_##id(NULL, 0);                                                     \
    if (t) __avltree_insert_fromTree_##id(t, __avl_successor_##id(other->root), NULL);                       \
    return t;                                                                                                \
}                                                                                                            \
                                                                                                             \
void __avltree_remove_entry_##id(TreeType *this, EntryType *v) {                                             \
    char deleteData = 1;                                                                                     \
    EntryType *curr, *parent, *child;                                                                        \
    if (!v) return;                                                                                          \
    if (v->left && v->right) {                                                                               \
        EntryType *temp = __avl_inorder_successor_##id(v);                                                   \
        deleteData = 0;                                                                                      \
        deleteKey(entry_get_key(v));                                                                         \
        deleteValue(v->data.second);                                                                         \
        v->data = temp->data;                                                                                \
        v = temp;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    curr = v, parent = curr->parent;                                                                         \
    while (curr && parent) {                                                                                 \
        if (curr == parent->left) {                                                                          \
            if (parent->bf == -1) {                                                                          \
                parent->bf = 0;                                                                              \
            } else if (parent->bf == 0) {                                                                    \
                parent->bf = 1;                                                                              \
                break;                                                                                       \
            } else if (parent->bf == 1) {                                                                    \
                if (parent->right->bf == 1) {                                                                \
                    parent = __avl_leftRotate_##id(this, parent);                                            \
                    parent->bf = parent->left->bf = 0;                                                       \
                } else if (parent->right->bf == 0) {                                                         \
                    parent = __avl_leftRotate_##id(this, parent);                                            \
                    parent->bf = -1;                                                                         \
                    parent->left->bf = 1;                                                                    \
                } else if (parent->right->bf == -1) {                                                        \
                    signed char old = parent->right->left->bf;                                               \
                    __avl_rightRotate_##id(this, parent->right);                                             \
                    parent = __avl_leftRotate_##id(this, parent);                                            \
                    parent->bf = 0;                                                                          \
                    if (old == -1) {                                                                         \
                        parent->left->bf = 0;                                                                \
                        parent->right->bf = 1;                                                               \
                    } else if (old == 1) {                                                                   \
                        parent->left->bf = -1;                                                               \
                        parent->right->bf = 0;                                                               \
                    } else if (old == 0) {                                                                   \
                        parent->left->bf = parent->right->bf = 0;                                            \
                    }                                                                                        \
                }                                                                                            \
                if (parent->bf == -1) break;                                                                 \
            }                                                                                                \
        } else {                                                                                             \
            if (parent->bf == 1) {                                                                           \
                parent->bf = 0;                                                                              \
            } else if (parent->bf == 0) {                                                                    \
                parent->bf = -1; break;                                                                      \
            } else if (parent->bf == -1) {                                                                   \
                if (parent->left->bf == -1) {                                                                \
                    parent = __avl_rightRotate_##id(this, parent);                                           \
                    parent->bf = parent->right->bf = 0;                                                      \
                } else if (parent->left->bf == 0) {                                                          \
                    parent = __avl_rightRotate_##id(this, parent);                                           \
                    parent->bf = 1;                                                                          \
                    parent->right->bf = -1;                                                                  \
                } else if (parent->left->bf == 1) {                                                          \
                    signed char old = parent->left->right->bf;                                               \
                    __avl_leftRotate_##id(this, parent->left);                                               \
                    parent = __avl_rightRotate_##id(this, parent);                                           \
                    parent->bf = 0;                                                                          \
                    if (old == -1) {                                                                         \
                        parent->left->bf = 0;                                                                \
                        parent->right->bf = 1;                                                               \
                    } else if (old == 1) {                                                                   \
                        parent->left->bf = -1;                                                               \
                        parent->right->bf = 0;                                                               \
                    } else if (old == 0) {                                                                   \
                        parent->left->bf = parent->right->bf = 0;                                            \
                    }                                                                                        \
                }                                                                                            \
                if (parent->bf == 1) break;                                                                  \
            }                                                                                                \
        }                                                                                                    \
                                                                                                             \
        curr = parent;                                                                                       \
        parent = curr->parent;                                                                               \
    }                                                                                                        \
                                                                                                             \
    if ((child = (v->left ? v->left : v->right))) {                                                          \
        child->parent = v->parent;                                                                           \
    }                                                                                                        \
    if (v->parent) {                                                                                         \
        if (v == v->parent->left) {                                                                          \
            v->parent->left = child;                                                                         \
        } else {                                                                                             \
            v->parent->right = child;                                                                        \
        }                                                                                                    \
    } else if (v == this->root && (!v->left || !v->right)) {                                                 \
        this->root = child;                                                                                  \
    }                                                                                                        \
    if (deleteData) {                                                                                        \
        deleteKey(entry_get_key(v));                                                                         \
        deleteValue(v->data.second);                                                                         \
    }                                                                                                        \
    free(v);                                                                                                 \
    --this->size;                                                                                            \
}                                                                                                            \
                                                                                                             \
void __avltree_erase_##id(TreeType *this, EntryType *begin, EntryType *end) {                                \
    char getNext = 1;                                                                                        \
    kt nextKey;                                                                                              \
    kt keys[256];                                                                                            \
    if (!begin) return;                                                                                      \
                                                                                                             \
    nextKey = entry_get_key(begin);                                                                          \
    while (getNext) {                                                                                        \
        int i, count = 0;                                                                                    \
        for (begin = __avltree_find_key_##id(this, nextKey, 0); begin != end && count < 256; begin = __avl_inorder_successor_##id(begin)) { \
            keys[count++] = entry_get_key(begin);                                                            \
        }                                                                                                    \
                                                                                                             \
        if (begin == end) getNext = 0;                                                                       \
        else nextKey = entry_get_key(begin);                                                                 \
                                                                                                             \
        for (i = 0; i < count; ++i) {                                                                        \
            __avltree_remove_entry_##id(this, __avltree_find_key_##id(this, keys[i], 0));                    \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \

#endif /* DS_AVL_TREE_H */
