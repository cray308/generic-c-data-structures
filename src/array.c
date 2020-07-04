#include "defaults.h"
#include "array.h"

// Internal macros

#define INITIAL_CAPACITY 8
#define arr_offset(a,i) ((a)->helper.size * (i))
#define valid_idx(a,i) ((i) < (a)->size)
#define arr_idx(a,i) ((void *) ((a)->arr + arr_offset((a),(i))))
#define arr_find_idx(a,e) (((char*)(e) - (a)->arr) / (a)->helper.size)
#define arr_next(a,eptr) (((arr_find_idx(a,eptr) + 1) == (a)->size) ? NULL : arr_idx(a,(arr_find_idx(a,eptr) + 1)))
#define arr_prev(a,eptr) ((arr_find_idx(a,eptr) == 0) ? NULL : arr_idx(a,(arr_find_idx(a,eptr) - 1)))


inline void arr_copy(Array *a, size_t index, const void *value) {
    if (a->helper.copy) { // copy using function
        a->helper.copy(arr_idx(a, index), value);
    } else { // copy the raw bytes
        memcpy(arr_idx(a, index), value, a->helper.size);
    }
}

inline void arr_rm(Array *a, size_t index) {
    if (a->helper.del) { // call custom delete function if it's provided
        a->helper.del(arr_idx(a, index));
    }
}

Array *array_new(const DSHelper *helper) {
    if (!helper || helper->size == 0) {
        return NULL;
    }

    Array *a = malloc(sizeof(Array));
    if (!a) {
        DS_OOM();
    }

    memset(a, 0, sizeof(Array));
    a->helper = *helper;
    array_reserve(a, INITIAL_CAPACITY);
    return a;
}

void array_free(Array *a) {
    if (a->capacity) {
        array_clear(a);
        free(a->arr);
    }
    free(a);
}

void array_resize(Array *a, size_t n, void *value) {
    if (n <= a->size) { // shrink array
        if (a->helper.del) {
            for (size_t i = n; i < a->size; ++i) {
                a->helper.del(arr_idx(a,i));
            }
        }
    } else { // expand array
        array_reserve(a, n);

        if (value) {
            for (size_t i = a->size; i < n; ++i) {
                arr_copy(a, i, value);
            }
        } else {
            size_t nBytes = a->helper.size * (n - a->size);
            memset(arr_idx(a,a->size), 0, nBytes);
        }
    }
    a->size = n;
}

void array_reserve(Array *a, size_t n) {
    if (n <= a->capacity) { // don't realloc smaller than the current capacity
        return;
    }

    char *tmp = realloc(a->arr, n * a->helper.size);
    if (!tmp) {
        DS_OOM();
    }
    a->capacity = n;
    a->arr = tmp;
}

void array_push_back(Array *a, void *e) {
    if (a->size == a->capacity) { // allocate more space
        array_reserve(a, a->size << 1);
    }
    arr_copy(a, a->size++, e);
}

int array_insert(Array *a, size_t index, void *value) {
    if (!value) {
        return ARRAY_ERROR;
    } else if (array_empty(a) || index >= a->size) {
        array_push_back(a, value);
        return (int) a->size - 1;
    }

    array_reserve(a, a->size + 1);
    size_t nBytes = a->helper.size * (a->size - index);
    memmove(arr_idx(a, index + 1), arr_idx(a, index), nBytes);
    arr_copy(a, index, value);
    a->size++;
    return index;
}

int array_insert_arr(Array *a, size_t index, Array *other, size_t first, size_t nelem) {
    if (!other || (first > other->size) || !nelem) {
        return ARRAY_ERROR;
    }

    size_t currIdx = index;
    size_t endIdx = min(other->size, first + nelem);
    size_t i = first;

    int res = array_insert(a, currIdx++, array_at(other, i));
    size_t nCopied = 1;

    for (++i; i < endIdx; ++i) {
        array_insert(a, currIdx++, array_at(other, i));
        ++nCopied;
    }

    array_erase(other, first, nCopied);
    return res;
}

void array_pop_back(Array *a) {
    if (!(a->size)) {
        return;
    }
    arr_rm(a, a->size-- - 1);
}

void array_clear(Array *a) {
    if (a->helper.del) {
        for (size_t i = 0; i < a->size; ++i) {
            a->helper.del(arr_idx(a,i));
        }
    }
    a->size = 0;
}

int array_erase(Array *a, size_t first, size_t nelem) {
    if (!nelem || first >= a->size) {
        return ARRAY_ERROR;
    }

    size_t endIdx = min(a->size, first + nelem);
    nelem = endIdx - first;
    int res;

    if (a->helper.del) {
        for (size_t i = first; i < endIdx; ++i) {
            a->helper.del(arr_idx(a,i));
        }
    }    

    if (endIdx < a->size) {
        memmove(arr_idx(a,first), arr_idx(a,endIdx), a->helper.size * (a->size - endIdx));
        res = (int) first;
    } else {
        res = ARRAY_END;
    }
    a->size -= nelem;
    return res;
}

void array_shrink_to_fit(Array *a) {
    if (a->size == a->capacity || a->size == 0) {
        return;
    }

    char *tmp = realloc(a->arr, a->size * a->helper.size);
    if (!tmp) {
        DS_OOM();
    }
    a->capacity = a->size;
    a->arr = tmp;
}

void array_sort(Array *a) {
    if (a->size <= 1) {
        return;
    }
    qsort(a->arr, a->size, a->helper.size, a->helper.cmp);
}

void *array_find(Array *a, const void *key) {
    if (a->size == 0) {
        return NULL;
    } else if (a->size == 1) {
        if (a->helper.cmp(array_at(a, 0), key) == 0) {
            return array_at(a, 0);
        }
        return NULL;
    }
    array_sort(a);
    return bsearch(key, a->arr, a->size, a->helper.size, a->helper.cmp);
}
