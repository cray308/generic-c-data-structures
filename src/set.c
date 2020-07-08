#include "defaults.h"
#include "set.h"
#include "array.h"

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
	int i;
	for (i = start; i < endIdx; ++i) {
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
	
    if (init == SET_INIT_EMPTY) { /* nothing more to do in this case */
        return s;
    }

    int n;
    void *other;

    /* parse arguments */
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

void set_insert(Set *this, SetInsertType type, ...) {
	int n;
    int start;
    void *other;
	void *s_begin;
	void *s_end;

    /* parse arguments */
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

	/* store values in an array since RB tree deletions involve swapping values
	 * and thus it's not reliable to use RBNode pointers in a bulk delete operation */
	Array *vals = array_new(&(this->helper), ARR_INIT_EMPTY);
	RBNode *curr = begin;
	while (curr != end) {
		array_push_back(vals, curr->data);
		curr = inorder_successor(curr);
	}

	size_t i;
	for (i = 0; i < array_size(vals); ++i) {
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
		if (res == 0) { /* common */
			tree_insert(s_union, n1->data);
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			tree_insert(s_union, n2->data);
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
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
		if (res == 0) { /* common */
			tree_insert(s_inter, n1->data);
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
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
		if (res == 0) { /* common */
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
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
		if (res == 0) { /* common */
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			tree_insert(s_symm, n2->data);
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
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

int set_issubset(Set *this, Set *other) {
	if (!other) {
		return 0;
	}

	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { /* common */
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
			return 0;
		}
	}
	return n1 == NULL;
}

int set_isdisjoint(Set *this, Set *other) {
	if (!other) {
		return 0;
	}

	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { /* common */
			return 0;
		} else if (res > 0) { /* n2 not in "this" */
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
			n1 = inorder_successor(n1);
		}
	}
	return 1;
}
