#ifndef DS_AVL_TREE_H
#define DS_AVL_TREE_H

#include "ds.h"

#define __setup_avltree_headers(id, kt, TreeType, DataType, EntryType)                   \
                                                                                         \
typedef struct EntryType EntryType;                                                      \
struct EntryType {                                                                       \
    EntryType *parent;                                                                   \
    EntryType *left;                                                                     \
    EntryType *right;                                                                    \
    signed char bf;                                                                      \
    DataType data;                                                                       \
};                                                                                       \
                                                                                         \
typedef struct {                                                                         \
    EntryType *root;                                                                     \
    unsigned size;                                                                       \
} TreeType;                                                                              \
                                                                                         \
EntryType *__avl_successor_##id(EntryType *x);                                           \
EntryType *__avl_predecessor_##id(EntryType *x);                                         \
EntryType *__avl_inorder_successor_##id(EntryType const *x);                             \
EntryType *__avl_inorder_predecessor_##id(EntryType const *x);                           \
void __avlEntry_advance_##id(EntryType **p1, long n)                                     \
  __attribute__((nonnull (1)));                                                          \
long __avlEntry_distance_##id(EntryType const *p1, EntryType const *p2);                 \
                                                                                         \
EntryType *__avltree_find_key_##id(TreeType const *this,                                 \
                                   kt const key, unsigned char candidate)                \
  __attribute__((nonnull));                                                              \
EntryType *__avltree_insert_##id(TreeType *this,                                         \
                                 DataType const data, int *inserted)                     \
  __attribute__((nonnull (1)));                                                          \
unsigned char __avltree_insert_fromArray_##id(TreeType *this,                            \
                                              DataType const *arr,                       \
                                              unsigned n)                                \
  __attribute__((nonnull (1)));                                                          \
unsigned char __avltree_insert_fromTree_##id(TreeType *this,                             \
                                             EntryType const *start,                     \
                                             EntryType const *end)                       \
  __attribute__((nonnull (1)));                                                          \
TreeType *__avltree_new_fromArray_##id(DataType const *arr, unsigned n);                 \
TreeType *__avltree_createCopy_##id(TreeType const *other)                               \
  __attribute__((nonnull (1)));                                                          \
void __avltree_remove_entry_##id(TreeType *this, EntryType *v)                           \
  __attribute__((nonnull (1)));                                                          \
void __avltree_erase_##id(TreeType *this,                                                \
                          EntryType *begin, EntryType const *end)                        \
  __attribute__((nonnull (1)));                                                          \

#define __setup_avltree_source(id, kt, TreeType, DataType, EntryType, cmp_lt,            \
                               entry_get_key, data_get_key, copyKey, deleteKey,          \
                               copyValue, deleteValue)                                   \
                                                                                         \
EntryType *__avl_successor_##id(EntryType *x) {                                          \
    if (x) {                                                                             \
        for (; x->left; x = x->left);                                                    \
    }                                                                                    \
    return x;                                                                            \
}                                                                                        \
                                                                                         \
EntryType *__avl_predecessor_##id(EntryType *x) {                                        \
    if (x) {                                                                             \
        for (; x->right; x = x->right);                                                  \
    }                                                                                    \
    return x;                                                                            \
}                                                                                        \
                                                                                         \
EntryType *__avl_inorder_successor_##id(EntryType const *x) {                            \
    EntryType *parent;                                                                   \
    if (!x) return NULL;                                                                 \
    else if (x->right) return __avl_successor_##id(x->right);                            \
                                                                                         \
    parent = x->parent;                                                                  \
    for (; parent && x == parent->right; x = parent, parent = parent->parent);           \
    return parent;                                                                       \
}                                                                                        \
                                                                                         \
EntryType *__avl_inorder_predecessor_##id(EntryType const *x) {                          \
    EntryType *parent;                                                                   \
    if (!x) return NULL;                                                                 \
    else if (x->left) return __avl_predecessor_##id(x->left);                            \
                                                                                         \
    parent = x->parent;                                                                  \
    for (; parent && x == parent->left; x = parent, parent = parent->parent);            \
    return parent;                                                                       \
}                                                                                        \
                                                                                         \
void __avlEntry_advance_##id(EntryType **p1, long n) {                                   \
    long count = 0;                                                                      \
    EntryType *curr = *p1;                                                               \
    if (n >= 0) {                                                                        \
        for (; count++ < n && curr;                                                      \
                curr = __avl_inorder_successor_##id(curr));                              \
    } else {                                                                             \
        for (; count-- > n && curr;                                                      \
                curr = __avl_inorder_predecessor_##id(curr));                            \
    }                                                                                    \
    *p1 = curr;                                                                          \
}                                                                                        \
                                                                                         \
long __avlEntry_distance_##id(EntryType const *p1, EntryType const *p2) {                \
    long dist = 0;                                                                       \
    for (; p1 && p1 != p2; p1 = __avl_inorder_successor_##id(p1), ++dist);               \
    if (p1 != p2) return -1;                                                             \
    return dist;                                                                         \
}                                                                                        \
                                                                                         \
static EntryType *__avl_leftRotate_##id(TreeType *this, EntryType *x) {                  \
    EntryType *nParent = x->right;                                                       \
    if (x == this->root) this->root = nParent;                                           \
    x->right = nParent->left;                                                            \
    if (x->right) x->right->parent = x;                                                  \
    nParent->parent = x->parent;                                                         \
    if (x->parent) {                                                                     \
        if (x == x->parent->left) {                                                      \
            x->parent->left = nParent;                                                   \
        } else {                                                                         \
            x->parent->right = nParent;                                                  \
        }                                                                                \
    }                                                                                    \
    nParent->left = x;                                                                   \
    x->parent = nParent;                                                                 \
    return nParent;                                                                      \
}                                                                                        \
                                                                                         \
static EntryType *__avl_rightRotate_##id(TreeType *this, EntryType *x) {                 \
    EntryType *nParent = x->left;                                                        \
    if (x == this->root) this->root = nParent;                                           \
    x->left = nParent->right;                                                            \
    if (x->left) x->left->parent = x;                                                    \
    nParent->parent = x->parent;                                                         \
    if (x->parent) {                                                                     \
        if (x == x->parent->left) {                                                      \
            x->parent->left = nParent;                                                   \
        } else {                                                                         \
            x->parent->right = nParent;                                                  \
        }                                                                                \
    }                                                                                    \
    nParent->right = x;                                                                  \
    x->parent = nParent;                                                                 \
    return nParent;                                                                      \
}                                                                                        \
                                                                                         \
EntryType *__avltree_find_key_##id(TreeType const *this,                                 \
                                   kt const key, unsigned char candidate) {              \
    EntryType *curr = this->root;                                                        \
    while (curr) {                                                                       \
        if (cmp_lt(key, entry_get_key(curr))) {                                          \
            if (!curr->left) {                                                           \
                if (candidate) break;                                                    \
                return NULL;                                                             \
            } else {                                                                     \
                curr = curr->left;                                                       \
            }                                                                            \
        } else if (cmp_lt(entry_get_key(curr), key)) {                                   \
            if (!curr->right) {                                                          \
                if (candidate) break;                                                    \
                return NULL;                                                             \
            } else {                                                                     \
                curr = curr->right;                                                      \
            }                                                                            \
        } else {                                                                         \
            break;                                                                       \
        }                                                                                \
    }                                                                                    \
    return curr;                                                                         \
}                                                                                        \
                                                                                         \
EntryType *__avltree_insert_##id(TreeType *this,                                         \
                                 DataType const data, int *inserted) {                   \
    EntryType *curr = __avltree_find_key_##id(this, data_get_key(data), 1);              \
    EntryType *new, *parent;                                                             \
    if (this->size == UINT_MAX) return NULL;                                             \
    else if (curr && ds_cmp_eq(cmp_lt, entry_get_key(curr),                              \
                               data_get_key(data))) {                                    \
        deleteValue(curr->data.second);                                                  \
        copyValue(curr->data.second, data.second);                                       \
        if (inserted) *inserted = 0;                                                     \
        return curr;                                                                     \
    } else if (!(new = calloc(1, sizeof(EntryType)))) return NULL;                       \
                                                                                         \
    copyKey(entry_get_key(new), data_get_key(data));                                     \
    copyValue(new->data.second, data.second);                                            \
    new->parent = curr;                                                                  \
    if (!curr) {                                                                         \
        customAssert(this->root == NULL)                                                 \
        this->root = new;                                                                \
        ++this->size;                                                                    \
        if (inserted) *inserted = 1;                                                     \
        return new;                                                                      \
    }                                                                                    \
                                                                                         \
    if (cmp_lt(data_get_key(data), entry_get_key(curr))) {                               \
        curr->left = new;                                                                \
    } else {                                                                             \
        curr->right = new;                                                               \
    }                                                                                    \
                                                                                         \
    for (curr = new, parent = curr->parent;                                              \
            curr && parent;                                                              \
            curr = parent, parent = curr->parent) {                                      \
        if (curr == parent->left) {                                                      \
            if (parent->bf == 1) {                                                       \
                parent->bf = 0; break;                                                   \
            } else if (parent->bf == 0) {                                                \
                parent->bf = -1;                                                         \
            } else if (parent->bf == -1) {                                               \
                if (parent->left->bf == parent->bf) {                                    \
                    parent = __avl_rightRotate_##id(this, parent);                       \
                    parent->bf = parent->right->bf = 0;                                  \
                } else {                                                                 \
                    signed char old = parent->left->right->bf;                           \
                    __avl_leftRotate_##id(this, parent->left);                           \
                    parent = __avl_rightRotate_##id(this, parent);                       \
                    parent->bf = 0;                                                      \
                    if (old == -1) {                                                     \
                        parent->left->bf = 0;                                            \
                        parent->right->bf = 1;                                           \
                    } else if (old == 1) {                                               \
                        parent->left->bf = -1;                                           \
                        parent->right->bf = 0;                                           \
                    } else if (old == 0) {                                               \
                        parent->left->bf = parent->right->bf = 0;                        \
                    }                                                                    \
                }                                                                        \
                break;                                                                   \
            }                                                                            \
        } else {                                                                         \
            if (parent->bf == -1) {                                                      \
                parent->bf = 0; break;                                                   \
            } else if (parent->bf == 0) {                                                \
                parent->bf = 1;                                                          \
            } else if (parent->bf == 1) {                                                \
                if (parent->right->bf == parent->bf) {                                   \
                    parent = __avl_leftRotate_##id(this, parent);                        \
                    parent->bf = parent->left->bf = 0;                                   \
                } else {                                                                 \
                    signed char old = parent->right->left->bf;                           \
                    __avl_rightRotate_##id(this, parent->right);                         \
                    parent = __avl_leftRotate_##id(this, parent);                        \
                    parent->bf = 0;                                                      \
                    if (old == -1) {                                                     \
                        parent->left->bf = 0;                                            \
                        parent->right->bf = 1;                                           \
                    } else if (old == 1) {                                               \
                        parent->left->bf = -1;                                           \
                        parent->right->bf = 0;                                           \
                    } else if (old == 0) {                                               \
                        parent->left->bf = parent->right->bf = 0;                        \
                    }                                                                    \
                }                                                                        \
                break;                                                                   \
            }                                                                            \
        }                                                                                \
    }                                                                                    \
    ++this->size;                                                                        \
    if (inserted) *inserted = 1;                                                         \
    return new;                                                                          \
}                                                                                        \
                                                                                         \
unsigned char __avltree_insert_fromArray_##id(TreeType *this,                            \
                                              DataType const *arr,                       \
                                              unsigned n) {                              \
    unsigned i;                                                                          \
    if (arr) {                                                                           \
        for (i = 0; i < n; ++i) {                                                        \
            if (!__avltree_insert_##id(this, arr[i], NULL)) return 0;                    \
        }                                                                                \
    }                                                                                    \
    return 1;                                                                            \
}                                                                                        \
                                                                                         \
unsigned char __avltree_insert_fromTree_##id(TreeType *this,                             \
                                             EntryType const *start,                     \
                                             EntryType const *end) {                     \
    if (start) {                                                                         \
        for (; start != end; start = __avl_inorder_successor_##id(start)) {              \
            customAssert(start)                                                          \
            if (!__avltree_insert_##id(this, start->data, NULL)) return 0;               \
        }                                                                                \
    }                                                                                    \
    return 1;                                                                            \
}                                                                                        \
                                                                                         \
TreeType *__avltree_new_fromArray_##id(DataType const *arr, unsigned n) {                \
    TreeType *t = calloc(1, sizeof(TreeType));                                           \
    customAssert(t)                                                                      \
    if (t) __avltree_insert_fromArray_##id(t, arr, n);                                   \
    return t;                                                                            \
}                                                                                        \
                                                                                         \
TreeType *__avltree_createCopy_##id(TreeType const *other) {                             \
    EntryType *e = __avl_successor_##id(other->root);                                    \
    TreeType *t = __avltree_new_fromArray_##id(NULL, 0);                                 \
    if (t) __avltree_insert_fromTree_##id(t, e, NULL);                                   \
    return t;                                                                            \
}                                                                                        \
                                                                                         \
void __avltree_remove_entry_##id(TreeType *this, EntryType *v) {                         \
    char deleteData = 1;                                                                 \
    EntryType *curr, *parent, *child;                                                    \
    if (!v) return;                                                                      \
                                                                                         \
    if (v->left && v->right) {                                                           \
        EntryType *temp = __avl_inorder_successor_##id(v);                               \
        customAssert(temp)                                                               \
        deleteData = 0;                                                                  \
        deleteKey(entry_get_key(v));                                                     \
        deleteValue(v->data.second);                                                     \
        v->data = temp->data;                                                            \
        v = temp;                                                                        \
    }                                                                                    \
                                                                                         \
    for (curr = v, parent = curr->parent;                                                \
            curr && parent;                                                              \
            curr = parent, parent = curr->parent) {                                      \
        if (curr == parent->left) {                                                      \
            if (parent->bf == -1) {                                                      \
                parent->bf = 0;                                                          \
            } else if (parent->bf == 0) {                                                \
                parent->bf = 1;                                                          \
                break;                                                                   \
            } else if (parent->bf == 1) {                                                \
                if (parent->right->bf == 1) {                                            \
                    parent = __avl_leftRotate_##id(this, parent);                        \
                    parent->bf = parent->left->bf = 0;                                   \
                } else if (parent->right->bf == 0) {                                     \
                    parent = __avl_leftRotate_##id(this, parent);                        \
                    parent->bf = -1;                                                     \
                    parent->left->bf = 1;                                                \
                } else if (parent->right->bf == -1) {                                    \
                    signed char old = parent->right->left->bf;                           \
                    __avl_rightRotate_##id(this, parent->right);                         \
                    parent = __avl_leftRotate_##id(this, parent);                        \
                    parent->bf = 0;                                                      \
                    if (old == -1) {                                                     \
                        parent->left->bf = 0;                                            \
                        parent->right->bf = 1;                                           \
                    } else if (old == 1) {                                               \
                        parent->left->bf = -1;                                           \
                        parent->right->bf = 0;                                           \
                    } else if (old == 0) {                                               \
                        parent->left->bf = parent->right->bf = 0;                        \
                    }                                                                    \
                }                                                                        \
                if (parent->bf == -1) break;                                             \
            }                                                                            \
        } else {                                                                         \
            if (parent->bf == 1) {                                                       \
                parent->bf = 0;                                                          \
            } else if (parent->bf == 0) {                                                \
                parent->bf = -1; break;                                                  \
            } else if (parent->bf == -1) {                                               \
                if (parent->left->bf == -1) {                                            \
                    parent = __avl_rightRotate_##id(this, parent);                       \
                    parent->bf = parent->right->bf = 0;                                  \
                } else if (parent->left->bf == 0) {                                      \
                    parent = __avl_rightRotate_##id(this, parent);                       \
                    parent->bf = 1;                                                      \
                    parent->right->bf = -1;                                              \
                } else if (parent->left->bf == 1) {                                      \
                    signed char old = parent->left->right->bf;                           \
                    __avl_leftRotate_##id(this, parent->left);                           \
                    parent = __avl_rightRotate_##id(this, parent);                       \
                    parent->bf = 0;                                                      \
                    if (old == -1) {                                                     \
                        parent->left->bf = 0;                                            \
                        parent->right->bf = 1;                                           \
                    } else if (old == 1) {                                               \
                        parent->left->bf = -1;                                           \
                        parent->right->bf = 0;                                           \
                    } else if (old == 0) {                                               \
                        parent->left->bf = parent->right->bf = 0;                        \
                    }                                                                    \
                }                                                                        \
                if (parent->bf == 1) break;                                              \
            }                                                                            \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    child = v->left ? v->left : v->right;                                                \
    if (child) child->parent = v->parent;                                                \
    if (v->parent) {                                                                     \
        if (v == v->parent->left) {                                                      \
            v->parent->left = child;                                                     \
        } else {                                                                         \
            v->parent->right = child;                                                    \
        }                                                                                \
    } else if (v == this->root && (!v->left || !v->right)) {                             \
        this->root = child;                                                              \
    }                                                                                    \
    if (deleteData) {                                                                    \
        deleteKey(entry_get_key(v));                                                     \
        deleteValue(v->data.second);                                                     \
    }                                                                                    \
    free(v);                                                                             \
    --this->size;                                                                        \
}                                                                                        \
                                                                                         \
void __avltree_erase_##id(TreeType *this,                                                \
                          EntryType *begin, EntryType const *end) {                      \
    char getNext = 1, eraseToEnd;                                                        \
    kt nextKey; kt lastKey = (kt) 0;                                                     \
    kt keys[256];                                                                        \
    if (!begin) return;                                                                  \
                                                                                         \
    eraseToEnd = !end;                                                                   \
    if (!eraseToEnd) lastKey = entry_get_key(end);                                       \
    nextKey = entry_get_key(begin);                                                      \
                                                                                         \
    while (getNext) {                                                                    \
        int i, count = 0;                                                                \
        begin = __avltree_find_key_##id(this, nextKey, 0);                               \
        for (; begin != end && count < 256;                                              \
                begin = __avl_inorder_successor_##id(begin)) {                           \
            customAssert(begin)                                                          \
            keys[count++] = entry_get_key(begin);                                        \
        }                                                                                \
                                                                                         \
        if (begin == end) getNext = 0;                                                   \
        else {                                                                           \
            customAssert(begin)                                                          \
            nextKey = entry_get_key(begin);                                              \
        }                                                                                \
                                                                                         \
        for (i = 0; i < count; ++i) {                                                    \
            EntryType *e = __avltree_find_key_##id(this, keys[i], 0);                    \
            __avltree_remove_entry_##id(this, e);                                        \
        }                                                                                \
                                                                                         \
        if (!eraseToEnd) {                                                               \
            end = __avltree_find_key_##id(this, lastKey, 0);                             \
        }                                                                                \
    }                                                                                    \
}                                                                                        \

#endif /* DS_AVL_TREE_H */
