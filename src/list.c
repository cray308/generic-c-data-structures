#include "defaults.h"
#include "list.h"

int list_length(DLLNode *curr);
void merge(DLLNode **leftStart, DLLNode **leftEnd, DLLNode **rightStart, DLLNode **rightEnd, comparison cmp);
void mergeSort(DLLNode **front, comparison cmp);
void _list_init_builtin(List *l, void *arr, int n);
void _list_init_list(List *l, List *other);
DLLNode *_list_insert_elem(List *l, DLLNode *pos, void *value, bool sorted);
DLLNode *_list_insert_builtin(List *l, DLLNode *pos, void *arr, int start, int n, bool sorted);
DLLNode *_list_insert_list(List *l, DLLNode *pos, List *other, DLLNode *start, DLLNode *end, bool sorted);
DLLNode *_list_insert_elem_sorted(List *l, void *value);
DLLNode *_list_insert_after(List *l, DLLNode *pos, void *value);
DLLNode *dll_node_new(size_t size);

// *****************************************************************
// helper functions

inline void list_copy(List *l, DLLNode *n, const void *value) {
    if (l->helper.copy) {
        l->helper.copy(n->data, value);
    } else {
        memcpy(n->data, value, l->helper.size);
    }
}

inline void list_rm(List *l, DLLNode *n) {
    if (l->helper.del) {
        l->helper.del(n->data);
    }
}

DLLNode *dll_node_new(size_t size) {
    size_t bytes = sizeof(DLLNode) + size;
    DLLNode *node = malloc(bytes);
    if (!node) {
        DS_OOM();
    }
    memset(node, 0, bytes);
    return node;
}

// *****************************************************************************************
// va_args functions to handle initialization and insertion

void _list_init_builtin(List *l, void *arr, int n) {
    if (!arr || !n) {
        return;
    }

    char *ptr = (char *) arr;
    for (int i = 0; i < n; ++i) {
        list_push_back(l, ptr);
        ptr += l->helper.size;
    }
}

void _list_init_list(List *l, List *other) {
    if (!other || !other->size) {
        return;
    }

    DLLNode *curr = other->front;
    while (curr) {
        list_push_back(l, curr->data);
        curr = curr->next;
    }
}

DLLNode *_list_insert_elem(List *l, DLLNode *pos, void *value, bool sorted) {
    if (!value) {
        return LIST_ERROR;
    } else if (sorted) {
        return _list_insert_elem_sorted(l, value);
    } else if (!pos || pos == LIST_END) {
        list_push_back(l, value);
        return l->back;
    }

    DLLNode *prev = pos->prev;
    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    new->next = pos;
    pos->prev = new;
    new->prev = prev;

    if (prev) {
        prev->next = new;
    } else {
        l->front = new;
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
                return _list_insert_elem(l, curr, value, false);
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
    DLLNode *rv = LIST_END; // ListEntry where first element from arr was inserted
    int endIdx = start + n;
    int i = start;

    if (sorted) {
        for (; i < endIdx; ++i) {
            _list_insert_elem_sorted(l, ptr);
            ptr += l->helper.size;
        }
        rv = l->front;
    } else {
        pos = _list_insert_elem(l, pos, ptr, false);
        ptr += l->helper.size;
        rv = pos;

        for (++i; i < endIdx; ++i) {
            _list_insert_after(l, pos, ptr);
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
    } else if (end && end == LIST_END) {
        end = NULL;
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
        pos = _list_insert_elem(l, pos, curr->data, false);
        rv = pos;
        curr = curr->next;

        while (curr != end) {
            pos = _list_insert_after(l, pos, curr->data);
            curr = curr->next;
        }
    }
    return rv;
}

DLLNode *_list_insert_after(List *l, DLLNode *pos, void *value) {
    if (!value) {
        return LIST_ERROR;
    } else if (!pos) {
        list_push_back(l, value);
        return l->back;
    }

    DLLNode *next = pos->next;
    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    pos->next = new;
    new->prev = pos;
    new->next = next;

    if (next) {
        next->prev = new;
    } else {
        l->back = new;
    }
    l->size++;
    return new;
}

// *****************************************************************************************
// main list functions

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

    if (init == LIST_INIT_EMPTY) { // nothing more to do in this case
        return l;
    }

    int n;
    void *other;

    // parse arguments
    va_list args;
    va_start(args, init);

    other = va_arg(args, void *);

    if (init == LIST_INIT_BUILTIN) {
        n = va_arg(args, int);
    }

    va_end(args);

    if (init == LIST_INIT_BUILTIN) {
        _list_init_builtin(l, other, n);
    } else {
        _list_init_list(l, (List *) other);
    }
    return l;
}

void list_free(List *l) {
    list_clear(l);
    free(l);
}

void list_push_front(List *l, const void *value) {
    if (!value) {
        return;
    }

    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    if (!(l->front)) {
        l->front = new;
        l->back = new;
    } else {
        new->next = l->front;
        l->front->prev = new;
        l->front = new;
    }
    l->size++;
}

void list_push_back(List *l, const void *value) {
    if (!value) {
        return;
    }

    DLLNode *new = dll_node_new(l->helper.size);
    list_copy(l, new, value);

    if (!(l->back)) {
        l->front = new;
        l->back = new;
    } else {
        new->prev = l->back;
        l->back->next = new;
        l->back = new;
    }
    l->size++;
}

void list_pop_front(List *l) {
    if (!(l->front)) {
        return;
    }

    DLLNode *front = l->front;
    l->front = front->next;
    if (l->front) {
        l->front->prev = NULL;
    } else {
        l->back = NULL;
    }

    list_rm(l, front);
    free(front);
    l->size--;
}

void list_pop_back(List *l) {
    if (!(l->back)) {
        return;
    }

    DLLNode *back = l->back;
    l->back = back->prev;
    if (l->back) {
        l->back->next = NULL;
    } else {
        l->front = NULL;
    }

    list_rm(l, back);
    free(back);
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
            rv = _list_insert_elem(l, pos, (DLLNode *)value, sorted);
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
    if (!first) {
        return LIST_ERROR;
    }

    DLLNode *before = first->prev;
    DLLNode *curr = first;
    DLLNode *tmp;

    while (curr != last) {
        tmp = curr->next;
        list_rm(l, curr);
        free(curr);
        curr = tmp;
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

void list_clear(List *l) {
    DLLNode *curr = l->front;
    DLLNode *temp = NULL;

    while (curr != NULL) {
        temp = curr->next;
        list_rm(l, curr);
        free(curr);
        curr = temp;
    }
    l->front = NULL;
    l->back = NULL;
    l->size = 0;
}

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
    if (l->front == l->back) {
        return;
    }

    mergeSort(&(l->front), l->helper.cmp);
    l->front->prev = NULL;
    DLLNode *prev = l->front;
    DLLNode *curr = prev->next;

    while (curr) {
        curr->prev = prev;
        prev = curr;
        curr = curr->next;
    }
    l->back = prev;
    l->back->next = NULL;
}

void list_unique(List *l) {
    if (l->size <= 1) {
        return;
    }

    DLLNode *prev = l->front;
    DLLNode *curr = prev->next;
    DLLNode *next;

    while (curr) {
        next = curr->next;

        if (l->helper.cmp(prev->data, curr->data) == 0) {
            prev->next = next;
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

void list_remove_value(List *l, void *val) {
    if (!(l->front)) {
        return;
    }

    DLLNode *curr = l->front;
    DLLNode *prev = curr->prev;
    DLLNode *next;

    while (curr) {
        next = curr->next;
        if (l->helper.cmp(val, curr->data) == 0) {
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
            curr = next;
        }
    }
    l->back = prev;
}

void list_remove_if(List *l, meetsCondition condition) {
    if (!(l->front)) {
        return;
    }

    DLLNode *curr = l->front;
    DLLNode *prev = curr->prev;
    DLLNode *next;

    while (curr) {
        next = curr->next;
        if (condition(curr->data)) {
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
            curr = next;
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

void mergeSort(DLLNode **front, comparison cmp) {
    if (*front == NULL || (*front)->next == NULL) {
        return;
    }

    DLLNode *leftStart = NULL, *leftEnd = NULL;
    DLLNode *rightStart = NULL, *rightEnd = NULL;
    DLLNode* prevend = NULL;
    int len = list_length(*front);
  
    for (int gap = 1; gap < len; gap <<= 1) {
        leftStart = *front;

        while (leftStart) {
            bool isFirstIter = false;
            if (leftStart == *front) {
                isFirstIter = true;
            }
  
            // First part for merging
            int counter = gap;
            leftEnd = leftStart;
            while (--counter && leftEnd->next) {
                leftEnd = leftEnd->next;
            }
  
            // Second part for merging
            rightStart = leftEnd->next;
            if (!rightStart) {
                break;
            }

            counter = gap;
            rightEnd = rightStart;
            while (--counter && rightEnd->next) {
                rightEnd = rightEnd->next;
            }
  
            // To store for next iteration.
            DLLNode *temp = rightEnd->next;
  
            // Merging two parts. 
            merge(&leftStart, &leftEnd, &rightStart, &rightEnd, cmp); 
  
            // Update head for first iteration, else
            // append after previous list
            if (isFirstIter) {
                *front = leftStart;
            } else {
                prevend->next = leftStart;
            }
            prevend = rightEnd;
            leftStart = temp;
        }
        prevend->next = leftStart;
    }
}

int list_length(DLLNode *curr) {
    int count = 0;
    while (curr != NULL) {
        ++count;
        curr = curr->next;
    }
    return count;
}

void merge(DLLNode **leftStart, DLLNode **leftEnd, DLLNode **rightStart, DLLNode **rightEnd, comparison cmp) {
    DLLNode *temp = NULL;

    if (cmp((*leftStart)->data, (*rightStart)->data) > 0) {
        // swap
        DLLNode *s = *leftStart;
        *leftStart = *rightStart;
        *rightStart = s;

        s = *leftEnd;
        *leftEnd = *rightEnd;
        *rightEnd = s;
    }

    // Merging remaining nodes 
    DLLNode* astart = *leftStart, *aend = *leftEnd;
    DLLNode* bstart = *rightStart;
    DLLNode* bendnext = (*rightEnd)->next;

    while (astart != aend && bstart != bendnext) {
        if (cmp(astart->next->data, bstart->data) > 0) {
            temp = bstart->next;
            bstart->next = astart->next;
            astart->next = bstart;
            bstart = temp;
        }
        astart = astart->next;
    }

    if (astart == aend) {
        astart->next = bstart;
    } else {
        *rightEnd = *leftEnd;
    }
}
