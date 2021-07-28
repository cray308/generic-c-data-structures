#ifndef DS_AVL_TREE_H
#define DS_AVL_TREE_H

#include "iterator.h"

#define __avl_nextNode_body(x, dir)                                                                          \
    if (!(x)) return NULL;                                                                                   \
    while ((x)->dir) {                                                                                       \
        (x) = (x)->dir;                                                                                      \
    }                                                                                                        \
    return x;                                                                                                \

#define __avl_nextNode_inorder_body(EntryType, x, dir, nextNode)                                             \
    EntryType *parent;                                                                                       \
    if (!(x)) return NULL;                                                                                   \
    else if ((x)->dir) return nextNode((x)->dir);                                                            \
                                                                                                             \
    parent = (x)->parent;                                                                                    \
    while (parent && (x) == parent->dir) {                                                                   \
        (x) = parent;                                                                                        \
        parent = parent->parent;                                                                             \
    }                                                                                                        \
    return parent;                                                                                           \

#define __avl_tree_x_rotate_body(EntryType, t, x, dir, rev)                                                  \
    EntryType *nParent = (x)->dir;                                                                           \
    if ((x) == (t)->root) {                                                                                  \
        (t)->root = nParent;                                                                                 \
    }                                                                                                        \
    (x)->dir = nParent->rev;                                                                                 \
    if ((x)->dir) {                                                                                          \
        (x)->dir->parent = x;                                                                                \
    }                                                                                                        \
    nParent->parent = (x)->parent;                                                                           \
    if ((x)->parent) {                                                                                       \
        if ((x) == (x)->parent->left) {                                                                      \
            (x)->parent->left = nParent;                                                                     \
        } else {                                                                                             \
            (x)->parent->right = nParent;                                                                    \
        }                                                                                                    \
    }                                                                                                        \
    nParent->rev = x;                                                                                        \
    (x)->parent = nParent;                                                                                   \
    return nParent;                                                                                          \

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
__DS_FUNC_PREFIX_INL EntryType *__avl_successor_##id(EntryType *x) {                                         \
    __avl_nextNode_body(x, left)                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__avl_predecessor_##id(EntryType *x) {                                       \
    __avl_nextNode_body(x, right)                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__avl_inorder_successor_##id(EntryType *x) {                                 \
    __avl_nextNode_inorder_body(EntryType, x, right, __avl_successor_##id)                                   \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__avl_inorder_predecessor_##id(EntryType *x) {                               \
    __avl_nextNode_inorder_body(EntryType, x, left, __avl_predecessor_##id)                                  \
}                                                                                                            \
                                                                                                             \
create_iterator_distance_helper(AVLTREE, id, EntryType *)                                                    \
                                                                                                             \
EntryType *__avltree_find_key_##id(TreeType *this, const kt key, unsigned char candidate);                   \
EntryType *__avltree_insert_##id(TreeType *this, DataType data, int *inserted);                              \
void __avltree_insert_fromArray_##id(TreeType *this, DataType *arr, unsigned n);                             \
void __avltree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end);                       \
TreeType *__avltree_new_fromArray_##id(DataType *arr, unsigned n);                                           \
TreeType *__avltree_createCopy_##id(TreeType *other);                                                        \
void __avltree_remove_entry_##id(TreeType *this, EntryType *v);                                              \
void __avltree_erase_##id(TreeType *this, EntryType *begin, EntryType *end);                                 \

#define __setup_avltree_source(id, kt, TreeType, DataType, EntryType, cmp_lt, entry_get_key, data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
EntryType *__avl_leftRotate_##id(TreeType *this, EntryType *x) {                                             \
    __avl_tree_x_rotate_body(EntryType, this, x, right, left)                                                \
}                                                                                                            \
                                                                                                             \
EntryType *__avl_rightRotate_##id(TreeType *this, EntryType *x) {                                            \
    __avl_tree_x_rotate_body(EntryType, this, x, left, right)                                                \
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
    EntryType *curr = __avltree_find_key_##id(this, data_get_key(data), 1), *new, *parent;                   \
    if (curr && ds_cmp_eq(cmp_lt, entry_get_key(curr), data_get_key(data))) {                                \
        deleteValue(curr->data.second);                                                                      \
        copyValue(curr->data.second, data.second);                                                           \
        if (inserted) *inserted = 0;                                                                         \
        return curr;                                                                                         \
    }                                                                                                        \
    __ds_calloc(new, 1, sizeof(EntryType))                                                                   \
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
    while (curr && parent) {                                                                                 \
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
        curr = parent;                                                                                       \
        parent = curr->parent;                                                                               \
    }                                                                                                        \
    ++this->size;                                                                                            \
    if (inserted) *inserted = 1;                                                                             \
    return new;                                                                                              \
}                                                                                                            \
                                                                                                             \
void __avltree_insert_fromArray_##id(TreeType *this, DataType *arr, unsigned n) {                            \
    unsigned i;                                                                                              \
    if (!(arr && n)) return;                                                                                 \
    for (i = 0; i < n; ++i) {                                                                                \
        __avltree_insert_##id(this, arr[i], NULL);                                                           \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
void __avltree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end) {                      \
    if (!start) return;                                                                                      \
    while (start != end) {                                                                                   \
        __avltree_insert_##id(this, start->data, NULL);                                                      \
        start = __avl_inorder_successor_##id(start);                                                         \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
TreeType *__avltree_new_fromArray_##id(DataType *arr, unsigned n) {                                          \
    TreeType *t;                                                                                             \
    __ds_calloc(t, 1, sizeof(TreeType))                                                                      \
    __avltree_insert_fromArray_##id(t, arr, n);                                                              \
    return t;                                                                                                \
}                                                                                                            \
                                                                                                             \
TreeType *__avltree_createCopy_##id(TreeType *other) {                                                       \
    TreeType *t = __avltree_new_fromArray_##id(NULL, 0);                                                     \
    __avltree_insert_fromTree_##id(t, __avl_successor_##id(other->root), NULL);                              \
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
    EntryType *curr;                                                                                         \
    kt* keys;                                                                                                \
    int i = 0, count = 0;                                                                                    \
    if (!begin) return;                                                                                      \
                                                                                                             \
    /* store keys in an array since tree deletions involve swapping values
     * and thus it's not reliable to use node pointers in a bulk delete operation */                         \
    __ds_malloc(keys, this->size * sizeof(kt))                                                               \
    for (curr = begin; curr != end; curr = __avl_inorder_successor_##id(curr)) {                             \
        keys[count++] = entry_get_key(curr);                                                                 \
    }                                                                                                        \
    for (; i < count; ++i) {                                                                                 \
        __avltree_remove_entry_##id(this, __avltree_find_key_##id(this, keys[i], 0));                        \
    }                                                                                                        \
    free(keys);                                                                                              \
}                                                                                                            \

#endif /* DS_AVL_TREE_H */
