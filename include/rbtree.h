#ifndef RBTREE_H
#define RBTREE_H

#include "iterator.h"

typedef enum {
    BLACK,
    RED
} __rb_node_color;

#define __rb_isOnLeft(n) ((n) == (n)->parent->left)

#define __rb_getSibling(n) (((n)->parent == NULL) ? NULL : (__rb_isOnLeft((n)) ? (n)->parent->right : (n)->parent->left))

#define __rb_moveDown(s, np) {                                                                               \
    if ((s)->parent) {                                                                                       \
        if (__rb_isOnLeft((s))) {                                                                            \
            (s)->parent->left = (np);                                                                        \
        } else {                                                                                             \
            (s)->parent->right = (np);                                                                       \
        }                                                                                                    \
    }                                                                                                        \
    (np)->parent = (s)->parent;                                                                              \
    (s)->parent = (np);                                                                                      \
}

#define __rb_swapColors(x1, x2) { int tmp_color = (x1)->color; (x1)->color = (x2)->color; (x2)->color = tmp_color; }

#define __rb_leftRotate(EntryType, t, x) {                                                                   \
    EntryType *nParent = (x)->right;                                                                         \
    if ((x) == (t)->root) {                                                                                  \
        (t)->root = nParent;                                                                                 \
    }                                                                                                        \
    __rb_moveDown((x), nParent)                                                                              \
    (x)->right = nParent->left;                                                                              \
    if (nParent->left) {                                                                                     \
        nParent->left->parent = (x);                                                                         \
    }                                                                                                        \
    nParent->left = (x);                                                                                     \
}

#define __rb_rightRotate(EntryType, t, x) {                                                                  \
    EntryType *nParent = (x)->left;                                                                          \
    if ((x) == (t)->root) {                                                                                  \
        (t)->root = nParent;                                                                                 \
    }                                                                                                        \
    __rb_moveDown((x), nParent)                                                                              \
    (x)->left = nParent->right;                                                                              \
    if (nParent->right) {                                                                                    \
        nParent->right->parent = (x);                                                                        \
    }                                                                                                        \
    nParent->right = (x);                                                                                    \
}

#define __rb_nextNode_body(x, dir)                                                                           \
    if (!(x)) return NULL;                                                                                   \
    while ((x)->dir) {                                                                                       \
        (x) = (x)->dir;                                                                                      \
    }                                                                                                        \
    return x;                                                                                                \

#define __rb_nextNode_inorder_body(EntryType, x, dir, nextNode)                                              \
    if (!(x)) return NULL;                                                                                   \
    else if ((x)->dir) return nextNode((x)->dir);                                                            \
                                                                                                             \
    EntryType *parent = (x)->parent;                                                                         \
    while (parent && (x) == parent->dir) {                                                                   \
        (x) = parent;                                                                                        \
        parent = parent->parent;                                                                             \
    }                                                                                                        \
    return parent;                                                                                           \

#define __gen_rbtree(id, kt, cmp_lt, TreeType, DataType, EntryType, entry_get_key, data_get_key, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
typedef struct EntryType EntryType;                                                                          \
struct EntryType {                                                                                           \
    EntryType *parent;                                                                                       \
    EntryType *left;                                                                                         \
    EntryType *right;                                                                                        \
    __rb_node_color color;                                                                                   \
    DataType data;                                                                                           \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    EntryType *root;                                                                                         \
    size_t size;                                                                                             \
} TreeType;                                                                                                  \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__rb_successor_##id(EntryType *x) {                                          \
    __rb_nextNode_body(x, left)                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__rb_predecessor_##id(EntryType *x) {                                        \
    __rb_nextNode_body(x, right)                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__rb_inorder_successor_##id(EntryType *x) {                                  \
    __rb_nextNode_inorder_body(EntryType, x, right, __rb_successor_##id)                                     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL EntryType *__rb_inorder_predecessor_##id(EntryType *x) {                                \
    __rb_nextNode_inorder_body(EntryType, x, left, __rb_predecessor_##id)                                    \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_insert_fromArray_##id(TreeType *this, DataType *arr, size_t n);               \
__DS_FUNC_PREFIX void __rbtree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end);       \
__DS_FUNC_PREFIX void __rbtree_clear_##id(TreeType *this);                                                   \
__DS_FUNC_PREFIX void __rbtree_remove_entry_##id(TreeType *this, EntryType *v);                              \
__DS_FUNC_PREFIX void __rbtree_fixDoubleBlack_##id(TreeType *this, EntryType *x);                            \
__DS_FUNC_PREFIX void __rbtree_fixRedRed_##id(TreeType *this, EntryType *x);                                 \
                                                                                                             \
create_iterator_distance_helper(TREE, id, EntryType *)                                                       \
                                                                                                             \
__DS_FUNC_PREFIX TreeType *__rbtree_new_fromArray_##id(DataType *arr, size_t n) {                            \
    TreeType *t = __ds_calloc(1, sizeof(TreeType));                                                          \
    __rbtree_insert_fromArray_##id(t, arr, n);                                                               \
    return t;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX TreeType *__rbtree_createCopy_##id(TreeType *other) {                                       \
    TreeType *t = __ds_calloc(1, sizeof(TreeType));                                                          \
    __rbtree_insert_fromTree_##id(t, __rb_successor_##id(other->root), NULL);                                \
    return t;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __rbtree_free_##id(TreeType *this) {                                               \
    __rbtree_clear_##id(this);                                                                               \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __rbtree_clear_##id(TreeType *this) {                                              \
    EntryType *curr = this->root;                                                                            \
    while (curr) {                                                                                           \
        __rbtree_remove_entry_##id(this, curr);                                                              \
        curr = this->root;                                                                                   \
    }                                                                                                        \
    this->root = NULL;                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX EntryType *__rbtree_find_key_##id(TreeType *this, const kt key, bool candidate) {           \
    EntryType *temp = this->root;                                                                            \
    while (temp) {                                                                                           \
        if (cmp_lt(key, entry_get_key(temp))) {                                                              \
            if (!temp->left) {                                                                               \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                temp = temp->left;                                                                           \
            }                                                                                                \
        } else if (cmp_lt(entry_get_key(temp), key)) {                                                       \
            if (!temp->right) {                                                                              \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                temp = temp->right;                                                                          \
            }                                                                                                \
        } else {                                                                                             \
            break;                                                                                           \
        }                                                                                                    \
    }                                                                                                        \
    return temp;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX EntryType *__rbtree_insert_##id(TreeType *this, DataType data, int *inserted) {             \
    EntryType *newNode = __ds_calloc(1, sizeof(EntryType));                                                  \
    newNode->color = RED;                                                                                    \
    if (!this->root) {                                                                                       \
        newNode->color = BLACK;                                                                              \
        copyKey(entry_get_key(newNode), data_get_key(data));                                                 \
        copyValue(newNode->data.second, data.second);                                                        \
        this->root = newNode;                                                                                \
    } else {                                                                                                 \
        EntryType *temp = __rbtree_find_key_##id(this, data_get_key(data), true);                            \
        if (ds_cmp_eq(cmp_lt, entry_get_key(temp), data_get_key(data))) {                                    \
            free(newNode);                                                                                   \
            deleteValue(temp->data.second);                                                                  \
            copyValue(temp->data.second, data.second);                                                       \
            if (inserted) *inserted = 0;                                                                     \
            return temp;                                                                                     \
        }                                                                                                    \
        copyKey(entry_get_key(newNode), data_get_key(data));                                                 \
        copyValue(newNode->data.second, data.second);                                                        \
        newNode->parent = temp;                                                                              \
                                                                                                             \
        if (cmp_lt(data_get_key(data), entry_get_key(temp))) {                                               \
            temp->left = newNode;                                                                            \
        } else {                                                                                             \
            temp->right = newNode;                                                                           \
        }                                                                                                    \
        __rbtree_fixRedRed_##id(this, newNode);                                                              \
    }                                                                                                        \
    if (inserted) *inserted = 1;                                                                             \
    this->size++;                                                                                            \
    return newNode;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_insert_fromArray_##id(TreeType *this, DataType *arr, size_t n) {              \
    if (!(arr && n)) return;                                                                                 \
    for (size_t i = 0; i < n; ++i) {                                                                         \
        __rbtree_insert_##id(this, arr[i], NULL);                                                            \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_insert_fromTree_##id(TreeType *this, EntryType *start, EntryType *end) {      \
    while (start != end) {                                                                                   \
        __rbtree_insert_##id(this, start->data, NULL);                                                       \
        start = __rb_inorder_successor_##id(start);                                                          \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_remove_entry_##id(TreeType *this, EntryType *v) {                             \
    EntryType *u = NULL;                                                                                     \
    if (v->left && v->right) u = __rb_successor_##id(v->right);                                              \
    else if (v->left) u = v->left;                                                                           \
    else if (v->right) u = v->right;                                                                         \
                                                                                                             \
    bool uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));                                       \
    EntryType *parent = v->parent;                                                                           \
    if (!u) {                                                                                                \
        if (v == this->root) {                                                                               \
            this->root = NULL;                                                                               \
        } else {                                                                                             \
            if (uvBlack) {                                                                                   \
                __rbtree_fixDoubleBlack_##id(this, v);                                                       \
            } else {                                                                                         \
                EntryType *sibling = __rb_getSibling(v);                                                     \
                if (sibling != NULL) {                                                                       \
                    sibling->color = RED;                                                                    \
                }                                                                                            \
            }                                                                                                \
                                                                                                             \
           if (__rb_isOnLeft(v)) {                                                                           \
               parent->left = NULL;                                                                          \
            } else {                                                                                         \
                parent->right = NULL;                                                                        \
            }                                                                                                \
        }                                                                                                    \
        deleteKey(entry_get_key(v));                                                                         \
        deleteValue(v->data.second);                                                                         \
        free(v);                                                                                             \
        this->size--;                                                                                        \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    if (!v->left || !v->right) {                                                                             \
        if (v == this->root) {                                                                               \
            deleteKey(entry_get_key(v));                                                                     \
            deleteValue(v->data.second);                                                                     \
            v->data = u->data;                                                                               \
            free(u);                                                                                         \
            v->left = v->right = NULL;                                                                       \
            this->size--;                                                                                    \
        } else {                                                                                             \
            if (__rb_isOnLeft(v)) {                                                                          \
                parent->left = u;                                                                            \
            } else {                                                                                         \
                parent->right = u;                                                                           \
            }                                                                                                \
                                                                                                             \
            deleteKey(entry_get_key(v));                                                                     \
            deleteValue(v->data.second);                                                                     \
            free(v);                                                                                         \
            this->size--;                                                                                    \
                                                                                                             \
            u->parent = parent;                                                                              \
            if (uvBlack) {                                                                                   \
                __rbtree_fixDoubleBlack_##id(this, u);                                                       \
            } else {                                                                                         \
                u->color = BLACK;                                                                            \
            }                                                                                                \
        }                                                                                                    \
        return;                                                                                              \
    }                                                                                                        \
    DataType tmp = u->data;                                                                                  \
    u->data = v->data;                                                                                       \
    v->data = tmp;                                                                                           \
    __rbtree_remove_entry_##id(this, u);                                                                     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_remove_key_##id(TreeType *this, const kt key) {                               \
    if (!this->root) return;                                                                                 \
    EntryType *v = __rbtree_find_key_##id(this, key, true);                                                  \
    if (ds_cmp_neq(cmp_lt, entry_get_key(v), key)) return;                                                   \
    __rbtree_remove_entry_##id(this, v);                                                                     \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_erase_##id(TreeType *this, EntryType *begin, EntryType *end) {                \
    if (!begin) return;                                                                                      \
                                                                                                             \
    /* store keys in an array since RB tree deletions involve swapping values
     * and thus it's not reliable to use node pointers in a bulk delete operation */                         \
    kt keys[this->size];                                                                                     \
    int count = 0;                                                                                           \
    for (EntryType *curr = begin; curr != end; curr = __rb_inorder_successor_##id(curr)) {                   \
        keys[count++] = entry_get_key(curr);                                                                 \
    }                                                                                                        \
                                                                                                             \
    for (int i = 0; i < count; ++i) {                                                                        \
        __rbtree_remove_key_##id(this, keys[i]);                                                             \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_fixDoubleBlack_##id(TreeType *this, EntryType *x) {                           \
    if (x == this->root) return;                                                                             \
                                                                                                             \
    EntryType *sibling = __rb_getSibling(x), *parent = x->parent;                                            \
    if (!sibling) {                                                                                          \
        __rbtree_fixDoubleBlack_##id(this, parent);                                                          \
    } else {                                                                                                 \
        if (sibling->color == RED) {                                                                         \
            parent->color = RED;                                                                             \
            sibling->color = BLACK;                                                                          \
                                                                                                             \
            if (__rb_isOnLeft(sibling)) {                                                                    \
                __rb_rightRotate(EntryType, this, parent)                                                    \
            } else {                                                                                         \
                __rb_leftRotate(EntryType, this, parent)                                                     \
            }                                                                                                \
            __rbtree_fixDoubleBlack_##id(this, x);                                                           \
        } else {                                                                                             \
            if ((sibling->left && sibling->left->color == RED) || (sibling->right && sibling->right->color == RED)) { \
                if (sibling->left && sibling->left->color == RED) {                                          \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->left->color = sibling->color;                                               \
                        sibling->color = parent->color;                                                      \
                        __rb_rightRotate(EntryType, this, parent)                                            \
                    } else {                                                                                 \
                        sibling->left->color = parent->color;                                                \
                        __rb_rightRotate(EntryType, this, sibling)                                           \
                        __rb_leftRotate(EntryType, this, parent)                                             \
                    }                                                                                        \
                } else {                                                                                     \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->right->color = parent->color;                                               \
                        __rb_leftRotate(EntryType, this, sibling)                                            \
                        __rb_rightRotate(EntryType, this, parent)                                            \
                    } else {                                                                                 \
                        sibling->right->color = sibling->color;                                              \
                        sibling->color = parent->color;                                                      \
                        __rb_leftRotate(EntryType, this, parent)                                             \
                    }                                                                                        \
                }                                                                                            \
                                                                                                             \
                parent->color = BLACK;                                                                       \
            } else {                                                                                         \
                sibling->color = RED;                                                                        \
                                                                                                             \
                if (parent->color == BLACK) {                                                                \
                    __rbtree_fixDoubleBlack_##id(this, parent);                                              \
                } else {                                                                                     \
                    parent->color = BLACK;                                                                   \
                }                                                                                            \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rbtree_fixRedRed_##id(TreeType *this, EntryType *x) {                                \
    if (x == this->root) {                                                                                   \
        x->color = BLACK;                                                                                    \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    EntryType *parent = x->parent, *grandparent = parent->parent, *uncle = NULL;                             \
    if (parent && grandparent) {                                                                             \
        uncle = __rb_isOnLeft(parent) ? grandparent->right : grandparent->left;                              \
    }                                                                                                        \
                                                                                                             \
    if (parent->color != BLACK) {                                                                            \
        if (uncle && uncle->color == RED) {                                                                  \
            parent->color = BLACK;                                                                           \
            uncle->color = BLACK;                                                                            \
            grandparent->color = RED;                                                                        \
            __rbtree_fixRedRed_##id(this, grandparent);                                                      \
        } else {                                                                                             \
            if (__rb_isOnLeft(parent)) {                                                                     \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_swapColors(parent, grandparent)                                                     \
                } else {                                                                                     \
                    __rb_leftRotate(EntryType, this, parent)                                                 \
                    __rb_swapColors(x, grandparent)                                                          \
                }                                                                                            \
                __rb_rightRotate(EntryType, this, grandparent)                                               \
            } else {                                                                                         \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_rightRotate(EntryType, this, parent)                                                \
                    __rb_swapColors(x, grandparent)                                                          \
                } else {                                                                                     \
                    __rb_swapColors(parent, grandparent)                                                     \
                }                                                                                            \
                __rb_leftRotate(EntryType, this, grandparent)                                                \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \

#endif /* RBTREE_H */
