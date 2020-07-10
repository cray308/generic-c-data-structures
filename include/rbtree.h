#ifndef RBTREE_H
#define RBTREE_H

#include "ds.h"
#include <stdbool.h>

typedef struct RBNode RBNode;
struct RBNode {
    RBNode *parent;
    RBNode *left;
    RBNode *right;
    enum {
        BLACK,
        RED
    } color;
    char data[];
};

typedef struct {
    RBNode *root;
    RBNode *iter_curr;
    size_t size;
    DSHelper helper;
} Tree;

RBNode* _tree_search(Tree *t, const void *val, bool candidate);

inline RBNode* successor(RBNode *x) {
    if (!x) {
        return x;
    }
	while (x->left) {
		x = x->left;
	}
	return x;
}

inline RBNode *inorder_successor(RBNode *x) {
	if (!x) {
		return NULL;
	} else if (x->right) {
		return successor(x->right);
	}

	RBNode *parent = x->parent;
	while (parent && x == parent->right) {
		x = parent;
		parent = parent->parent;
	}
	return parent;
}


/**
 * Macro to iterate through the tree using an in-order traversal.
 * 
 * @param   t     Pointer to tree.
 * @param   eptr  Pointer which is assigned to the current tree node's data.
 */
#define tree_inorder(t, eptr) \
    for ((t)->iter_curr = successor((t)->root), (eptr) = ((t)->iter_curr != NULL) ? (void *)((t)->iter_curr->data) : NULL; \
         (t)->iter_curr != NULL; \
         (t)->iter_curr = inorder_successor((t)->iter_curr), (eptr) = ((t)->iter_curr != NULL) ? (void *)((t)->iter_curr->data) : NULL)


/**
 * Creates a new RB-Tree.
 *
 * @param   helper  Pointer to DSHelper struct. Since trees require ordering of elements, the
 *                    DSHelper struct MUST contain a comparison function.
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
 * Removes all nodes from the tree, leaving it with the root node set to NULL.
 *
 * @param  this  Pointer to tree.
 */
void tree_clear(Tree *this);


/**
 * Searches for the value using the comparison function in the provided DSHelper struct.
 *
 * @param   t    Pointer to tree.
 * @param   val  Pointer to value to search for.
 *
 * @return       RBNode of the element if it was found, or NULL if it was not found.
 */
#define tree_find(t, val) _tree_search((t), (val), false)


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
void tree_delete_by_val(Tree *t, const void *val);

#endif /* RBTREE_H */
