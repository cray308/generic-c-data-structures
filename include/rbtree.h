#ifndef RBTREE_H
#define RBTREE_H

#include "iterator.h"

typedef enum {
    BLACK,
    RED
} __rb_node_color;

#define __rb_node_swap(t, x, y)                                                                              \
do {                                                                                                         \
    t _swap_tmp = (x)->data;                                                                                 \
    (x)->data = (y)->data;                                                                                   \
    (y)->data = _swap_tmp;                                                                                   \
} while(0)

#define __rb_isOnLeft(n) ((n) == (n)->parent->left)

#define __rb_hasRedChild(n)                                                                                  \
    (((n)->left != NULL && (n)->left->color == RED) || ((n)->right != NULL && (n)->right->color == RED))

#define __rb_get_uncle(n)                                                                                    \
    (((n)->parent == NULL || (n)->parent->parent == NULL) ? NULL :                                           \
    (__rb_isOnLeft((n)->parent) ? (n)->parent->parent->right : (n)->parent->parent->left))

#define __rb_getSibling(n)                                                                                   \
    (((n)->parent == NULL) ? NULL : (__rb_isOnLeft((n)) ? (n)->parent->right : (n)->parent->left))

#define __rb_moveDown(s, np)                                                                                 \
    do {                                                                                                     \
        if ((s)->parent != NULL) {                                                                           \
            if (__rb_isOnLeft((s))) {                                                                        \
                (s)->parent->left = (np);                                                                    \
            } else {                                                                                         \
                (s)->parent->right = (np);                                                                   \
            }                                                                                                \
        }                                                                                                    \
        (np)->parent = (s)->parent;                                                                          \
        (s)->parent = (np);                                                                                  \
    } while (0)

#define __rb_BSTreplace(id, x)                                                                               \
    (((x)->left && (x)->right) ? (__rb_successor_##id((x)->right)) :                                         \
    ((!((x)->left) && !((x)->right)) ? (NULL) : (((x)->left != NULL) ? ((x)->left) : ((x)->right))))

#define __rb_swapColors(x1, x2)                                                                              \
    do {                                                                                                     \
        int _tmp_color;                                                                                      \
        _tmp_color = (x1)->color;                                                                            \
        (x1)->color = (x2)->color;                                                                           \
        (x2)->color = _tmp_color;                                                                            \
    } while(0)

#define __rb_leftRotate(id, t, x)                                                                            \
    do {                                                                                                     \
        RBNode_##id *_nParent;                                                                               \
        _nParent = (x)->right;                                                                               \
        if ((x) == (t)->root) {                                                                              \
            (t)->root = _nParent;                                                                            \
        }                                                                                                    \
        __rb_moveDown((x), _nParent);                                                                        \
        (x)->right = _nParent->left;                                                                         \
        if (_nParent->left) {                                                                                \
            _nParent->left->parent = (x);                                                                    \
        }                                                                                                    \
        _nParent->left = (x);                                                                                \
    } while (0)

#define __rb_rightRotate(id, t, x)                                                                           \
    do {                                                                                                     \
        RBNode_##id *_nParent = (x)->left;                                                                   \
        if ((x) == (t)->root) {                                                                              \
            (t)->root = _nParent;                                                                            \
        }                                                                                                    \
        __rb_moveDown((x), _nParent);                                                                        \
        (x)->left = _nParent->right;                                                                         \
        if (_nParent->right) {                                                                               \
            _nParent->right->parent = (x);                                                                   \
        }                                                                                                    \
        _nParent->right = (x);                                                                               \
    } while (0)


/**
 * Macro to iterate through the tree using an in-order traversal.
 *
 * @param   id    ID used with gen_rbtree.
 * @param   t     Pointer to tree.
 * @param   ptr   TreeIterator which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(TREE, ptr) or ptr->data.
 */
#define tree_iter(id, t, ptr)                                                                                \
    for (ptr = iter_begin(TREE, id, t, 0); ptr != iter_end(TREE, id, t, 0); iter_next(TREE, id, ptr))


/**
 * Macro to iterate through the tree in reverse (largest element to smallest).
 *
 * @param   id    ID used with gen_rbtree.
 * @param   t     Pointer to tree.
 * @param   ptr   TreeIterator which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(TREE, ptr) or ptr->data.
 */
#define tree_riter(id, t, ptr)                                                                               \
    for (ptr = iter_rbegin(TREE, id, t, 0); ptr != iter_rend(TREE, id, t, 0); iter_prev(TREE, id, ptr))      


/**
 * Creates a new RB-Tree.
 *
 * @param   id  ID used with gen_rbtree.
 *
 * @return          Pointer to the newly created tree.
 */
#define tree_new(id) tree_new_##id()


/**
 * Deletes all elements and frees the tree.
 *
 * @param  id  ID used with gen_rbtree.
 * @param  t   Pointer to tree.
 */
#define tree_free(id, t) tree_free_##id(t)


/**
 * Removes all nodes from the tree, leaving it with the root node set to NULL.
 *
 * @param  id    ID used with gen_rbtree.
 * @param  t     Pointer to tree.
 */
#define tree_clear(id, t) tree_clear_##id(t)


/**
 * Searches for the provided value.
 *
 * @param   id   ID used with gen_rbtree.
 * @param   t    Pointer to tree.
 * @param   val  Value to search for.
 *
 * @return       RBNode of the element if it was found, or NULL if it was not found.
 */
#define tree_find(id, t, val) __rb_tree_search_##id(t, val, false)


/**
 * Inserts a new node into the tree.
 *
 * @param  id   ID used with gen_rbtree.
 * @param  t    Pointer to tree.
 * @param  val  Value to be inserted.
 */
#define tree_insert(id, t, val) tree_insert_##id(t, val)


/**
 * Deletes the provided RBNode from the tree.
 *
 * @param  id  ID used with gen_rbtree.
 * @param  t   Pointer to tree.
 * @param  z   Node to be deleted.
 */
#define tree_delete_node(id, t, z) tree_delete_node_##id(t, z)


/**
 * Deletes a node from the tree with a value matching that which was provided.
 *
 * @param  id   ID used with gen_rbtree.
 * @param  t    Pointer to tree.
 * @param  val  Pointer to the value to compare to when searching the tree.
 */
#define tree_delete_by_val(id, t, val) tree_delete_by_val_##id(t, val)



/**
 * Generates RB-tree code for a specified type and ID.
 *
 * @param   id      ID to be used for the Tree struct and function names (must be unique).
 * @param   t       Type to be stored in the given tree node.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_rbtree(id, t, cmp_lt)                                                                            \
__gen_rb_node_tree(id, t)                                                                                    \
__gen_rb_successor_declarations(id)                                                                          \
__gen_iter_TREE(id)                                                                                          \
__gen_rb_helper_funcs(id, t)                                                                                 \
                                                                                                             \
__DS_FUNC_PREFIX Tree_##id *tree_new_##id(void) {                                                            \
    Tree_##id *this = malloc(sizeof(Tree_##id));                                                             \
    if (!this) {                                                                                             \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    this->root = NULL;                                                                                       \
    this->size = 0;                                                                                          \
    return this;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void tree_free_##id(Tree_##id *this) {                                                  \
    tree_clear_##id(this);                                                                                   \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void tree_clear_##id(Tree_##id *this) {                                                 \
    RBNode_##id *curr = this->root;                                                                          \
    while (curr) {                                                                                           \
        tree_delete_node_##id(this, curr);                                                                   \
        curr = this->root;                                                                                   \
    }                                                                                                        \
    this->root = NULL;                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX RBNode_##id *__rb_tree_search_##id(Tree_##id *this, t val, bool candidate) {                \
    RBNode_##id *temp = this->root;                                                                          \
    while (temp) {                                                                                           \
        if (cmp_lt(val, temp->data)) {                                                                       \
            if (!temp->left) {                                                                               \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                temp = temp->left;                                                                           \
            }                                                                                                \
        } else if (cmp_lt(temp->data, val)) {                                                                \
            if (!temp->right) {                                                                              \
                if (candidate) break;                                                                        \
                else return NULL;                                                                            \
            } else {                                                                                         \
                temp = temp->right;                                                                          \
            }                                                                                                \
        } else {                                                                                             \
            break;                                                                                           \
        }                                                                                                    \
}                                                                                                            \
    return temp;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_insert_##id(Tree_##id *this, t val) {                                             \
    RBNode_##id *newNode = rb_node_new_##id();                                                               \
    if (!this->root) {                                                                                       \
        newNode->data = val;                                                                                 \
        newNode->color = BLACK;                                                                              \
        this->root = newNode;                                                                                \
    } else {                                                                                                 \
        RBNode_##id *temp = __rb_tree_search_##id(this, val, true);                                          \
        if (ds_cmp_eq(cmp_lt, temp->data, val)) {                                                            \
            free(newNode);                                                                                   \
            return;                                                                                          \
        }                                                                                                    \
        newNode->data = val;                                                                                 \
        newNode->parent = temp;                                                                              \
                                                                                                             \
        if (cmp_lt(val, temp->data)) {                                                                       \
            temp->left = newNode;                                                                            \
        } else {                                                                                             \
            temp->right = newNode;                                                                           \
        }                                                                                                    \
        __rb_fixRedRed_##id(this, newNode);                                                                  \
    }                                                                                                        \
    this->size++;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_delete_node_##id(Tree_##id *this, RBNode_##id *v) {                               \
    RBNode_##id *u = __rb_BSTreplace(id, v);                                                                 \
                                                                                                             \
    bool uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));                                       \
    RBNode_##id *parent = v->parent;                                                                         \
    if (!u) {                                                                                                \
        if (v == this->root) {                                                                               \
            this->root = NULL;                                                                               \
        } else {                                                                                             \
            if (uvBlack) {                                                                                   \
                __rb_fixDoubleBlack_##id(this, v);                                                           \
            } else {                                                                                         \
                RBNode_##id *sibling = __rb_getSibling(v);                                                   \
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
        free(v);                                                                                             \
        this->size--;                                                                                        \
    return;                                                                                                  \
    }                                                                                                        \
                                                                                                             \
    if (!v->left || !v->right) {                                                                             \
        if (v == this->root) {                                                                               \
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
            free(v);                                                                                         \
            this->size--;                                                                                    \
                                                                                                             \
            u->parent = parent;                                                                              \
            if (uvBlack) {                                                                                   \
                __rb_fixDoubleBlack_##id(this, u);                                                           \
            } else {                                                                                         \
                u->color = BLACK;                                                                            \
            }                                                                                                \
        }                                                                                                    \
        return;                                                                                              \
    }                                                                                                        \
    __rb_node_swap(t, u, v);                                                                                 \
    tree_delete_node_##id(this, u);                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_delete_by_val_##id(Tree_##id *this, t val) {                                      \
    if (!this->root || !val) {                                                                               \
        return;                                                                                              \
    }                                                                                                        \
    RBNode_##id *v = __rb_tree_search_##id(this, val, true);                                                 \
    if (ds_cmp_neq(cmp_lt, v->data, val)) {                                                                  \
        return;                                                                                              \
    }                                                                                                        \
    tree_delete_node_##id(this, v);                                                                          \
}                                                                                                            \


#define __gen_rb_node_tree(id, t)                                                                            \
typedef struct RBNode_##id RBNode_##id;                                                                      \
struct RBNode_##id {                                                                                         \
    RBNode_##id *parent;                                                                                     \
    RBNode_##id *left;                                                                                       \
    RBNode_##id *right;                                                                                      \
    __rb_node_color color;                                                                                   \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
__DS_FUNC_PREFIX RBNode_##id *rb_node_new_##id(void) {                                                       \
    RBNode_##id *node = malloc(sizeof(RBNode_##id));                                                         \
    if (!node) {                                                                                             \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(node, 0, sizeof(RBNode_##id));                                                                    \
    node->color = RED;                                                                                       \
    return node;                                                                                             \
}                                                                                                            \
                                                                                                             \
typedef struct {                                                                                             \
    RBNode_##id *root;                                                                                       \
    size_t size;                                                                                             \
} Tree_##id;                                                                                                 \


#define __gen_rb_successor_declarations(id)                                                                  \
__DS_FUNC_PREFIX_INL RBNode_##id *__rb_successor_##id(RBNode_##id *x) {                                      \
    if (!x) {                                                                                                \
        return x;                                                                                            \
    }                                                                                                        \
    while (x->left) {                                                                                        \
        x = x->left;                                                                                         \
    }                                                                                                        \
    return x;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL RBNode_##id *__rb_predecessor_##id(RBNode_##id *x) {                                    \
    if (!x) {                                                                                                \
        return x;                                                                                            \
    }                                                                                                        \
    while (x->right) {                                                                                       \
        x = x->right;                                                                                        \
    }                                                                                                        \
    return x;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL RBNode_##id *__rb_inorder_successor_##id(RBNode_##id *x) {                              \
    if (!x) {                                                                                                \
        return NULL;                                                                                         \
    } else if (x->right) {                                                                                   \
        return __rb_successor_##id(x->right);                                                                \
    }                                                                                                        \
                                                                                                             \
    RBNode_##id *parent = x->parent;                                                                         \
    while (parent && x == parent->right) {                                                                   \
        x = parent;                                                                                          \
        parent = parent->parent;                                                                             \
    }                                                                                                        \
    return parent;                                                                                           \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL RBNode_##id *__rb_inorder_predecessor_##id(RBNode_##id *x) {                            \
    if (!x) {                                                                                                \
        return NULL;                                                                                         \
    } else if (x->left) {                                                                                    \
        return __rb_predecessor_##id(x->left);                                                               \
    }                                                                                                        \
                                                                                                             \
    RBNode_##id *parent = x->parent;                                                                         \
    while (parent && x == parent->left) {                                                                    \
        x = parent;                                                                                          \
        parent = parent->parent;                                                                             \
    }                                                                                                        \
    return parent;                                                                                           \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_clear_##id(Tree_##id *this);                                                      \
__DS_FUNC_PREFIX void tree_delete_node_##id(Tree_##id *this, RBNode_##id *v);                                \
__DS_FUNC_PREFIX void __rb_fixDoubleBlack_##id(Tree_##id *this, RBNode_##id *x);                             \
__DS_FUNC_PREFIX void __rb_fixRedRed_##id(Tree_##id *this, RBNode_##id *x);                                  \


#define __gen_rb_helper_funcs(id, t)                                                                         \
__DS_FUNC_PREFIX void __rb_fixDoubleBlack_##id(Tree_##id *this, RBNode_##id *x) {                            \
    if (x == this->root) {                                                                                   \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    RBNode_##id *sibling = __rb_getSibling(x), *parent = x->parent;                                          \
    if (!sibling) {                                                                                          \
        __rb_fixDoubleBlack_##id(this, parent);                                                              \
    } else {                                                                                                 \
        if (sibling->color == RED) {                                                                         \
            parent->color = RED;                                                                             \
            sibling->color = BLACK;                                                                          \
                                                                                                             \
            if (__rb_isOnLeft(sibling)) {                                                                    \
                __rb_rightRotate(id, this, parent);                                                          \
            } else {                                                                                         \
                __rb_leftRotate(id, this, parent);                                                           \
            }                                                                                                \
            __rb_fixDoubleBlack_##id(this, x);                                                               \
        } else {                                                                                             \
            if (__rb_hasRedChild(sibling)) {                                                                 \
                if (sibling->left && sibling->left->color == RED) {                                          \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->left->color = sibling->color;                                               \
                        sibling->color = parent->color;                                                      \
                        __rb_rightRotate(id, this, parent);                                                  \
                    } else {                                                                                 \
                        sibling->left->color = parent->color;                                                \
                        __rb_rightRotate(id, this, sibling);                                                 \
                        __rb_leftRotate(id, this, parent);                                                   \
                    }                                                                                        \
                } else {                                                                                     \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->right->color = parent->color;                                               \
                        __rb_leftRotate(id, this, sibling);                                                  \
                        __rb_rightRotate(id, this, parent);                                                  \
                    } else {                                                                                 \
                        sibling->right->color = sibling->color;                                              \
                        sibling->color = parent->color;                                                      \
                        __rb_leftRotate(id, this, parent);                                                   \
                    }                                                                                        \
                }                                                                                            \
                                                                                                             \
                parent->color = BLACK;                                                                       \
            } else {                                                                                         \
                sibling->color = RED;                                                                        \
                                                                                                             \
                if (parent->color == BLACK) {                                                                \
                    __rb_fixDoubleBlack_##id(this, parent);                                                  \
                } else {                                                                                     \
                    parent->color = BLACK;                                                                   \
                }                                                                                            \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rb_fixRedRed_##id(Tree_##id *this, RBNode_##id *x) {                                 \
    if (x == this->root) {                                                                                   \
        x->color = BLACK;                                                                                    \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    RBNode_##id *parent = x->parent, *grandparent = parent->parent, *uncle = __rb_get_uncle(x);              \
    if (parent->color != BLACK) {                                                                            \
        if (uncle && uncle->color == RED) {                                                                  \
            parent->color = BLACK;                                                                           \
            uncle->color = BLACK;                                                                            \
            grandparent->color = RED;                                                                        \
            __rb_fixRedRed_##id(this, grandparent);                                                          \
        } else {                                                                                             \
            if (__rb_isOnLeft(parent)) {                                                                     \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_swapColors(parent, grandparent);                                                    \
                } else {                                                                                     \
                    __rb_leftRotate(id, this, parent);                                                       \
                    __rb_swapColors(x, grandparent);                                                         \
                }                                                                                            \
                __rb_rightRotate(id, this, grandparent);                                                     \
            } else {                                                                                         \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_rightRotate(id, this, parent);                                                      \
                    __rb_swapColors(x, grandparent);                                                         \
                } else {                                                                                     \
                    __rb_swapColors(parent, grandparent);                                                    \
                }                                                                                            \
                __rb_leftRotate(id, this, grandparent);                                                      \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \

#endif /* RBTREE_H */
