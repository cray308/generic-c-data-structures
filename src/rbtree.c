#include "rbtree.h"
#include <stdbool.h>

/* -------------------------------------------------------------------------
 *  RBNode functions
 * ------------------------------------------------------------------------- */

#define isOnLeft(n) ((n) == (n)->parent->left)

#define hasRedChild(n) \
	(((n)->left != NULL && (n)->left->color == RED) || ((n)->right != NULL && (n)->right->color == RED))

#define get_uncle(n) \
	(((n)->parent == NULL || (n)->parent->parent == NULL) ? NULL : \
	 (isOnLeft((n)->parent) ? (n)->parent->parent->right : (n)->parent->parent->left))

#define getSibling(n) \
	(((n)->parent == NULL) ? NULL : (isOnLeft((n)) ? (n)->parent->right : (n)->parent->left))

#define moveDown(s, np) \
	do { \
		if ((s)->parent != NULL) { \
			if (isOnLeft((s))) { \
				(s)->parent->left = (np); \
			} else { \
				(s)->parent->right = (np); \
			} \
		} \
		(np)->parent = (s)->parent; \
		(s)->parent = (np); \
	} while (0)

#define BSTreplace(x) \
	(((x)->left && (x)->right) ? (successor((x)->right)) : ((!((x)->left) && !((x)->right)) ? (NULL) : (((x)->left != NULL) ? ((x)->left) : ((x)->right))))

#define swapColors(x1, x2) \
	do { \
		int _tmp_color; \
		_tmp_color = (x1)->color; \
		(x1)->color = (x2)->color; \
		(x2)->color = _tmp_color; \
	} while(0)

RBNode *rb_node_new(size_t size) {
    size_t bytes = sizeof(RBNode) + size;
    RBNode *node = malloc(bytes);
    if (!node) {
		DS_OOM();
	}
    memset(node, 0, bytes);
	node->color = RED;
    return node;
}

/* ------------------------------------------------------------------------- */
/*  Tree functions    */
/* ------------------------------------------------------------------------- */

#define tree_copy(t, n, value) \
    do { \
        if ((t)->helper.copy) { \
            (t)->helper.copy((n)->data, (value)); \
        } else { \
            memcpy((n)->data, (value), (t)->helper.size); \
        } \
    } while(0)

#define tree_rm(t, n) \
    do { \
        if ((t)->helper.del) { \
            (t)->helper.del((n)->data); \
        } \
    } while(0)

#define swapValues(t, u, v) \
	do { \
		Node *_temp = node_new((t)->helper.size); \
		tree_copy((t), (_temp), ((u)->data)); \
		tree_rm((t), (u)); \
		tree_copy((t), (u), ((v)->data)); \
		tree_rm((t), (v)); \
		tree_copy((t), (v), (_temp->data)); \
		tree_rm((t), (_temp)); \
		free(_temp); \
	} while (0)

#define leftRotate(t, x) \
	do { \
		RBNode *_nParent; \
		_nParent = (x)->right; \
		if ((x) == (t)->root) { \
			(t)->root = _nParent; \
		} \
		moveDown((x), _nParent); \
		(x)->right = _nParent->left; \
		if (_nParent->left) { \
			_nParent->left->parent = (x); \
		} \
		_nParent->left = (x); \
	} while (0)


#define rightRotate(t, x) \
	do { \
		RBNode *_nParent = (x)->left; \
		if ((x) == (t)->root) { \
			(t)->root = _nParent; \
		} \
		moveDown((x), _nParent); \
		(x)->left = _nParent->right; \
		if (_nParent->right) { \
			_nParent->right->parent = (x); \
		} \
		_nParent->right = (x); \
	} while (0)

void fixRedRed(Tree *t, RBNode *x);
void fixDoubleBlack(Tree *t, RBNode *x);

Tree *tree_new(const DSHelper *helper) {
    if (!helper || helper->size == 0 || !(helper->cmp)) {
		return NULL;
	}
    Tree *t = malloc(sizeof(Tree));
    if (!t) {
		DS_OOM();
	}
    t->root = NULL;
	t->size = 0;
    t->helper = *helper;
    return t;
}

void tree_free(Tree *t) {
	tree_clear(t);
    free(t);
}

void tree_clear(Tree *this) {
	RBNode *curr = this->root;
	while (curr) {
		tree_delete_node(this, curr);
		curr = this->root;
	}
	this->root = NULL;
}

void fixRedRed(Tree *t, RBNode *x) { 
	if (x == t->root) {
		x->color = BLACK;
		return;
	}

	RBNode *parent = x->parent, *grandparent = parent->parent, *uncle = get_uncle(x);
	if (parent->color != BLACK) {
		if (uncle && uncle->color == RED) {
			parent->color = BLACK;
			uncle->color = BLACK;
			grandparent->color = RED;
			fixRedRed(t, grandparent);
		} else {
			if (isOnLeft(parent)) {
				if (isOnLeft(x)) {
					swapColors(parent, grandparent);
				} else {
					leftRotate(t, parent);
					swapColors(x, grandparent);
				}
				rightRotate(t, grandparent);
			} else {
				if (isOnLeft(x)) {
					rightRotate(t, parent);
					swapColors(x, grandparent);
				} else {
					swapColors(parent, grandparent);
				}
				leftRotate(t, grandparent);
			}
		}
	}
}

void tree_delete_node(Tree *t, RBNode *v) {
	RBNode *u = BSTreplace(v);

	bool uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));
	RBNode *parent = v->parent;
	if (!u) {
		if (v == t->root) {
			t->root = NULL;
		} else {
			if (uvBlack) {
				fixDoubleBlack(t, v);
			} else {
				RBNode *sibling = getSibling(v);
				if (sibling != NULL) {
					sibling->color = RED;
				}
			}

			if (isOnLeft(v)) {
				parent->left = NULL;
			} else {
				parent->right = NULL;
			}
		}
		tree_rm(t, v);
		free(v);
		t->size--;
		return;
	}

	if (!v->left || !v->right) {
		if (v == t->root) {
			tree_rm(t, v);
			tree_copy(t, v, u->data);
			tree_rm(t, u);
			free(u);
			v->left = v->right = NULL;
			t->size--;
		} else {
			if (isOnLeft(v)) {
				parent->left = u;
			} else {
				parent->right = u;
			}

			tree_rm(t, v);
			free(v);
			t->size--;

			u->parent = parent; 
			if (uvBlack) {
				fixDoubleBlack(t, u);
			} else {
				u->color = BLACK;
			}
		}
		return;
	}
	swapValues(t, u, v);
	tree_delete_node(t, u);
}

void fixDoubleBlack(Tree *t, RBNode *x) {
	if (x == t->root) {
		return;
	}

	RBNode *sibling = getSibling(x), *parent = x->parent;
	if (!sibling) {
		fixDoubleBlack(t, parent);
	} else {
		if (sibling->color == RED) {
			parent->color = RED;
			sibling->color = BLACK;

			if (isOnLeft(sibling)) {
				rightRotate(t, parent);
			} else {
				leftRotate(t, parent);
			}
			fixDoubleBlack(t, x);
		} else {
			if (hasRedChild(sibling)) {
				if (sibling->left && sibling->left->color == RED) {
					if (isOnLeft(sibling)) {
						sibling->left->color = sibling->color;
						sibling->color = parent->color;
						rightRotate(t, parent);
					} else {
						sibling->left->color = parent->color;
						rightRotate(t, sibling);
						leftRotate(t, parent);
					}
				} else {
					if (isOnLeft(sibling)) {
						sibling->right->color = parent->color;
						leftRotate(t, sibling);
						rightRotate(t, parent);
					} else {
						sibling->right->color = sibling->color;
						sibling->color = parent->color;
						leftRotate(t, parent);
					}
				}

				parent->color = BLACK;
			} else {
				sibling->color = RED;

				if (parent->color == BLACK) {
					fixDoubleBlack(t, parent);
				} else {
					parent->color = BLACK;
				}
			}
		}
	}
}

RBNode* _tree_search(Tree *t, const void *val, bool candidate) {
	RBNode *temp = t->root;
	int res;
	while (temp) {
		res = t->helper.cmp(val, temp->data);
		if (res < 0) {
			if (!temp->left) {
				if (candidate) break;
				else return NULL;
			} else {
				temp = temp->left;
			}
		} else if (res == 0) {
			break;
		} else {
			if (!temp->right) {
				if (candidate) break;
				else return NULL;
			} else {
				temp = temp->right;
			}
		}
	}
	return temp; 
}

void tree_insert(Tree *t, const void *val) {
	RBNode *newNode = rb_node_new(t->helper.size);
	if (!t->root) {
		tree_copy(t, newNode, val);
		newNode->color = BLACK;
		t->root = newNode;
	} else {
		RBNode *temp = _tree_search(t, val, true);
		if (t->helper.cmp(temp->data, val) == 0) {
			free(newNode);
			return;
		}
		tree_copy(t, newNode, val);
		newNode->parent = temp;

		if (t->helper.cmp(val, temp->data) < 0) {
			temp->left = newNode;
		} else {
			temp->right = newNode;
		}
		fixRedRed(t, newNode);
	}
	t->size++;
}

void tree_delete_by_val(Tree *t, const void *val) {
	if (!t->root || !val) {
		return;
	}
	RBNode *v = _tree_search(t, val, true);
	if (t->helper.cmp(v->data, val) != 0) {
		return;
	}
	tree_delete_node(t, v);
}
