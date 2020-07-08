#include "defaults.h"
#include "rbtree.h"
#include "array.h"

/* ------------------------------------------------------------------------- */
/*  RBNode functions    */
/* ------------------------------------------------------------------------- */

#define isOnLeft(n) ((n) == (n)->parent->left)

#define hasRedChild(n) \
	(((n)->left != NULL && (n)->left->color == RED) || ((n)->right != NULL && (n)->right->color == RED))

#define get_uncle(n) \
	(((n)->parent == NULL || (n)->parent->parent == NULL) ? NULL : \
	 (isOnLeft((n)->parent) ? (n)->parent->parent->right : (n)->parent->parent->left))

#define getSibling(n) \
	(((n)->parent == NULL) ? NULL : (isOnLeft((n)) ? (n)->parent->right : (n)->parent->left))

#define moveDown(self, nParent) \
	do { \
		if ((self)->parent != NULL) { \
			if (isOnLeft((self))) { \
				(self)->parent->left = (nParent); \
			} else { \
				(self)->parent->right = (nParent); \
			} \
		} \
		(nParent)->parent = (self)->parent; \
		(self)->parent = (nParent); \
	} while (0)

RBNode* BSTreplace(RBNode *x) {
	if (x->left && x->right) {
		return successor(x->right);
	}
	
	if (!x->left && !x->right) {
		return NULL;
	}

	if (x->left != NULL) {
		return x->left;
	} else {
		return x->right;
	}
}

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

void leftRotate(Tree *t, RBNode *x);
void rightRotate(Tree *t, RBNode *x);
void swapValues(Tree *t, RBNode *u, RBNode *v);
void fixRedRed(Tree *t, RBNode *x);
void fixDoubleBlack(Tree *t, RBNode *x);
RBNode* tree_search(Tree *t, const void *val);

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
	if (!this->root) {
		return;
	}
	RBNode *curr = this->root;

	while (curr) {
		tree_delete_node(this, curr);
		curr = this->root;
	}
	this->root = NULL;
}

void leftRotate(Tree *t, RBNode *x) { 
	RBNode *nParent = x->right;

	if (x == t->root) {
		t->root = nParent; 
	}

	moveDown(x, nParent);
	x->right = nParent->left;

	if (nParent->left) {
		nParent->left->parent = x;
	}
	nParent->left = x;
} 

void rightRotate(Tree *t, RBNode *x) {
	RBNode *nParent = x->left;

	if (x == t->root) {
		t->root = nParent; 
	}
	
	moveDown(x, nParent);
	x->left = nParent->right;

	if (nParent->right != NULL) {
		nParent->right->parent = x;
	}
	nParent->right = x;
}

void swapValues(Tree *t, RBNode *u, RBNode *v) {
	Node *temp = node_new(t->helper.size);
	if (t->helper.copy) {
        t->helper.copy(temp->data, u->data);
    } else {
        memcpy(temp->data, u->data, t->helper.size);
    }
	
	if (t->helper.del) {
        t->helper.del(u->data);
    }
	if (t->helper.copy) {
        t->helper.copy(u->data, v->data);
    } else {
        memcpy(u->data, v->data, t->helper.size);
    }

	if (t->helper.del) {
        t->helper.del(v->data);
    }
	if (t->helper.copy) {
        t->helper.copy(v->data, temp->data);
    } else {
        memcpy(v->data, temp->data, t->helper.size);
    }

	if (t->helper.del) {
        t->helper.del(temp->data);
    }
	free(temp);
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

	int uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));
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
		if (t->helper.del) {
			t->helper.del(v->data);
		}
		t->size--;
		free(v);
		return;
	}

	if (!v->left || !v->right) {
		if (v == t->root) {
			if (t->helper.del) {
				t->helper.del(v->data);
			}

			if (t->helper.copy) {
				t->helper.copy(v->data, u->data);
			} else {
				memcpy(v->data, u->data, t->helper.size);
			}
			v->left = v->right = NULL;

			if (t->helper.del) {
				t->helper.del(u->data);
			}
			t->size--;
			free(u);
		} else {
			if (isOnLeft(v)) {
				parent->left = u;
			} else {
				parent->right = u;
			}

			if (t->helper.del) {
				t->helper.del(v->data);
			}
			t->size--;
			free(v);

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

RBNode* tree_search(Tree *t, const void *val) {
	RBNode *temp = t->root;
	int res;
	while (temp) {
		res = t->helper.cmp(val, temp->data);
		if (res < 0) {
			if (!temp->left) {
				break;
			} else {
				temp = temp->left;
			}
		} else if (res == 0) {
			break;
		} else {
			if (!temp->right) {
				break;
			} else {
				temp = temp->right;
			}
		}
	}
	return temp; 
}

RBNode* tree_find(Tree *t, const void *val) {
	RBNode *temp = t->root;
	int res;
	while (temp) {
		res = t->helper.cmp(val, temp->data);
		if (res < 0) {
			if (!temp->left) {
				return NULL;
			} else {
				temp = temp->left;
			}
		} else if (res == 0) {
			break;
		} else {
			if (!temp->right) {
				return NULL;
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
		if (t->helper.copy) {
			t->helper.copy(newNode->data, val);
		} else {
			memcpy(newNode->data, val, t->helper.size);
		}
		newNode->color = BLACK;
		t->root = newNode;
	} else {
		RBNode *temp = tree_search(t, val);
		if (t->helper.cmp(temp->data, val) == 0) {
			free(newNode);
			return;
		}

		if (t->helper.copy) {
			t->helper.copy(newNode->data, val);
		} else {
			memcpy(newNode->data, val, t->helper.size);
		}

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

	RBNode *v = tree_search(t, val);

	if (t->helper.cmp(v->data, val) != 0) {
		return;
	}
	tree_delete_node(t, v);
}
