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

#define __rb_leftRotate(id, t, x) {                                                                          \
    TreeEntry_##id *nParent = (x)->right;                                                                    \
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

#define __rb_rightRotate(id, t, x) {                                                                         \
    TreeEntry_##id *nParent = (x)->left;                                                                     \
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

#define __rb_nextNode_body(x, dir) \
    if (!(x)) return NULL;                                                                                   \
    while ((x)->dir) {                                                                                       \
        (x) = (x)->dir;                                                                                      \
    }                                                                                                        \
    return x;                                                                                                \

#define __rb_nextNode_inorder_body(id, x, dir, nextNode) \
    if (!(x)) return NULL;                                                                                   \
    else if ((x)->dir) return nextNode((x)->dir);                                                            \
                                                                                                             \
    TreeEntry_##id *parent = (x)->parent;                                                                    \
    while (parent && (x) == parent->dir) {                                                                   \
        (x) = parent;                                                                                        \
        parent = parent->parent;                                                                             \
    }                                                                                                        \
    return parent;                                                                                           \


/**
 * Macro to iterate through the tree using an in-order traversal.
 *
 * @param   id    ID used with gen_rbtree.
 * @param   t     Pointer to tree.
 * @param   ptr   TreeEntry which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(TREE, ptr) or ptr->data.
 */
#define tree_iter(id, t, ptr) for (ptr = iter_begin(TREE, id, t, 0); ptr != iter_end(TREE, id, t, 0); iter_next(TREE, id, ptr))


/**
 * Macro to iterate through the tree in reverse (largest element to smallest).
 *
 * @param   id    ID used with gen_rbtree.
 * @param   t     Pointer to tree.
 * @param   ptr   TreeEntry which is assigned to the current element.
 *                 - May be dereferenced with iter_deref(TREE, ptr) or ptr->data.
 */
#define tree_riter(id, t, ptr) for (ptr = iter_rbegin(TREE, id, t, 0); ptr != iter_rend(TREE, id, t, 0); iter_prev(TREE, id, ptr))      


/**
 * Creates a new RB-Tree.
 *
 * @param   id  ID used with gen_rbtree.
 *
 * @return      Pointer to the newly created tree.
 */
#define tree_new(id) __ds_calloc(1, sizeof(Tree_##id))


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
 * @return       TreeEntry of the element if it was found, or NULL if it was not found.
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
 * Deletes the provided TreeEntry from the tree.
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


/* --------------------------------------------------------------------------
 * Tree iterator macros
 * -------------------------------------------------------------------------- */

#define iter_begin_TREE(id, t, n)    __rb_successor_##id((t)->root)
#define iter_end_TREE(id, t, n)      NULL
#define iter_rbegin_TREE(id, t, n)   __rb_predecessor_##id((t)->root)
#define iter_rend_TREE(id, t, n)     NULL
#define iter_next_TREE(id, p)        ((p) = __rb_inorder_successor_##id((p)))
#define iter_prev_TREE(id, p)        ((p) = __rb_inorder_predecessor_##id((p)))
#define iter_deref_TREE(p)           ((p)->data)
#define iter_advance_TREE(id, p, n)  iterator_advance_helper(TREE, id, p, n)
#define iter_dist_TREE(id, p1, p2)   __iter_dist_helper_TREE_##id(p1, p2)


/**
 * Generates RB-tree code for a specified type and ID.
 *
 * @param   id      ID to be used for the Tree struct and function names (must be unique).
 * @param   t       Type to be stored in the given tree node.
 * @param   cmp_lt  Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_rbtree(id, t, cmp_lt)                                                                            \
                                                                                                             \
typedef struct TreeEntry_##id TreeEntry_##id;                                                                \
struct TreeEntry_##id {                                                                                      \
    TreeEntry_##id *parent;                                                                                  \
    TreeEntry_##id *left;                                                                                    \
    TreeEntry_##id *right;                                                                                   \
    __rb_node_color color;                                                                                   \
    t data;                                                                                                  \
};                                                                                                           \
                                                                                                             \
__DS_FUNC_PREFIX TreeEntry_##id *rb_node_new_##id(void) {                                                    \
    TreeEntry_##id *node = __ds_calloc(1, sizeof(TreeEntry_##id));                                           \
    node->color = RED;                                                                                       \
    return node;                                                                                             \
}                                                                                                            \
                                                                                                             \
typedef struct {                                                                                             \
    TreeEntry_##id *root;                                                                                    \
    size_t size;                                                                                             \
} Tree_##id;                                                                                                 \
                                                                                                             \
__DS_FUNC_PREFIX_INL TreeEntry_##id *__rb_successor_##id(TreeEntry_##id *x) {                                \
    __rb_nextNode_body(x, left)                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL TreeEntry_##id *__rb_predecessor_##id(TreeEntry_##id *x) {                              \
    __rb_nextNode_body(x, right)                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL TreeEntry_##id *__rb_inorder_successor_##id(TreeEntry_##id *x) {                        \
    __rb_nextNode_inorder_body(id, x, right, __rb_successor_##id)                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL TreeEntry_##id *__rb_inorder_predecessor_##id(TreeEntry_##id *x) {                      \
    __rb_nextNode_inorder_body(id, x, left, __rb_predecessor_##id)                                           \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_clear_##id(Tree_##id *this);                                                      \
__DS_FUNC_PREFIX void tree_delete_node_##id(Tree_##id *this, TreeEntry_##id *v);                             \
__DS_FUNC_PREFIX void __rb_fixDoubleBlack_##id(Tree_##id *this, TreeEntry_##id *x);                          \
__DS_FUNC_PREFIX void __rb_fixRedRed_##id(Tree_##id *this, TreeEntry_##id *x);                               \
                                                                                                             \
create_iterator_distance_helper(TREE, id, TreeEntry_##id *)                                                  \
                                                                                                             \
__DS_FUNC_PREFIX_INL void tree_free_##id(Tree_##id *this) {                                                  \
    tree_clear_##id(this);                                                                                   \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void tree_clear_##id(Tree_##id *this) {                                                 \
    TreeEntry_##id *curr = this->root;                                                                       \
    while (curr) {                                                                                           \
        tree_delete_node_##id(this, curr);                                                                   \
        curr = this->root;                                                                                   \
    }                                                                                                        \
    this->root = NULL;                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX TreeEntry_##id *__rb_tree_search_##id(Tree_##id *this, t val, bool candidate) {             \
    TreeEntry_##id *temp = this->root;                                                                       \
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
    TreeEntry_##id *newNode = rb_node_new_##id();                                                            \
    if (!this->root) {                                                                                       \
        newNode->data = val;                                                                                 \
        newNode->color = BLACK;                                                                              \
        this->root = newNode;                                                                                \
    } else {                                                                                                 \
        TreeEntry_##id *temp = __rb_tree_search_##id(this, val, true);                                       \
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
__DS_FUNC_PREFIX void tree_delete_node_##id(Tree_##id *this, TreeEntry_##id *v) {                            \
    TreeEntry_##id *u = NULL;                                                                                \
    if (v->left && v->right) u = __rb_successor_##id(v->right);                                              \
    else if (v->left) u = v->left;                                                                           \
    else if (v->right) u = v->right;                                                                         \
                                                                                                             \
    bool uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));                                       \
    TreeEntry_##id *parent = v->parent;                                                                      \
    if (!u) {                                                                                                \
        if (v == this->root) {                                                                               \
            this->root = NULL;                                                                               \
        } else {                                                                                             \
            if (uvBlack) {                                                                                   \
                __rb_fixDoubleBlack_##id(this, v);                                                           \
            } else {                                                                                         \
                TreeEntry_##id *sibling = __rb_getSibling(v);                                                \
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
        return;                                                                                              \
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
    t tmp = u->data;                                                                                         \
    u->data = v->data;                                                                                       \
    v->data = tmp;                                                                                           \
    tree_delete_node_##id(this, u);                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void tree_delete_by_val_##id(Tree_##id *this, t val) {                                      \
    if (!this->root || !val) return;                                                                         \
    TreeEntry_##id *v = __rb_tree_search_##id(this, val, true);                                              \
    if (ds_cmp_neq(cmp_lt, v->data, val)) return;                                                            \
    tree_delete_node_##id(this, v);                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __rb_fixDoubleBlack_##id(Tree_##id *this, TreeEntry_##id *x) {                         \
    if (x == this->root) return;                                                                             \
                                                                                                             \
    TreeEntry_##id *sibling = __rb_getSibling(x), *parent = x->parent;                                       \
    if (!sibling) {                                                                                          \
        __rb_fixDoubleBlack_##id(this, parent);                                                              \
    } else {                                                                                                 \
        if (sibling->color == RED) {                                                                         \
            parent->color = RED;                                                                             \
            sibling->color = BLACK;                                                                          \
                                                                                                             \
            if (__rb_isOnLeft(sibling)) {                                                                    \
                __rb_rightRotate(id, this, parent)                                                           \
            } else {                                                                                         \
                __rb_leftRotate(id, this, parent)                                                            \
            }                                                                                                \
            __rb_fixDoubleBlack_##id(this, x);                                                               \
        } else {                                                                                             \
            if ((sibling->left && sibling->left->color == RED) || (sibling->right && sibling->right->color == RED)) { \
                if (sibling->left && sibling->left->color == RED) {                                          \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->left->color = sibling->color;                                               \
                        sibling->color = parent->color;                                                      \
                        __rb_rightRotate(id, this, parent)                                                   \
                    } else {                                                                                 \
                        sibling->left->color = parent->color;                                                \
                        __rb_rightRotate(id, this, sibling)                                                  \
                        __rb_leftRotate(id, this, parent)                                                    \
                    }                                                                                        \
                } else {                                                                                     \
                    if (__rb_isOnLeft(sibling)) {                                                            \
                        sibling->right->color = parent->color;                                               \
                        __rb_leftRotate(id, this, sibling)                                                   \
                        __rb_rightRotate(id, this, parent)                                                   \
                    } else {                                                                                 \
                        sibling->right->color = sibling->color;                                              \
                        sibling->color = parent->color;                                                      \
                        __rb_leftRotate(id, this, parent)                                                    \
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
__DS_FUNC_PREFIX void __rb_fixRedRed_##id(Tree_##id *this, TreeEntry_##id *x) {                              \
    if (x == this->root) {                                                                                   \
        x->color = BLACK;                                                                                    \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    TreeEntry_##id *parent = x->parent, *grandparent = parent->parent, *uncle = NULL;                        \
    if (parent && grandparent) {                                                                             \
        uncle = __rb_isOnLeft(parent) ? grandparent->right : grandparent->left;                              \
    }                                                                                                        \
                                                                                                             \
    if (parent->color != BLACK) {                                                                            \
        if (uncle && uncle->color == RED) {                                                                  \
            parent->color = BLACK;                                                                           \
            uncle->color = BLACK;                                                                            \
            grandparent->color = RED;                                                                        \
            __rb_fixRedRed_##id(this, grandparent);                                                          \
        } else {                                                                                             \
            if (__rb_isOnLeft(parent)) {                                                                     \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_swapColors(parent, grandparent)                                                     \
                } else {                                                                                     \
                    __rb_leftRotate(id, this, parent)                                                        \
                    __rb_swapColors(x, grandparent)                                                          \
                }                                                                                            \
                __rb_rightRotate(id, this, grandparent)                                                      \
            } else {                                                                                         \
                if (__rb_isOnLeft(x)) {                                                                      \
                    __rb_rightRotate(id, this, parent)                                                       \
                    __rb_swapColors(x, grandparent)                                                          \
                } else {                                                                                     \
                    __rb_swapColors(parent, grandparent)                                                     \
                }                                                                                            \
                __rb_leftRotate(id, this, grandparent)                                                       \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}                                                                                                            \

#endif /* RBTREE_H */
