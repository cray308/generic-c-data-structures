#include "set.h"
#include "array.h"
#include <stdarg.h>

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

	for (int i = 0; i < array_size(vals); ++i) {
		tree_delete_by_val(this, array_at(vals, i));
	}
	array_free(vals);
}

Set *_set_creation_ops(Set *this, Set *other, unsigned char op) {
	if (!other) {
		return NULL;
	}

	Set *s_new = tree_new(&(this->helper));
	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;

	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { /* common */
			if (op & 0x10) {
				tree_insert(s_new, n1->data);
			}
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			if (op & 0x08) {
				tree_insert(s_new, n2->data);
			}
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
			if (op & 0x04) {
				tree_insert(s_new, n1->data);
			}
			n1 = inorder_successor(n1);
		}
	}
	if (op & 0x02) {
		while (n1) {
			tree_insert(s_new, n1->data);
			n1 = inorder_successor(n1);
		}
	}
	if (op & 0x01) {
		while (n2) {
			tree_insert(s_new, n2->data);
			n2 = inorder_successor(n2);
		}
	}
	return s_new;
}

bool _set_bool_ops(Set *this, Set *other, bool subset) {
	if (!other) {
		return false;
	}

	RBNode *n1 = this->root ? successor(this->root) : this->root;
	RBNode *n2 = other->root ? successor(other->root) : other->root;
	int res;
	while (n1 && n2) {
		res = this->helper.cmp(n1->data, n2->data);
		if (res == 0) { /* common */
			if (!subset) return false;
			n1 = inorder_successor(n1);
			n2 = inorder_successor(n2);
		} else if (res > 0) { /* n2 not in "this" */
			n2 = inorder_successor(n2);
		} else { /* n1 not in "other" */
			if (subset) return false;
			n1 = inorder_successor(n1);
		}
	}
	return subset ? (n1 == NULL) : true;
}
