#ifndef RBTREE_H
#define RBTREE_H

#include "ds.h"

typedef void (*tree_print_fn)(const void *_val);

typedef enum {
    BLACK,
    RED
} Color;

typedef struct RBNode RBNode;
struct RBNode {
    RBNode *parent;
    RBNode *left;
    RBNode *right;
    Color color;
    char data[];
};

inline bool isOnLeft(RBNode *n) {
    return n == n->parent->left;
}

inline bool hasRedChild(RBNode *self) { 
    return (self->left != NULL && self->left->color == RED) || (self->right != NULL && self->right->color == RED); 
}

typedef struct {
    RBNode *root;
    DSHelper helper;
} Tree;

typedef Tree Set;

/**
 * Creates a new RB-Tree.
 *
 * @param   helper  Pointer to DSHelper struct.
 *
 * @return          Pointer to the newly created tree.
 */
Tree *tree_new(const DSHelper *helper);

/**
 * Deletes all elements and frees the tree.
 *
 * @param  t  Pointer to tree.
 */
void tree_free(Tree *t);

/**
 * Searches for the value using the comparison function in the provided DSHelper struct.
 *
 * @param   t    Pointer to tree.
 * @param   val  Pointer to value to search for.
 *
 * @return       RBNode of the element if it was found, or NULL if it was not found.
 */
RBNode *tree_find(Tree *t, const void *val);

/**
 * Prints the in-order traversal of the tree using the provided print function.
 *
 * @param  t  Pointer to tree.
 * @param  f  Function pointer to printing function.
 */
void tree_inorder(Tree *t, tree_print_fn f);

/**
 * Inserts a new node into the tree.
 *
 * @param  t    Pointer to tree.
 * @param  val  Pointer to the value to be inserted.
 */
void tree_insert(Tree *t, const void *val);

/**
 * Deletes the provided RBNode from the tree.
 *
 * @param  t  Pointer to tree.
 * @param  z  Node to be deleted.
 */
void tree_delete_node(Tree *t, RBNode *z);

/**
 * Deletes a node from the tree with a value matching that which was provided.
 *
 * @param  t    Pointer to tree.
 * @param  val  Pointer to the value to compare to when searching the tree.
 */
void tree_deleteByVal(Tree *t, const void *val);

#endif // RBTREE_H
