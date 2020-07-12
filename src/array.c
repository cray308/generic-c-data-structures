#include "array.h"
#include <stdbool.h>
#include <stdarg.h>

/* Internal macros  */

#define INITIAL_CAPACITY 8
#define arr_offset(a,i) ((a)->helper.size * (i))
#define arr_idx(a,i) ((void *) ((a)->arr + arr_offset((a),(i))))

int _arr_insert_elem(Array *a, int index, void *value);
int _arr_insert_builtin(Array *a, int index, void *arr, int start, int nelem);
int _arr_insert_array(Array *a, int index, Array *other, int start, int nelem);

/*  --------------------------------------------  */
/* copy and delete functions for brevity */
/*  --------------------------------------------  */

#define arr_copy(a, index, value) \
    do { \
        if ((a)->helper.copy) { \
            (a)->helper.copy(arr_idx((a), (index)), (value)); \
        } else { \
            memcpy(arr_idx((a), (index)), (value), (a)->helper.size); \
        } \
    } while(0)

#define arr_rm(a, index) \
    do { \
        if ((a)->helper.del) { \
            (a)->helper.del(arr_idx((a), (index))); \
        } \
    } while(0)

/*  --------------------------------------------  */
/* va_args functions to handle initialization and insertion */
/*  --------------------------------------------  */

int _arr_insert_elem(Array *a, int index, void *value) {
    if (!value) { /* check invalid arguments */
        return ARRAY_ERROR;
    } else if (!a->size || index >= (int) a->size) { /* append */
        array_push_back(a, value);
        return a->size - 1;
    }

    index = modulo(index, a->size); /* account for possible negative index */
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

int _arr_insert_builtin(Array *a, int index, void *arr, int start, int n) {
    if (!arr || !n) {
        return ARRAY_ERROR;
    }

    bool append = (!a->size || index >= (int) a->size);
    if (!append) {
        index = modulo(index, a->size);
        if (index < 0) {
            return ARRAY_ERROR;
        }
    }

    char *ptr = (char *) arr + (start * a->helper.size);

    int res; /* index where first element from other was inserted */
    int currIdx;
    int endIdx = start + n;
    int i = start;

    if (append) { /* append to a */
        array_push_back(a, ptr);
        ptr += a->helper.size;
        res = (int) a->size - 1;

        for (++i; i < endIdx; ++i) {
            array_push_back(a, ptr);
            ptr += a->helper.size;
        }
    } else { /* insert one after another, starting at index */
        currIdx = index;
        res = _arr_insert_elem(a, currIdx++, ptr);
        ptr += a->helper.size;

        for (++i; i < endIdx; ++i) {
            _arr_insert_elem(a, currIdx++, ptr);
            ptr += a->helper.size;
        }
    }

    return res;
}

int _arr_insert_array(Array *a, int index, Array *other, int start, int n) {
    if (!other || other->size == 0 || !n) {
        return ARRAY_ERROR;
    }

    bool append = (!a->size || index >= (int) a->size);
    if (!append) {
        index = modulo(index, a->size);
        if (index < 0) {
            return ARRAY_ERROR;
        }
    }

    start = modulo(start, other->size);
    if (start < 0) {
        return ARRAY_ERROR;
    }

    if (n < 0) { /* insert from first until end of other array */
        n = (int) other->size - start;
    } else {
        n = min(n, (int) other->size - start);
    }

    int res; /* index where first element from other was inserted */
    int currIdx;
    int endIdx = start + n;
    int i = start;

    if (append) { /* append to a */
        array_push_back(a, arr_idx(other, i));
        res = (int) a->size - 1;

        for (++i; i < endIdx; ++i) {
            array_push_back(a, arr_idx(other, i));
        }
    } else { /* insert one after another, starting at index */
        currIdx = index;
        res = _arr_insert_elem(a, currIdx++, arr_idx(other, i));

        for (++i; i < endIdx; ++i) {
            _arr_insert_elem(a, currIdx++, arr_idx(other, i));
        }
    }

    return res;
}

/*  --------------------------------------------  */
/* main array functions */
/*  --------------------------------------------  */

Array *array_new(const DSHelper *helper, ArrayInitializer init, ...) {
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
    if (init == ARR_INIT_EMPTY) { /* nothing more to do in this case */
        return a;
    }

    int n;
    void *value;

    /* parse arguments */
    va_list args;
    va_start(args, init);

    if (init == ARR_INIT_SIZE) {
        n = va_arg(args, int);
        value = va_arg(args, void *);
    } else {
        value = va_arg(args, void *);
        if (init == ARR_INIT_BUILTIN) {
            n = va_arg(args, int);
        }
    }

    va_end(args);

    switch (init) {
        case ARR_INIT_SIZE:
            array_resize(a, n, value);
            break;
        case ARR_INIT_BUILTIN:
            _arr_insert_builtin(a, 0, value, 0, n);
            break;
        default: /* ARR_INIT_ARRAY */
            _arr_insert_array(a, 0, (Array *) value, 0, ((Array *) value)->size);
            break;
    }
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
    size_t i;
    if (n <= a->size) { /* shrink array */
        if (a->helper.del) {
            for (i = n; i < a->size; ++i) {
                a->helper.del(arr_idx(a,i));
            }
        }
    } else { /* expand array */
        array_reserve(a, n);

        if (value) {
            for (i = a->size; i < n; ++i) {
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
    if (n <= a->capacity) { /* don't realloc smaller than the current capacity */
        return;
    }
    size_t ncap = a->capacity ? a->capacity : 1;
    while (ncap < n) {
        ncap <<= 1; /* use multiple of 2 */
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
    array_reserve(a, a->size + 1); /* allocate more space */
    arr_copy(a, a->size, e);
    a->size++;
}

int array_insert(Array *a, int index, ArrayInsertType type, ...) {
    int n;
    int start;
    void *other;

    /* parse arguments */
    va_list args;
    va_start(args, type);

    other = va_arg(args, void *);

    if (type != ARR_INSERT_SINGLE) {
        start = va_arg(args, int);
        n = va_arg(args, int);
    }

    va_end(args);

    int rv = 0;

    switch (type) {
        case ARR_INSERT_SINGLE:
            rv = _arr_insert_elem(a, index, other);
            break;
        case ARR_INSERT_BUILTIN:
            rv = _arr_insert_builtin(a, index, other, start, n);
            break;
        case ARR_INSERT_ARRAY:
            rv = _arr_insert_array(a, index, (Array *) other, start, n);
            break;
    }

    return rv;
}

void array_pop_back(Array *a) {
    if (!(a->size)) {
        return;
    }
    arr_rm(a, a->size - 1);
    a->size--;
}

int array_erase(Array *a, int first, int nelem) {
    if (!nelem || a->size == 0) {
        return ARRAY_ERROR;
    }

    first = modulo(first, a->size);
    if (first < 0) {
        return ARRAY_ERROR;
    }

    if (nelem < 0) { /* erase from first to end of array */
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

    if (endIdx < (int) a->size) { /* move elements from endIdx onward back to first */
        memmove(arr_idx(a, first), arr_idx(a, endIdx), a->helper.size * (a->size - endIdx));
        res = first;
    } else {
        res = ARRAY_END(a) - nelem;
    }
    a->size -= (size_t) nelem;
    return res;
}

void array_shrink_to_fit(Array *a) {
    if (a->size == a->capacity || a->size == 0) { /* can't shrink any more */
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
    if (a->size <= 1) { /* nothing to sort */
        return;
    }
    qsort(a->arr, a->size, a->helper.size, a->helper.cmp);
}

void *array_find(Array *a, const void *key) {
    if (a->size == 0) { /* nothing to find */
        return NULL;
    } else if (a->size == 1) { /* check it here rather than calling bsearch */
        if (a->helper.cmp(array_at(a, 0), key) == 0) {
            return array_at(a, 0);
        }
        return NULL;
    }
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

    Array *sub = array_new(&(a->helper), ARR_INIT_EMPTY);
    int end;
    int i;

    if (step_size < 0) {
        end = (n < 0) ? -1 : max(-1, start + (n * step_size));
        for (i = start; i > end; i += step_size) {
            array_push_back(sub, arr_idx(a, i));
        }
    } else {
        end = (n < 0) ? ARRAY_END(a) : min(ARRAY_END(a), start + (n * step_size));
        for (i = start; i < end; i += step_size) {
            array_push_back(sub, arr_idx(a, i));
        }
    }
    return sub;
}

void vec_2d_helper_copy(void *_dst, const void *_src) {
    Array *dst = (Array *) _dst;
    Array *src = (Array *) _src;
    if (!(src->helper.size)) {
        fputs("DSHelper provided with a size of 0 in vec_2d_helper_copy.", stderr);
        exit(1);
    }
    dst->size = dst->capacity = 0;
    dst->arr = NULL;
    dst->helper = src->helper;
    array_reserve(dst, INITIAL_CAPACITY);
}

void vec_2d_helper_del(void *_elem) {
    Array *a = (Array *) _elem;
    if (a) {
        array_clear(a);
        free(a->arr);
    }
}
