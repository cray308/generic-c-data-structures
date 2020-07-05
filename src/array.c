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


inline void arr_copy(Array *a, int index, const void *value) {
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
    size_t ncap = a->capacity ? a->capacity : 1;
    while (ncap < n) {
        ncap <<= 1; // use multiple of 2
    }

    char *tmp = realloc(a->arr, ncap * a->helper.size);
    if (!tmp) {
        DS_OOM();
    }
    a->capacity = ncap;
    a->arr = tmp;
}

void array_push_back(Array *a, void *e) {
    if (!e) return;
    array_reserve(a, a->size + 1); // allocate more space
    arr_copy(a, a->size++, e);
}

int array_insert(Array *a, int index, void *value) {
    if (!value) { // check invalid arguments
        return ARRAY_ERROR;
    }

    if (!a->size || index >= (int) a->size) { // append
        array_push_back(a, value);
        return a->size - 1;
    }

    index = modulo(index, a->size); // account for possible negative index
    printf("index: %d\n", index);
    if (index < 0) {
        return ARRAY_ERROR;
    }

    array_reserve(a, a->size + 1);
    size_t nBytes = a->helper.size * ((int) a->size - index);
    memmove(arr_idx(a, index + 1), arr_idx(a, index), nBytes);
    arr_copy(a, index, value);
    a->size++;
    return index;
}

int array_insert_arr(Array *a, int index, Array *other, int first, int nelem) {
    if (!other || other->size == 0 || !nelem) {
        return ARRAY_ERROR;
    }

    bool append = (!a->size || index >= (int) a->size);
    if (!append) {
        index = modulo(index, a->size);
        if (index < 0) {
            return ARRAY_ERROR;
        }
    }

    first = modulo(first, other->size);
    if (first < 0) {
        return ARRAY_ERROR;
    }

    if (nelem < 0) { // insert from first until end of other array
        nelem = (int) other->size - first;
    } else {
        nelem = min(nelem, (int) other->size - first);
    }

    int res; // index where first element from other was inserted
    int currIdx;
    int endIdx = first + nelem;
    int i = first;

    if (append) { // append to a
        array_push_back(a, arr_idx(other, i));
        res = (int) a->size - 1;

        for (++i; i < endIdx; ++i) {
            array_push_back(a, arr_idx(other, i));
        }
    } else { // insert one after another, starting at index
        currIdx = index;
        res = array_insert(a, currIdx++, arr_idx(other, i));

        for (++i; i < endIdx; ++i) {
            array_insert(a, currIdx++, arr_idx(other, i));
        }
    }

    //array_erase(other, first, nelem);
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

int array_erase(Array *a, int first, int nelem) {
    if (!nelem || a->size == 0) {
        return ARRAY_ERROR;
    }

    first = modulo(first, a->size);
    if (first < 0) {
        return ARRAY_ERROR;
    }

    if (nelem < 0) { // erase from first to end of array
        nelem = (int) a->size - first;
    } else {
        nelem = min(nelem, (int) a->size - first);
    }

    int endIdx = first + nelem;
    int res;

    if (a->helper.del) {
        for (int i = first; i < endIdx; ++i) {
            a->helper.del(arr_idx(a, i));
        }
    }    

    if (endIdx < (int) a->size) { // move elements from endIdx onward back to first
        memmove(arr_idx(a, first), arr_idx(a, endIdx), a->helper.size * (a->size - endIdx));
        res = first;
    } else {
        res = (int) ARRAY_END(a) - nelem;
    }
    a->size -= (size_t) nelem;
    return res;
}

void array_shrink_to_fit(Array *a) {
    if (a->size == a->capacity || a->size == 0) { // can't shrink any more
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
    if (a->size <= 1) { // nothing to sort
        return;
    }
    qsort(a->arr, a->size, a->helper.size, a->helper.cmp);
}

void *array_find(Array *a, const void *key) {
    if (a->size == 0) { // nothing to find
        return NULL;
    } else if (a->size == 1) { // check it here rather than calling sort and bsearch
        if (a->helper.cmp(array_at(a, 0), key) == 0) {
            return array_at(a, 0);
        }
        return NULL;
    }
    array_sort(a);
    return bsearch(key, a->arr, a->size, a->helper.size, a->helper.cmp);
}

Array *array_subarr(Array *a, int start, int n, int step_size) {
    if (!a->size || !n) {
        return NULL;
    } else if (step_size == 0) {
        step_size = 1;
    }

    start = modulo(start, a->size);
    if (start < 0) {
        return NULL;
    }

    Array *sub = array_new(&(a->helper));
    int end;

    if (step_size < 0) {
        end = (n < 0) ? -1 : max(-1, start + (n * step_size));
    } else {
        end = (n < 0) ? ARRAY_END(a) : min(ARRAY_END(a), start + (n * step_size));
    }

    if (step_size < 0) {
        for (int i = start; i > end; i += step_size) {
            array_push_back(sub, arr_idx(a, i));
        }
    } else {
        for (int i = start; i < end; i += step_size) {
            array_push_back(sub, arr_idx(a, i));
        }
    }
    return sub;
}
