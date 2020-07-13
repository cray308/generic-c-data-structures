#include "list.h"
#include <stdarg.h>

typedef enum {
    LIST_RM_OP_UNIQUE,
    LIST_RM_OP_VALUE,
    LIST_RM_OP_COND
} ListRemovalOp;

DLLNode *_list_insert_elem(List *l, DLLNode *pos, void *value, bool before);
DLLNode *_list_insert_builtin(List *l, DLLNode *pos, void *arr, int start, int n, bool sorted);
DLLNode *_list_insert_list(List *l, DLLNode *pos, List *other, DLLNode *start, DLLNode *end, bool sorted);
DLLNode *_list_insert_elem_sorted(List *l, void *value);
void _list_transfer_all(List *this, DLLNode *position, List *other);
void _list_transfer_single(List *this, DLLNode *position, List *other, DLLNode *e);
void _list_transfer_range(List *this, DLLNode *position, List *other, DLLNode *first, DLLNode *last);

/* --------------------------------------------------------------------------
 * List sort - swap data in two lists
 * -------------------------------------------------------------------------- */

#define _list_swap(x, y) \
    do { \
        DLLNode *_ltemp_front = (x)->front; \
        DLLNode *_ltemp_back = (x)->back; \
        size_t _ltemp_size = (x)->size; \
        (x)->front = (y)->front; \
        (x)->back = (y)->back; \
        (x)->size = (y)->size; \
        (y)->front = _ltemp_front; \
        (y)->back = _ltemp_back; \
        (y)->size = _ltemp_size; \
    } while(0)

/* ------------------------------------------------------------------------- */
/*  Copy/remove helper functions    */
/* ------------------------------------------------------------------------- */

#define list_copy(l, n, value) \
    do { \
        if ((l)->helper.copy) { \
            (l)->helper.copy((n)->data, (value)); \
        } else { \
            memcpy((n)->data, (value), (l)->helper.size); \
        } \
    } while(0)

#define list_rm(l, n) \
    do { \
        if ((l)->helper.del) { \
            (l)->helper.del((n)->data); \
        } \
    } while(0)

/* ------------------------------------------------------------------------- */
/*  DLLNode initializer    */
/* ------------------------------------------------------------------------- */

DLLNode *dll_node_new(size_t size) {
    size_t bytes = sizeof(DLLNode) + size;
    DLLNode *node = malloc(bytes);
    if (!node) {
        DS_OOM();
    }
    memset(node, 0, bytes);
    return node;
}

/* ------------------------------------------------------------------------- */
/*  Main list functions    */
/* ------------------------------------------------------------------------- */

List *list_new(const DSHelper *helper, ListInitializer init, ...) {
    if (!helper || helper->size == 0) {
        return NULL;
    }

    List *l = malloc(sizeof(List));
    if (!l) {
        DS_OOM();
    }
    memset(l, 0, sizeof(List));
    l->helper = *(helper);

    if (init == LIST_INIT_EMPTY) { /* nothing more to do in this case */
        return l;
    }

    int n;
    void *other;

    /* parse arguments */
    va_list args;
    va_start(args, init);

    other = va_arg(args, void *);

    if (init == LIST_INIT_BUILTIN) {
        n = va_arg(args, int);
    }

    va_end(args);

    if (init == LIST_INIT_BUILTIN) {
        _list_insert_builtin(l, LIST_END, other, 0, n, false);
    } else {
        _list_insert_list(l, LIST_END, (List *) other, ((List *) other)->front, LIST_END, false);
    }
    return l;
}

void list_free(List *l) {
    list_clear(l);
    free(l);
}

void _list_push_val(List *l, const void *value, bool front) {
    if (!value) {
        return;
    }
    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    if (!(l->front)) {
        l->front = new;
        l->back = new;
    } else {
        if (front) {
            new->next = l->front;
            l->front->prev = new;
            l->front = new;
        } else {
            new->prev = l->back;
            l->back->next = new;
            l->back = new;
        }
    }
    l->size++;
}

void _list_pop_val(List *l, bool front) {
    if (!(l->front)) {
        return;
    }
    DLLNode *repl = front ? l->front : l->back;

    if (front) {
        l->front = repl->next;
        if (l->front) {
            l->front->prev = NULL;
        } else {
            l->back = NULL;
        }
    } else {
        l->back = repl->prev;
        if (l->back) {
            l->back->next = NULL;
        } else {
            l->front = NULL;
        }
    }
    list_rm(l, repl);
    free(repl);
    l->size--;
}

DLLNode *list_insert(List *l, DLLNode *pos, bool sorted, ListInsertType type, ...) {    
    void *value;
    int builtin_start;
    int builtin_n;
    void *l_start;
    void *l_end;

    va_list args;
    va_start(args, type);

    value = va_arg(args, void *);

    if (type == LIST_INSERT_BUILTIN) {
        builtin_start = va_arg(args, int);
        builtin_n = va_arg(args, int);
    } else if (type == LIST_INSERT_LIST) {
        l_start = va_arg(args, void *);
        l_end = va_arg(args, void *);
    }

    va_end(args);

    DLLNode *rv = LIST_END;

    switch (type) {
        case LIST_INSERT_SINGLE:
            rv = sorted ? _list_insert_elem_sorted(l, value) : _list_insert_elem(l, pos, (DLLNode *)value, true);
            break;
        case LIST_INSERT_BUILTIN:
            rv = _list_insert_builtin(l, pos, value, builtin_start, builtin_n, sorted);
            break;
        case LIST_INSERT_LIST:
            rv = _list_insert_list(l, pos, (List *) value, (DLLNode *) l_start, (DLLNode *) l_end, sorted);
            break;
    }
    return rv;
}

DLLNode *list_erase(List *l, DLLNode *first, DLLNode *last) {
    if (!first || !l->front || (first == last)) {
        return LIST_ERROR;
    }

    DLLNode *before = first->prev;
    DLLNode *tmp;

    while (first != last) {
        tmp = first->next;
        list_rm(l, first);
        free(first);
        first = tmp;
        l->size--;
    }

    if (before) {
        before->next = last;
    } else {
        l->front = last;
    }

    DLLNode *res;

    if (last) {
        res = last;
        last->prev = before;
    } else {
        res = LIST_END;
        l->back = before;
    }
    return res;
}

/* ------------------------------------------------------------------------- */
/*  List utility functions    */
/* ------------------------------------------------------------------------- */

void list_reverse(List *l) {
    DLLNode *newFront = l->back;
    DLLNode *newBack = l->front;
    DLLNode *prev = NULL;
    DLLNode *curr = l->front;
    DLLNode *next = NULL;

    while (curr) {
        prev = curr->prev;
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        curr = next;
    }
    l->front = newFront;
    l->back = newBack;
}

void list_sort(List *l) {
    if (l->front == l->back || !l->helper.cmp) {
        return;
    } else if (l->size == 2 && (l->helper.cmp(l->front->data, l->back->data) > 0)) {
        DLLNode *temp = l->back;
        l->front = l->back;
        l->back = temp;
        l->front->prev = l->back->next = NULL;
        l->front->next = l->back;
        l->back->prev = l->front;
        return;
    }

    List *carry = list_new(&(l->helper), LIST_INIT_EMPTY);
    List *tmp = malloc(64 * sizeof(List));
    if (!tmp) DS_OOM();

    for (size_t i = 0; i < 64; ++i) {
        tmp[i].size = 0;
        tmp[i].back = tmp[i].front = tmp[i].curr = NULL;
        tmp[i].helper = l->helper;
    }

    List *fill = tmp;
    List *counter;

    do {
        list_splice(carry, carry->front, l, LIST_SPLICE_SINGLE, l->front);

        for (counter = tmp; counter != fill && !(list_empty(counter)); ++counter) {
            list_merge(counter, carry);
            _list_swap(carry, counter);
        }

        _list_swap(carry, counter);
        if (counter == fill) {
            ++fill;
        }
    } while (!(list_empty(l)));

    for (counter = tmp + 1; counter != fill; ++counter) {
        list_merge(counter, (counter - 1));
    }

    _list_swap(l, (fill - 1));
    list_free(carry);
    free(tmp);
}

void _list_removal_ops(List *l, void *val, meetsCondition cond) {
    if (!(l->front)) {
        return;
    }

    ListRemovalOp op = (!val && !cond) ? LIST_RM_OP_UNIQUE : ((!cond) ? LIST_RM_OP_VALUE : LIST_RM_OP_COND);

    DLLNode *curr = (op == LIST_RM_OP_UNIQUE) ? l->front->next : l->front;
    DLLNode *prev = (op == LIST_RM_OP_UNIQUE) ? l->front : NULL;
    DLLNode *next;
    int res;

    while (curr) {
        next = curr->next;

        switch (op) {
        case LIST_RM_OP_UNIQUE:
            res = !(l->helper.cmp(prev->data, curr->data));
            break;
        case LIST_RM_OP_VALUE:
            res = !(l->helper.cmp(val, curr->data));
            break;
        default:
            res = cond(curr->data);
            break;
        }

        if (res) {
            if (prev) {
                prev->next = next;
            } else {
                l->front = next;
            }

            if (next) {
                next->prev = prev;
            } else {
                l->back = prev;
            }

            list_rm(l, curr);
            free(curr);
            l->size--;
            curr = next;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    l->back = prev;
}

DLLNode *list_find(List *l, void *val) {
    DLLNode *curr = l->front;
    while (curr) {
        if (l->helper.cmp(curr->data, val) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

List *list_sublist(List *this, DLLNode *first, DLLNode *last) {
    if (!this->front || !first || (first == last)) {
        return NULL;
    }

    List *sub = list_new(&(this->helper), LIST_INIT_EMPTY);

    while (first != last) {
        list_push_back(sub, first->data);
        first = first->next;
    }
    return sub;
}

void list_merge(List *this, List *other) {
    if (!other || !other->front) { /* nothing to merge */
        return;
    } else if (!this->front) { /* "this" is empty, set it to other and return */
        this->front = other->front;
        this->back = other->back;
        this->size = other->size;
        other->front = other->back = NULL;
        other->size = 0;
        return;
    }

    DLLNode *first1 = this->front;
    DLLNode *last1 = NULL;
    DLLNode *first2 = other->front;
    DLLNode *last2 = NULL;

    while (first1 != last1 && first2 != last2) {
        if (this->helper.cmp(first2->data, first1->data) < 0) {
            DLLNode *next = first2->next;
            DLLNode *prev = first1->prev;
            if (prev) {
                prev->next = first2;
            } else {
                this->front = first2;
            }
            first2->prev = prev;
            first2->next = first1;
            first1->prev = first2;
            first2 = next;
        } else {
            first1 = first1->next;
        }
    }

    if (first2 != last2) { /* still elements in first2, but have reached end of first1 */
        first2->prev = this->back;
        this->back->next = first2;
        this->back = other->back;
    }
    this->size += other->size;
    other->front = other->back = NULL;
    other->size = 0;
}

void list_splice(List *this, DLLNode *position, List *other, ListSpliceType type, ...) {
    if (!other->front) return;
    else if (type == LIST_SPLICE_ALL) {
        _list_transfer_all(this, position, other);
        return;
    }

    void *first = NULL;
    void *last = NULL;

    va_list args;
    va_start(args, type);

    first = va_arg(args, void *);

    if (type == LIST_SPLICE_RANGE) {
        last = va_arg(args, void *);
    }

    va_end(args);

    switch (type) {
        case LIST_SPLICE_SINGLE:
            _list_transfer_single(this, position, other, (DLLNode *) first);
            break;
        default:
            _list_transfer_range(this, position, other, (DLLNode *) first, (DLLNode *) last);
            break;
    }
}

/* ------------------------------------------------------------------------- */
/*  va_args functions to handle initialization and insertion  */
/* ------------------------------------------------------------------------- */

DLLNode *_list_insert_elem(List *l, DLLNode *pos, void *value, bool before) {
    if (!value) {
        return LIST_ERROR;
    } else if (!pos) {
        list_push_back(l, value);
        return l->back;
    }

    DLLNode *prev = pos->prev, *next = pos->next;
    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    if (before) {
        new->next = pos;
        pos->prev = new;
        new->prev = prev;
        if (prev) {
            prev->next = new;
        } else {
            l->front = new;
        }
    } else {
        pos->next = new;
        new->prev = pos;
        new->next = next;
        if (next) {
            next->prev = new;
        } else {
            l->back = new;
        }
    }
    l->size++;
    return new;
}

DLLNode *_list_insert_elem_sorted(List *l, void *value) {
    if (!value) {
        return LIST_ERROR;
    }
    DLLNode *curr = l->front;

    if (!curr || l->helper.cmp(value, curr->data) <= 0) {
        list_push_front(l, value);
        return l->front;
    } else {
        int res = 0;
        DLLNode *prev = l->front;
        curr = curr->next;
        while (curr != NULL) {
            res = l->helper.cmp(value, curr->data);
            if ((res == 0) || ((res < 0) && (l->helper.cmp(value, prev->data) > 0))) {
                return _list_insert_elem(l, curr, value, true);
            }
            prev = prev->next;
            curr = curr->next;
        }
        list_push_back(l, value);
        return l->back;
    }
}

DLLNode *_list_insert_builtin(List *l, DLLNode *pos, void *arr, int start, int n, bool sorted) {
    if (!arr || !n) {
        return LIST_ERROR;
    }

    char *ptr = (char *) arr + (start * l->helper.size);
    DLLNode *rv = LIST_END; /* ListEntry where first element from arr was inserted */
    int endIdx = start + n;
    int i = start;

    if (sorted) {
        for (; i < endIdx; ++i) {
            _list_insert_elem_sorted(l, ptr);
            ptr += l->helper.size;
        }
        rv = l->front;
    } else {
        pos = _list_insert_elem(l, pos, ptr, true);
        ptr += l->helper.size;
        rv = pos;

        for (++i; i < endIdx; ++i) {
            pos = _list_insert_elem(l, pos, ptr, false);
            ptr += l->helper.size;
        }
    }
    return rv;
}

DLLNode *_list_insert_list(List *l, DLLNode *pos, List *other, DLLNode *start, DLLNode *end, bool sorted) {
    if (!other || !(other->front)) {
        return LIST_ERROR;
    } else if (!start) {
        start = other->front;
    }

    DLLNode *curr = start;
    DLLNode *rv = NULL;

    if (sorted) {
        while (curr != end) {
            _list_insert_elem_sorted(l, curr);
            curr = curr->next;
        }
        rv = l->front;
    } else {
        pos = _list_insert_elem(l, pos, curr->data, true);
        rv = pos;
        curr = curr->next;

        while (curr != end) {
            pos = _list_insert_elem(l, pos, curr->data, false);
            curr = curr->next;
        }
    }
    return rv;
}

void _list_transfer_all(List *this, DLLNode *position, List *other) {
    if (!this->front) {
        this->front = other->front;
        this->back = other->back;
    } else if (!position) {
        this->back->next = other->front;
        other->front->prev = this->back;
    } else {
        DLLNode *prev = position->prev;        
        other->front->prev = prev;
        other->back->next = position;
        position->prev = other->back;

        if (prev) {
            prev->next = other->front;
        } else {
            this->front = other->front;
        }
    }
    this->size += other->size;
    other->size = 0;
    other->front = other->back = NULL;
}

void _list_transfer_single(List *this, DLLNode *position, List *other, DLLNode *e) {
    if (!e) return;
    DLLNode *eprev = e->prev;
    DLLNode *enext = e->next;
    if (!this->front) {
        this->front = this->back = e;
        this->front->prev = this->back->next = NULL;
    } else if (!position) {
        this->back->next = e;
        e->prev = this->back;
        this->back = e;
        this->back->next = NULL;
    } else {
        DLLNode *prev = position->prev;
        e->prev = prev;
        e->next = position;
        position->prev = e;
        if (prev) {
            prev->next = e;
        } else {
            this->front = e;
        }
    }

    if (eprev) {
        eprev->next = enext;
    } else {
        other->front = enext;
    }

    if (enext) {
        enext->prev = eprev;
    } else {
        other->back = eprev;
    }
    other->size--;
    this->size++;
}

void _list_transfer_range(List *this, DLLNode *position, List *other, DLLNode *first, DLLNode *last) {
    if (!first || first == last) return;
    DLLNode *firstprev = first->prev;
    size_t count = 0;

    /* get number of elements */
    DLLNode *curr = first;
    while (curr != last) {
        ++count;
        curr = curr->next;
    }

    if (!this->front) {
        this->front = first;
        this->back = last ? last->prev : other->back;
        this->front->prev = this->back->next = NULL;
    } else if (!position) { /* append */
        this->back->next = first;
        first->prev = this->back;
        this->back = last ? last->prev : other->back;
        this->back->next = NULL;
    } else {
        DLLNode *prev = position->prev;
        first->prev = prev;
        position->prev = last ? last->prev : other->back;
        position->prev->next = position;
        if (prev) {
            prev->next = first;
        } else {
            this->front = first;
        }
    }

    if (firstprev) {
        firstprev->next = last;
    } else {
        other->front = last;
    }

    if (last) {
        last->prev = firstprev;
    } else {
        other->back = firstprev;
    }
    this->size += count;
    other->size -= count;
}
