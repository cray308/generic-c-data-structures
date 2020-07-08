#include "defaults.h"
#include "rbtree.h"
#include "array.h"

/* ------------------------------------------------------------------------- */
/*  RBNode functions    */
/* ------------------------------------------------------------------------- */

inline bool isOnLeft(RBNode *n) {
    return n == n->parent->left;
}

inline bool hasRedChild(RBNode *self) {
    return (self->left != NULL && self->left->color == RED) || (self->right != NULL && self->right->color == RED);
}

inline RBNode *get_uncle(RBNode *n) {
	if (n->parent == NULL || n->parent->parent == NULL) {
		return NULL; 
	}

    if (isOnLeft(n->parent)) {
    	return n->parent->parent->right;
	} else {
    	return n->parent->parent->left;
	}
}

inline RBNode *getSibling(RBNode *n) {
	if (n->parent == NULL) {
      	return NULL;
	}

    if (isOnLeft(n)) {
      	return n->parent->right;
	}
    return n->parent->left; 
}

inline void moveDown(RBNode *self, RBNode *nParent) {
	if (self->parent != NULL) { 
		if (isOnLeft(self)) { 
			self->parent->left = nParent; 
		} else { 
			self->parent->right = nParent; 
		} 
    } 
    nParent->parent = self->parent; 
    self->parent = nParent; 
}

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

void swapColors(RBNode *x1, RBNode *x2) { 
	int temp = x1->color; 
	x1->color = x2->color; 
	x2->color = temp; 
}

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
	set_clear(t);
    free(t);
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


/*  --------------------------------------------  */
/* Set va_args insertion functions */
/*  --------------------------------------------  */


void _set_insert_set(Set *this, Set *other, SetEntry *start, SetEntry *end) {
    if (!other || !other->root) {
        return;
    } else if (!start) {
    	start = successor(other->root);
    }

	RBNode *curr = start;
	RBNode *next;

	while (curr != end) {
		next = inorder_successor(curr);
		tree_insert(this, curr->data);
		curr = next;
	}
}

void _set_insert_builtin(Set *this, void *arr, int start, int n) {
    if (!arr || !n) {
        return;
    }

    char *ptr = (char *) arr + (start * this->helper.size);
    int endIdx = start + n;

	for (int i = start; i < endIdx; ++i) {
		tree_insert(this, ptr);
		ptr += this->helper.size;
	}
}


/*  --------------------------------------------  */
/* main set functions */
/*  --------------------------------------------  */


Set *set_new(const DSHelper *helper, SetInitializer init, ...) {
	Set *s = tree_new(helper);
	if (!s) {
		return NULL;
	}
	
    if (init == SET_INIT_EMPTY) { // nothing more to do in this case
        return s;
    }

    int n;
    void *other;

    // parse arguments
    va_list args;
    va_start(args, init);

    other = va_arg(args, void *);

    if (init == SET_INIT_BUILTIN) {
        n = va_arg(args, int);
    }

    va_end(args);
	if (!other) {
		return s;
	}

    if (init == SET_INIT_BUILTIN) {
        _set_insert_builtin(s, other, 0, n);
    } else {
        _set_insert_set(s, (Set *) other, successor(((Set *) other)->root), NULL);
    }
    return s;
}

void set_clear(Set *this) {
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

void set_insert(Set *this, SetInsertType type, ...) {
	int n;
    int start;
    void *other;
	void *s_begin;
	void *s_end;

    // parse arguments
    va_list args;
    va_start(args, type);

    other = va_arg(args, void *);

    if (type == SET_INSERT_BUILTIN) {
        start = va_arg(args, int);
        n = va_arg(args, int);
    } else if (type == SET_INSERT_SET) {
		s_begin = va_arg(args, void *);
        s_end = va_arg(args, void *);
	}

    va_end(args);

    switch (type) {
        case SET_INSERT_SINGLE:
			tree_insert(this, other);
            break;
        case SET_INSERT_BUILTIN:
            _set_insert_builtin(this, other, start, n);
            break;
        case SET_INSERT_SET:
            _set_insert_set(this, (Set *) other, (SetEntry *) s_begin, (SetEntry *) s_end);
            break;
    }
}

void set_erase(Set *this, SetEntry *begin, SetEntry *end) {
	if (!begin) {
		begin = successor(this->root);
	}

	// store values in an array since RB tree deletions involve swapping values
	// and thus it's not reliable to use RBNode pointers in a bulk delete operation
	Array *vals = array_new(&(this->helper), ARR_INIT_EMPTY);
	RBNode *curr = begin;
	while (curr != end) {
		array_push_back(vals, curr->data);
		curr = inorder_successor(curr);
	}

	for (size_t i = 0; i < array_size(vals); ++i) {
		tree_delete_by_val(this, array_at(vals, i));
	}
	array_free(vals);
}

Set *set_union(Set *this, Set *other) {
	if (!other) {
		return NULL;
	}

	Set *s_union = tree_new(&(this->helper));
	RBNode *n1 = this->root;
	if (this->root) {
		n1 = successor(this->root);
	}
	
	RBNode *n2 = other->root;
	if (other->root) {
		n2 = successor(other->root);
	}
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			tree_insert(s_union, n1->data);
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { // n2 not in "this"
			tree_insert(s_union, n2->data);
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			tree_insert(s_union, n1->data);
			n1 = inorder_successor(n1);
		}
	}

	while (n1) {
		tree_insert(s_union, n1->data);
		n1 = inorder_successor(n1);
	}
	while (n2) {
		tree_insert(s_union, n2->data);
		n2 = inorder_successor(n2);
	}
	return s_union;
}

Set *set_intersection(Set *this, Set *other) {
	if (!other) {
		return NULL;
	}

	Set *s_inter = tree_new(&(this->helper));
	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			tree_insert(s_inter, n1->data);
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { // n2 not in "this"
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			n1 = inorder_successor(n1);
		}
	}
	return s_inter;
}

Set *set_difference(Set *this, Set *other) {
	if (!other) {
		return NULL;
	}

	Set *s_diff = tree_new(&(this->helper));
	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { // n2 not in "this"
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			tree_insert(s_diff, n1->data);
			n1 = inorder_successor(n1);
		}
	}

	while (n1) {
		tree_insert(s_diff, n1->data);
		n1 = inorder_successor(n1);
	}
	return s_diff;
}

Set *set_symmetric_difference(Set *this, Set *other) {
	if (!other) {
		return NULL;
	}

	Set *s_symm = tree_new(&(this->helper));
	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { // n2 not in "this"
			tree_insert(s_symm, n2->data);
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			tree_insert(s_symm, n1->data);
			n1 = inorder_successor(n1);
		}
	}

	while (n1) {
		tree_insert(s_symm, n1->data);
		n1 = inorder_successor(n1);
	}
	while (n2) {
		tree_insert(s_symm, n2->data);
		n2 = inorder_successor(n2);
	}
	return s_symm;
}

bool set_issubset(Set *this, Set *other) {
	if (!other) {
		return false;
	}

	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { // n2 not in "this"
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			return false;
		}
	}
	return n1 == NULL;
}

bool set_isdisjoint(Set *this, Set *other) {
	if (!other) {
		return false;
	}

	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { // common
			return false;
		} else if (res > 0) { // n2 not in "this"
			n2 = inorder_successor(n2);
		} else { // n1 not in "other"
			n1 = inorder_successor(n1);
		}
	}
	return true;
}
