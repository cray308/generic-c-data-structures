#ifndef ARRAY_H
#define ARRAY_H

#include "iterator.h"
#include "alg_helper.h"

#define ARRAY_ERROR (-1)

/* --------------------------------------------------------------------------
 * PRIMARY ARRAY SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at index `i`. Performs bounds checking, and if the index is out of bounds, 
 * returns NULL. Negative indices may be used.
 *
 * @param  i  The index in the array.
 */
#define array_at(id, this, i) array_at_##id(this, i)


/**
 * Direct access to the element at index `i`. Does NOT perform bounds checking, but faster than `array_at` 
 * for element access.
 *
 * @param  i  The index in the array.
 */
#define array_index(this, i) ((this)->arr[(i)])


/**
 * Pointer to the first element in the array, if it is is not empty.
 */
#define array_front(this) iter_begin(ARR, 0, (this)->arr, (this)->size)


/**
 * Pointer to the last element in the array, if it is is not empty.
 */
#define array_back(this) iter_rbegin(ARR, 0, (this)->arr, (this)->size)


/**
 * Macro for iterating over the array from front to back.
 *
 * @param  it  Pointer to the array's datatype (t *) which is assigned to the current element.
 *               May be dereferenced with (*it).
 */
#define array_iter(this, it) for (it = array_front(this); it != iter_end(ARR, 0, (this)->arr, (this)->size); iter_next(ARR, 0, it))


/**
 * Macro for iterating over the array in reverse (from back to front).
 *
 * @param  it  Pointer to the array's datatype (t *) which is assigned to the current element.
 *               May be dereferenced with (*it).
 */
#define array_riter(this, it) for (it = array_back(this); it != iter_rend(ARR, 0, (this)->arr, (this)->size); iter_prev(ARR, 0, it))


/**
 * The number of elements in the array.
 */
#define array_size(this) ((int) (this)->size)


/**
 * The capacity (maximum number of elements prior to resizing) of the array.
 */
#define array_capacity(this) ((int) (this)->capacity)


/**
 * Tests whether there are no elements in the array.
 */
#define array_empty(this) (!((this)->size))


/**
 * Creates a new, empty array.
 *
 * @return  Pointer to the newly created array.
 */
#define array_new(id) array_new_##id()


/**
 * Creates a new array with size `n`, where each index is set to `value`.
 *
 * @param   n      Number of elements to initialize.
 * @param   value  Value to set for each of the indices.
 *
 * @return         Pointer to the newly created array.
 */
#define array_new_repeatingValue(id, n, value) array_new_repeatingValue_##id(n, value)


/**
 * Creates a new array as a copy of a built-in array `arr` which has `n` elements.
 *
 * @param   arr   Pointer to the first element to insert. The `Array` type in this file can be used
 *                  by passing `array_at(id, a, index)`.
 * @param   n     Number of elements to include.
 *
 * @return        Pointer to the newly created array.
 */
#define array_new_fromArray(id, arr, n) array_new_fromArray_##id(arr, n)


/**
 * Creates a new array as a copy of `Array` object `other`.
 *
 * @return  Pointer to the newly created array.
 */
#define array_createCopy(id, other) array_new_fromArray_##id((other)->arr, (other)->size)


/**
 * Deletes all elements and frees the array.
 */
#define array_free(id, this) array_free_##id(this)


/**
 * Resizes the array to a size of `n`. If this is less than the current size, all but the first `n` 
 * elements are removed. If this is greater than the current size, elements are appended to the array 
 * with a value of 0.
 *
 * @param  n  The new array size.
 */
#define array_resize(id, this, n) array_resize_usingValue_##id(this, n, 0)


/**
 * Resizes the array to a size of `n`. If this is less than the current size, all but the first `n` 
 * elements are removed. If this is greater than the current size, elements are appended to the array 
 * with a value of `value`.
 *
 * @param  n      The new array size.
 * @param  value  Value to hold in the new indices if `n` is greater than the current size.
 */
#define array_resize_usingValue(id, this, n, value) array_resize_usingValue_##id(this, n, value)


/**
 * Requests a change in capacity to `n`. If this is smaller than the current capacity, nothing is 
 * done. If you wish to decrease the capacity, see `array_shrink_to_fit`.
 *
 * @param  n  The new capacity.
 */
#define array_reserve(id, this, n) array_reserve_##id(this, n)


/**
 * Appends `value` to the array.
 *
 * @param  value  Value to insert.
 */
#define array_push_back(id, this, value) array_push_back_##id(this, value)


/**
 * Inserts `value` before `index`. Any elements after this index will be shifted one position 
 * to the right. After insertion, the new element will be located at `index`.
 *
 * @param   index  Index before which the element will be inserted. If this is specified as
 *                   `array_size`, the element is appended.
 * @param   value  Value to insert.
 *
 * @return         The index where the element was inserted, or `ARRAY_ERROR` if there was an error.
 */
#define array_insert(id, this, index, value) array_insert_##id(this, index, value)


/**
 * Inserts `n` copies of `value` before `index`.
 *
 * @param   index   Index before which the elements should be inserted. If this is specified as
 *                   `array_size`, the elements are appended.
 * @param   n       Number of copies of `value` to insert.
 * @param   value   Value to insert.
 *
 * @return          The index where the first element was inserted, or `ARRAY_ERROR` if there was an error.
 */
#define array_insert_repeatingValue(id, this, index, n, value) array_insert_repeatingValue_##id(this, index, n, value)


/**
 * Inserts `n` new elements from `arr` before `index`. Any elements after this index will be shifted 
 * to the right. After insertion, the first of the new elements will be located at `index`.
 *
 * @param   index  Index before which the elements will be inserted. If this is specified as
 *                   `array_size`, the elements are appended.
 * @param   arr    Pointer to the first element to insert in a built-in array. The `Array` type in 
 *                   this file can be used by passing `array_at(id, a, index)`.
 * @param   n      Number of elements to insert from `arr`.
 *
 * @return         The index where the first element was inserted, or `ARRAY_ERROR` if there was an error.
 */
#define array_insert_fromArray(id, this, index, arr, n) array_insert_fromArray_##id(this, index, arr, n)


/**
 * Removes the last element, if the array is not empty.
 */
#define array_pop_back(id, this) array_pop_back_##id(this)


/**
 * Removes all elements, leaving the array with a size of 0.
 */
#define array_clear(id, this) array_clear_##id(this)


/**
 * Removes `nelem` elements from the array, starting at index `first`.
 *
 * @param  first  The first index to delete.
 * @param  nelem  The number of elements to delete. If this is -1, it means to erase all elements
 *                  from `first` to the end of the array.
 *
 * @return        The index after the last element to be deleted. If the last element to be deleted
 *                was the end of the array, returns `array_size`. If an error occurred, returns
 *                `ARRAY_ERROR`.
 */
#define array_erase(id, this, first, nelem) array_erase_##id(this, first, nelem)


/**
 * If the array's capacity is greater than its size, reallocates only enough memory to hold `array_size` 
 * number of elements. This should only be used if the array's capacity has grown to be much larger 
 * than its size.
 */
#define array_shrink_to_fit(id, this) array_shrink_to_fit_##id(this)


/**
 * Creates a subarray from this array with `n` elements, starting at index `start` and moving to the 
 * next element to include with a step size of `step_size`.
 *
 * @param   start      Index to start the subarray.
 * @param   n          Maximum number of elements to include in the new subarray. -1 implies to
 *                       include as many elements as the start and step size allow.
 * @param   step_size  How to adjust the index when copying elements. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return             A newly allocated subarray, or NULL if an error occurred.
 */
#define array_subarr(id, this, start, n, step_size) array_subarr_##id(this, start, n, step_size)


/**
 * Generates `Array` code for a specified type and ID.
 *
 * @param  id           ID to be used for the array (must be unique).
 * @param  t            Type to be stored in the array.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the array.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_array(id, t, copyValue, deleteValue)                                                             \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t capacity;                                                                                         \
    t *arr;                                                                                                  \
} Array_##id;                                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* array_at_##id(Array_##id *this, int i) {                                             \
    int size = array_size(this);                                                                             \
    __ds_adjust_index(i, size)                                                                               \
    return (i < 0) ? NULL : &(this->arr[i]);                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_new_##id(void) {                                                          \
    Array_##id *a;                                                                                           \
    __ds_malloc(a, sizeof(Array_##id))                                                                       \
    __ds_malloc(a->arr, 8 * sizeof(t))                                                                       \
    a->size = 0;                                                                                             \
    a->capacity = 8;                                                                                         \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void array_reserve_##id(Array_##id *this, size_t n) {                                       \
    size_t ncap;                                                                                             \
    t *tmp;                                                                                                  \
    if (n <= this->capacity) return;                                                                         \
                                                                                                             \
    ncap = this->capacity;                                                                                   \
    while (ncap < n) {                                                                                       \
        ncap <<= 1; /* use multiple of 2 */                                                                  \
    }                                                                                                        \
                                                                                                             \
    __ds_realloc(tmp, this->arr, ncap * sizeof(t))                                                           \
    this->capacity = ncap;                                                                                   \
    this->arr = tmp;                                                                                         \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_erase_##id(Array_##id *this, int first, int nelem) {                              \
    int endIdx, i, res, size = array_size(this);                                                             \
    __ds_adjust_index(first, size)                                                                           \
    if (!nelem || first < 0) return ARRAY_ERROR;                                                             \
                                                                                                             \
    if (nelem < 0) { /* erase from first to end of array */                                                  \
        nelem = size - first;                                                                                \
    } else {                                                                                                 \
        nelem = min(nelem, size - first);                                                                    \
    }                                                                                                        \
                                                                                                             \
    endIdx = first + nelem;                                                                                  \
    for (i = first; i < endIdx; ++i) {                                                                       \
        deleteValue(this->arr[i]);                                                                           \
    }                                                                                                        \
                                                                                                             \
    if (endIdx < size) { /* move elements from endIdx onward back to first */                                \
        memmove(&this->arr[first], &this->arr[endIdx], (this->size - (size_t) endIdx) * sizeof(t));          \
        res = first;                                                                                         \
    } else {                                                                                                 \
        res = size - nelem;                                                                                  \
    }                                                                                                        \
    this->size -= (size_t) nelem;                                                                            \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_insert_repeatingValue_##id(Array_##id *this, int index, size_t n, t value) {      \
    char append;                                                                                             \
    t* i; t* end;                                                                                            \
    int res, size = array_size(this);                                                                        \
    if (!n) return ARRAY_ERROR;                                                                              \
    else if (!(append = (index == size))) {                                                                  \
        __ds_adjust_index(index, size)                                                                       \
        if (index < 0) return ARRAY_ERROR;                                                                   \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(this, this->size + n);                                                                \
                                                                                                             \
    if (append) { /* append to a */                                                                          \
        res = size;                                                                                          \
    } else { /* insert in the middle of a */                                                                 \
        memmove(&this->arr[index + (int) n], &this->arr[index], (this->size - (size_t) index) * sizeof(t));  \
        res = index;                                                                                         \
    }                                                                                                        \
    end = &this->arr[res + (int) n];                                                                         \
    for (i = &this->arr[res]; i != end; ++i) {                                                               \
        copyValue((*i), value);                                                                              \
    }                                                                                                        \
    this->size += n;                                                                                         \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void array_resize_usingValue_##id(Array_##id *this, size_t n, t value) {                    \
    if (n == this->size) return;                                                                             \
    else if (n < this->size) {                                                                               \
        array_erase_##id(this, (int) n, (int)(this->size - n));                                              \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    array_insert_repeatingValue_##id(this, array_size(this), n - this->size, value);                         \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_new_repeatingValue_##id(size_t n, t value) {                              \
    Array_##id *a = array_new_##id();                                                                        \
    array_insert_repeatingValue_##id(a, 0, n, value);                                                        \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_push_back_##id(Array_##id *this, t value) {                                  \
    t *loc;                                                                                                  \
    array_reserve_##id(this, this->size + 1); /* allocate more space */                                      \
    loc = &this->arr[this->size];                                                                            \
    copyValue((*loc), (value));                                                                              \
    this->size++;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_insert_##id(Array_##id *this, int index, t element) {                             \
    int size = array_size(this);                                                                             \
    t *loc;                                                                                                  \
    if (index == size) { /* append */                                                                        \
        array_push_back_##id(this, element);                                                                 \
        return size;                                                                                         \
    }                                                                                                        \
    __ds_adjust_index(index, size)                                                                           \
    if (index < 0) return ARRAY_ERROR;                                                                       \
                                                                                                             \
    array_reserve_##id(this, this->size + 1);                                                                \
    memmove(this->arr + (index + 1), this->arr + index, (this->size - (size_t) index) * sizeof(t));          \
    loc = &this->arr[index];                                                                                 \
    copyValue((*loc), (element));                                                                            \
    this->size++;                                                                                            \
    return index;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX int array_insert_fromArray_##id(Array_##id *this, int index, t *arr, size_t n) {            \
    char append;                                                                                             \
    t* i; t* end;                                                                                            \
    int res, size = array_size(this);                                                                        \
    if (!(arr && n)) return ARRAY_ERROR;                                                                     \
    else if (!(append = (index == size))) {                                                                  \
        __ds_adjust_index(index, size)                                                                       \
        if (index < 0) return ARRAY_ERROR;                                                                   \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(this, this->size + n);                                                                \
                                                                                                             \
    if (append) { /* append to a */                                                                          \
        res = size;                                                                                          \
    } else { /* insert in the middle of a */                                                                 \
        memmove(&this->arr[index + (int) n], &this->arr[index], (this->size - (size_t) index) * sizeof(t));  \
        res = index;                                                                                         \
    }                                                                                                        \
    end = &this->arr[res + (int) n];                                                                         \
    for (i = &this->arr[res]; i != end; ++i, ++arr) {                                                        \
        copyValue((*i), (*arr));                                                                             \
    }                                                                                                        \
    this->size += n;                                                                                         \
    return res;                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_new_fromArray_##id(t *arr, size_t size) {                                 \
    Array_##id *a = array_new_##id();                                                                        \
    array_insert_fromArray_##id(a, 0, arr, size);                                                            \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_clear_##id(Array_##id *this) {                                               \
    array_erase_##id(this, 0, array_size(this));                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_free_##id(Array_##id *this) {                                                \
    if (this->capacity) {                                                                                    \
        array_clear_##id(this);                                                                              \
        free(this->arr);                                                                                     \
    }                                                                                                        \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_pop_back_##id(Array_##id *this) {                                            \
    if (!this->size) return;                                                                                 \
    deleteValue((this->arr[this->size - 1]));                                                                \
    this->size--;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void array_shrink_to_fit_##id(Array_##id *this) {                                       \
    t *tmp;                                                                                                  \
    if (this->capacity == 8 || this->size == this->capacity || this->size == 0) return;                      \
                                                                                                             \
    __ds_realloc(tmp, this->arr, this->size * sizeof(t))                                                     \
    this->capacity = this->size;                                                                             \
    this->arr = tmp;                                                                                         \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *array_subarr_##id(Array_##id *this, int start, int n, int step_size) {          \
    Array_##id *sub;                                                                                         \
    int end, i, size = array_size(this);                                                                     \
    __ds_adjust_index(start, size)                                                                           \
    if (!n || start < 0) return NULL;                                                                        \
                                                                                                             \
    if (step_size == 0) step_size = 1;                                                                       \
    sub = array_new_##id();                                                                                  \
                                                                                                             \
    if (step_size < 0) {                                                                                     \
        end = (n < 0) ? -1 : max(-1, start + (n * step_size));                                               \
        for (i = start; i > end; i += step_size) {                                                           \
            array_push_back_##id(sub, this->arr[i]);                                                         \
        }                                                                                                    \
    } else {                                                                                                 \
        end = (n < 0) ? size : min(size, start + (n * step_size));                                           \
        for (i = start; i < end; i += step_size) {                                                           \
            array_push_back_##id(sub, this->arr[i]);                                                         \
        }                                                                                                    \
    }                                                                                                        \
    return sub;                                                                                              \
}                                                                                                            \


/* --------------------------------------------------------------------------
 * MATRIX SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at row `row` and column `col`, similar to builtin_array[row][col]. 
 * Performs bounds checking, and if the row or column is out of bounds, returns NULL.
 *
 * @param  row  Matrix row.
 * @param  col  Matrix column.
 */
#define matrix_at(matid, this, row, col) matrix_at_##matid(this, row, col)


/**
 * Direct access to the element at row `row` and column `col`. Does not perform bounds checking.
 *
 * @param  row  Matrix row.
 * @param  col  Matrix column.
 */
#define matrix_index(this, row, col) ((this)->arr[(row)]->arr[(col)])


/**
 * Creates a new matrix of size (`rows` X `cols`).
 *
 * @param   rows  Number of rows in matrix.
 * @param   cols  Number of columns in matrix.
 *
 * @return        Pointer to newly allocated matrix.
 */
#define matrix_new(matid, rows, cols) matrix_new_##matid(rows, cols)


/**
 * Frees the matrix.
 */
#define matrix_free(matid, this) matrix_free_##matid(this)


/**
 * Generates code for a matrix (2D-array) that stores type `t`. `gen_array(id, t)` must have been 
 * called prior, because the matrix is an array of `Array_id`.
 *
 * @param  id  ID that was used in the prior call to `gen_array`.
 * @param  t   Type that was used in the prior call to `gen_array` (and thus the type stored) in the matrix.
 */
#define gen_matrix(id, t)                                                                                    \
gen_array(2d_##id, Array_##id *, DSDefault_shallowCopy, DSDefault_shallowDelete)                             \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* matrix_at_##id(Array_2d_##id *this, int row, int col) {                              \
    int nRows = array_size(this), nCols;                                                                     \
    __ds_adjust_index(row, nRows)                                                                            \
    if (row < 0) return NULL;                                                                                \
    nCols = (int) this->arr[row]->size;                                                                      \
    __ds_adjust_index(col, nCols)                                                                            \
    if (col < 0) return NULL;                                                                                \
    return &(this->arr[row]->arr[col]);                                                                      \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX Array_2d_##id *matrix_new_##id(size_t rows, size_t cols) {                                  \
    Array_2d_##id *m;                                                                                        \
    size_t i;                                                                                                \
    if (!(rows && cols)) return NULL;                                                                        \
    __ds_malloc(m, sizeof(Array_2d_##id))                                                                    \
    __ds_malloc(m->arr, sizeof(Array_##id *) * rows)                                                         \
    m->size = 0;                                                                                             \
    m->capacity = rows;                                                                                      \
    for (i = 0; i < rows; ++i) {                                                                             \
        Array_##id *row = array_new_repeatingValue_##id(cols, 0);                                            \
        array_push_back_2d_##id(m, row);                                                                     \
    }                                                                                                        \
    return m;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void matrix_free_##id(Array_2d_##id *this) {                                            \
    size_t i;                                                                                                \
    for (i = 0; i < this->size; ++i) {                                                                       \
        array_free_##id((this->arr[i]));                                                                     \
    }                                                                                                        \
    free(this->arr);                                                                                         \
    free(this);                                                                                              \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * ARRAY ALGORITHM SECTION
 * -------------------------------------------------------------------------- */

/**
 * Sorts the array in increasing order.
 */
#define array_sort(id, this) ds_sort_##id(this->arr, this->size)


/**
 * Given a sorted array, finds `key`.
 *
 * @param  key  Value to compare to an array element's data.
 *
 * @return      Pointer to the array element if it was found, or NULL if it was not found.
 */
#define array_find(id, this, key) ds_binary_search_##id((this)->arr, 0, (array_size(this) - 1), key)


/**
 * Merges the arrays in range [`first1`, `last1`) and [`first2`, `last2`) into a new `Array`.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define merge_array(id, first1, last1, first2, last2) merge_array_##id(first1, last1, first2, last2)


/**
 * Creates a new `Array` representing the union of the arrays in range [`first1`, `last1`) and [`first2`, `last2`) 
 * (i.e. elements that are in the first array, the second array, or both - all elements). Both arrays 
 * must have been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_union_array(id, first1, last1, first2, last2) __set_union_array_##id(first1, last1, first2, last2)


/**
 * Creates a new `Array` representing the intersection of the arrays in range [`first1`, `last1`) and 
 * [`first2`, `last2`) (i.e. all elements that both arrays have in common). Both arrays must have been 
 * sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_intersection_array(id, first1, last1, first2, last2) __set_intersection_array_##id(first1, last1, first2, last2)


/**
 * Creates a new `Array` representing the difference of the arrays in range [`first1`, `last1`) and 
 * [`first2`, `last2`) (i.e. all elements that are unique to the first array). Both arrays must have 
 * been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_difference_array(id, first1, last1, first2, last2) __set_difference_array_##id(first1, last1, first2, last2)


/**
 * Creates a new `Array` representing the symmetric difference of the arrays in range [`first1`, `last1`) 
 * and [`first2`, `last2`) (i.e. all elements that neither array has in common). Both arrays must have 
 * been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define set_symmetric_difference_array(id, first1, last1, first2, last2) __set_symmetric_difference_array_##id(first1, last1, first2, last2)


/**
 * Determines whether the array in range [`first1`, `last1`) contains each element in the range [`first2`, `last2`). 
 * Both arrays must have been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          True if the first array contains each element in the second array, false if not.
 */
#define includes_array(id, first1, last1, first2, last2) __includes_array_##id(first1, last1, first2, last2)


/**
 * Generates `Array` code for a specified type and ID, including sort, find, and set functions.
 *
 * @param  id           ID to be used for the array (must be unique).
 * @param  t            Type to be stored in the array.
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the set.
 *                        - If no special copying is required, pass DSDefault_shallowCopy.
 *                        - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param  deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                        - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                        - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_array_withalg(id, t, cmp_lt, copyValue, deleteValue)                                             \
gen_array(id, t, copyValue, deleteValue)                                                                     \
gen_alg(id, t, cmp_lt)                                                                                       \
__gen_alg_set_funcs(id, cmp_lt, Array_##id, array_##id, array_new, t *, iter_next_ARR, iter_deref_ARR, array_push_back, array_insert_fromArray_##id(d_new, array_size(d_new), first1, (size_t)(last1 - first1)), array_insert_fromArray_##id(d_new, array_size(d_new), first2, (size_t)(last2 - first2))) \
                                                                                                             \
__DS_FUNC_PREFIX Array_##id *merge_array_##id(t *first1, t *last1, t *first2, t *last2) {                    \
    Array_##id *a = array_new_##id();                                                                        \
    if (!(first1 && first2)) {                                                                               \
        if (first1) {                                                                                        \
            array_insert_fromArray_##id(a, array_size(a), first1, (size_t)(last1 - first1));                 \
        } else if (first2) {                                                                                 \
            array_insert_fromArray_##id(a, array_size(a), first2, (size_t)(last2 - first2));                 \
        }                                                                                                    \
        return a;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first2, *first1)) {                                                                      \
            array_push_back_##id(a, *first2);                                                                \
            ++first2;                                                                                        \
        } else {                                                                                             \
            array_push_back_##id(a, *first1);                                                                \
            ++first1;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (first1 != last1) {                                                                                   \
        array_insert_fromArray_##id(a, array_size(a), first1, (size_t)(last1 - first1));                     \
    }                                                                                                        \
    if (first2 != last2) {                                                                                   \
        array_insert_fromArray_##id(a, array_size(a), first2, (size_t)(last2 - first2));                     \
    }                                                                                                        \
    return a;                                                                                                \
}                                                                                                            \

#endif
