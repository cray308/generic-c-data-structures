#include "defaults.h"
#include "rbtree.h"

RBNode *rb_node_new(size_t size);
RBNode *get_uncle(RBNode *n);
RBNode *getSibling(RBNode *n);
void moveDown(RBNode *self, RBNode *nParent);
void leftRotate(Tree *t, RBNode *x);
void rightRotate(Tree *t, RBNode *x);
void swapColors(RBNode *x1, RBNode *x2);
void swapValues(Tree *t, RBNode *u, RBNode *v);
void fixRedRed(Tree *t, RBNode *x);
RBNode* successor(RBNode *x);
RBNode* BSTreplace(RBNode *x);
void fixDoubleBlack(Tree *t, RBNode *x);
void node_inorder(RBNode *x, tree_print_fn f);
RBNode* tree_search(Tree *t, const void *val);

RBNode *rb_node_new(size_t size) {
    size_t bytes = sizeof(RBNode) + size;
    RBNode *node = malloc(bytes);
    if (!node) DS_OOM();
    memset(node, 0, bytes);
	node->color = RED;
    return node;
}

RBNode *get_uncle(RBNode *n) {
	if (n->parent == NULL || n->parent->parent == NULL) return NULL; 
  
    if (isOnLeft(n->parent)) 
      // uncle on right 
      return n->parent->parent->right; 
    else
      // uncle on left
      return n->parent->parent->left;
}

RBNode *getSibling(RBNode *n) {
	if (n->parent == NULL)
      	return NULL;
    if (isOnLeft(n))
      	return n->parent->right;
    return n->parent->left; 
}

void moveDown(RBNode *self, RBNode *nParent) {
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

Tree *tree_new(const DSHelper *helper) {
    if (!helper || helper->size == 0 || !(helper->cmp)) return NULL;
    Tree *t = malloc(sizeof(Tree));
    if (!t) DS_OOM();
    memset(t, 0, sizeof(Tree));
    t->root = NULL;
    t->helper = *helper;
    return t;
}

void tree_free(Tree *t) {
    RBNode *curr = t->root;
    while (curr != NULL) {
        tree_delete_node(t, curr);
        curr = t->root;
    }
    free(t);
}

void leftRotate(Tree *t, RBNode *x) { 
	// new parent will be node's right child 
	RBNode *nParent = x->right; 
	
	// update root if current node is root 
	if (x == t->root) 
		t->root = nParent; 
	
	moveDown(x, nParent);
	
	// connect x with new parent's left element 
	x->right = nParent->left; 
	// connect new parent's left element with node 
	// if it is not null 
	if (nParent->left != NULL) 
		nParent->left->parent = x; 
	
	// connect new parent with x 
	nParent->left = x; 
} 

void rightRotate(Tree *t, RBNode *x) { 
	// new parent will be node's left child 
	RBNode *nParent = x->left; 
	
	// update root if current node is root 
	if (x == t->root) 
		t->root = nParent; 
	
	moveDown(x, nParent);
	
	// connect x with new parent's right element 
	x->left = nParent->right; 
	// connect new parent's right element with node 
	// if it is not null 
	if (nParent->right != NULL) 
		nParent->right->parent = x; 
	
	// connect new parent with x 
	nParent->right = x; 
}

void swapColors(RBNode *x1, RBNode *x2) { 
	Color temp; 
	temp = x1->color; 
	x1->color = x2->color; 
	x2->color = temp; 
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
	// if x is root color it black and return 
	if (x == t->root) { 
		x->color = BLACK; 
		return; 
	} 
	
	// initialize parent, grandparent, uncle 
	RBNode *parent = x->parent, *grandparent = parent->parent, *uncle = get_uncle(x);
	
	if (parent->color != BLACK) { 
		if (uncle != NULL && uncle->color == RED) { 
			// uncle red, perform recoloring and recurse 
			parent->color = BLACK; 
			uncle->color = BLACK; 
			grandparent->color = RED; 
			fixRedRed(t, grandparent); 
		} else { 
			// Else perform LR, LL, RL, RR 
			if (isOnLeft(parent)) { 
				if (isOnLeft(x)) { 
					// for left right 
					swapColors(parent, grandparent); 
				} else { 
					leftRotate(t, parent); 
					swapColors(x, grandparent); 
				} 
				// for left left and left right 
				rightRotate(t, grandparent); 
			} else { 
				if (isOnLeft(x)) { 
					// for right left 
					rightRotate(t, parent); 
					swapColors(x, grandparent); 
				} else { 
					swapColors(parent, grandparent); 
				} 
		
				// for right right and right left 
				leftRotate(t, grandparent); 
			} 
		} 
	} 
} 

RBNode* successor(RBNode *x) { 
	RBNode *temp = x;
	while (temp->left != NULL)
		temp = temp->left;
	return temp;
}

RBNode* BSTreplace(RBNode *x) { 
	// when node have 2 children 
	if (x->left != NULL && x->right != NULL) 
		return successor(x->right); 
	
	// when leaf 
	if (x->left == NULL && x->right == NULL) 
		return NULL; 
	
	// when single child 
	if (x->left != NULL) 
		return x->left; 
	else
		return x->right; 
}

void tree_delete_node(Tree *t, RBNode *v) {
	RBNode *u = BSTreplace(v); 
	
	// True when u and v are both black 
	bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK)); 
	RBNode *parent = v->parent; 
	
	if (u == NULL) { 
	// u is NULL therefore v is leaf 
		if (v == t->root) { 
			// v is root, making root null 
			t->root = NULL; 
		} else { 
			if (uvBlack) { 
				// u and v both black 
				// v is leaf, fix double black at v 
				fixDoubleBlack(t, v); 
			} else { 
				// u or v is red 
				RBNode *sibling = getSibling(v);
				if (sibling != NULL) {
					sibling->color = RED; // sibling is not null, make it red"
				}
			} 
			// delete v from the tree 
			if (isOnLeft(v)) { 
				parent->left = NULL; 
			} else { 
				parent->right = NULL; 
			} 
		}
		if (t->helper.del) {
			t->helper.del(v->data);
		}
		free(v);
		return; 
	}
	
	if (v->left == NULL || v->right == NULL) { 
	// v has 1 child 
		if (v == t->root) { 
			// v is root, assign the value of u to v, and delete u
			if (t->helper.del) {
				t->helper.del(v->data);
			}
			if (t->helper.copy) {
				t->helper.copy(v->data, u->data);
			} else {
				memcpy(v->data, u->data, t->helper.size);
			}
			//v->val = u->val; 
			v->left = v->right = NULL;

			if (t->helper.del) {
				t->helper.del(u->data);
			}
			free(u);
		} else {
			// Detach v from tree and move u up 
			if (isOnLeft(v)) {
				parent->left = u;
			} else {
				parent->right = u;
			}
			if (t->helper.del) {
				t->helper.del(v->data);
			}
			free(v);
			//delete v; 
			u->parent = parent; 
			if (uvBlack) { 
			// u and v both black, fix double black at u 
				fixDoubleBlack(t, u); 
			} else { 
			// u or v red, color u black 
				u->color = BLACK; 
			} 
		} 
		return; 
	} 
	
	// v has 2 children, swap values with successor and recurse 
	swapValues(t, u, v); 
	tree_delete_node(t, u); 
} 

void fixDoubleBlack(Tree *t, RBNode *x) { 
	if (x == t->root) 
		return; 
	
	RBNode *sibling = getSibling(x), *parent = x->parent; 
	if (sibling == NULL) { 
	// No sibiling, double black pushed up 
		fixDoubleBlack(t, parent); 
	} else { 
		if (sibling->color == RED) { 
			// Sibling red 
			parent->color = RED; 
			sibling->color = BLACK; 
			if (isOnLeft(sibling)) { 
			// left case 
				rightRotate(t, parent); 
			} else { 
			// right case 
				leftRotate(t, parent); 
			} 
			fixDoubleBlack(t, x); 
		} else { 
			// Sibling black 
			if (hasRedChild(sibling)) { 
			// at least 1 red children 
				if (sibling->left != NULL && sibling->left->color == RED) { 
					if (isOnLeft(sibling)) { 
					// left left 
						sibling->left->color = sibling->color; 
						sibling->color = parent->color; 
						rightRotate(t, parent); 
					} else { 
					// right left 
						sibling->left->color = parent->color; 
						rightRotate(t, sibling); 
						leftRotate(t, parent); 
					}
				} else { 
					if (isOnLeft(sibling)) { 
					// left right 
						sibling->right->color = parent->color; 
						leftRotate(t, sibling); 
						rightRotate(t, parent); 
					} else { 
					// right right 
						sibling->right->color = sibling->color; 
						sibling->color = parent->color; 
						leftRotate(t, parent); 
					} 
				}
				parent->color = BLACK; 
			} else { 
			// 2 black children 
				sibling->color = RED; 
				if (parent->color == BLACK) 
				fixDoubleBlack(t, parent); 
				else
				parent->color = BLACK; 
			} 
		} 
	} 
} 

void tree_inorder(Tree *t, tree_print_fn f) { 
	if (t->root == NULL) return;
	node_inorder(t->root->left, f); 
	f(t->root->data);
	node_inorder(t->root->right, f); 
}

void node_inorder(RBNode *x, tree_print_fn f) {
	if (x == NULL) return;
	node_inorder(x->left, f);
	f(x->data);
	node_inorder(x->right, f);
}

RBNode* tree_search(Tree *t, const void *val) { 
	RBNode *temp = t->root;
	int res;
	while (temp != NULL) {
		res = t->helper.cmp(val, temp->data);
		if (res < 0) {
			if (temp->left == NULL) 
				break;
			else
				temp = temp->left; 
		} else if (res == 0) { 
			break; 
		} else {
			if (temp->right == NULL) 
				break;
			else
				temp = temp->right; 
		} 
	}
	return temp; 
}

RBNode* tree_find(Tree *t, const void *val) { 
	RBNode *temp = t->root;
	int res;
	while (temp != NULL) {
		res = t->helper.cmp(val, temp->data);
		if (res < 0) {
			if (temp->left == NULL) 
				return NULL;
			else
				temp = temp->left; 
		} else if (res == 0) { 
			break; 
		} else {
			if (temp->right == NULL) 
				return NULL;
			else
				temp = temp->right; 
		} 
	}
	return temp; 
} 

void tree_insert(Tree *t, const void *val) { 
	RBNode *newNode = rb_node_new(t->helper.size); 
	if (t->root == NULL) {
		if (t->helper.copy) {
			t->helper.copy(newNode->data, val);
		} else {
			memcpy(newNode->data, val, t->helper.size);
		}
	// when root is null 
	// simply insert value at root 
		newNode->color = BLACK; 
		t->root = newNode;
	} else { 
		RBNode *temp = tree_search(t, val);
	
		if (t->helper.cmp(temp->data, val) == 0) { 
		// return if value already exists
			free(newNode);
			return; 
	}
	if (t->helper.copy) {
        t->helper.copy(newNode->data, val);
    } else {
        memcpy(newNode->data, val, t->helper.size);
    }
	
	// if value is not found, tree_search returns the node 
	// where the value is to be inserted 
	
	// connect new node to correct node 
	newNode->parent = temp; 
	
	if (t->helper.cmp(val, temp->data) < 0) 
		temp->left = newNode; 
	else
		temp->right = newNode; 
	
	// fix red red voilaton if exists 
		fixRedRed(t, newNode); 
	} 
} 

void tree_deleteByVal(Tree *t, const void *val) { 
	if (t->root == NULL) 
		// Tree is empty 
		return; 
	
	RBNode *v = tree_search(t, val);

	
	if (t->helper.cmp(v->data, val) != 0) { 
		return; 
	}
	
	tree_delete_node(t, v); 
} 
