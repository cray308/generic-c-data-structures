#ifndef ARRAY_H
#define ARRAY_H

#include "iterator.h"
#include "alg_helper.h"

#define ARRAY_ERROR (-1)

/* --------------------------------------------------------------------------
 * PRIMARY ARRAY SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at index @c i . Performs bounds checking, and if the index is out of bounds, returns NULL.
 *
 * @param  i  The index in the array.
 */
#define array_at(id, this, i) array_at_##id(this, i)


/**
 * Direct access to the element at index @c i . Does NOT perform bounds checking, but faster than @c array_at 
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
#define array_size(this) ((this)->size)


/**
 * The capacity (maximum number of elements prior to resizing) of the array.
 */
#define array_capacity(this) ((this)->capacity)


/**
 * Tests whether there are no elements in the array.
 */
#define array_empty(this) (!((this)->size))


/**
 * Creates a new, empty array.
 *
 * @return  Pointer to the newly created array.
 */
#define array_new(id) array_new_fromArray_##id(NULL, 0)


/**
 * Creates a new array with size @c n , where each index is set to @c value .
 *
 * @param   n      Number of elements to initialize.
 * @param   value  Value to set for each of the indices.
 *
 * @return         Pointer to the newly created array.
 */
#define array_new_repeatingValue(id, n, value) array_new_repeatingValue_##id(n, value)


/**
 * Creates a new array as a copy of a built-in array @c arr which has @c n elements.
 *
 * @param   arr   Pointer to the first element to insert. The @c Array type in this file can be used
 *                  by passing @c array_at(id,a,index) .
 * @param   n     Number of elements to include.
 *
 * @return        Pointer to the newly created array.
 */
#define array_new_fromArray(id, arr, n) array_new_fromArray_##id(arr, n)


/**
 * Creates a new array as a copy of @c Array object @c other .
 *
 * @return  Pointer to the newly created array.
 */
#define array_createCopy(id, other) array_new_fromArray_##id((other)->arr, (other)->size)


/**
 * Deletes all elements and frees the array.
 */
#define array_free(id, this) do { array_clear(id, this); free((this)->arr); free(this); } while(0)


/**
 * Resizes the array to a size of @c n . If this is less than the current size, all but the first @c n 
 * elements are removed. If this is greater than the current size, elements are appended to the array 
 * with a value of 0.
 *
 * @param  n  The new array size.
 */
#define array_resize(id, this, n) array_resize_usingValue_##id(this, n, 0)


/**
 * Resizes the array to a size of @c n . If this is less than the current size, all but the first @c n 
 * elements are removed. If this is greater than the current size, elements are appended to the array 
 * with a value of @c value .
 *
 * @param  n      The new array size.
 * @param  value  Value to hold in the new indices if @c n is greater than the current size.
 */
#define array_resize_usingValue(id, this, n, value) array_resize_usingValue_##id(this, n, value)


/**
 * Requests a change in capacity to @c n . If this is smaller than the current capacity, nothing is 
 * done. If you wish to decrease the capacity, see @c array_shrink_to_fit .
 *
 * @param  n  The new capacity.
 */
#define array_reserve(id, this, n) array_reserve_##id(this, n)


/**
 * Appends @c value to the array.
 *
 * @param  value  Value to insert.
 */
#define array_push_back(id, this, value) array_insert_repeatingValue_##id(this, array_size(this), 1, value)


/**
 * Inserts @c value before @c index . Any elements after this index will be shifted one position 
 * to the right. After insertion, the new element will be located at @c index .
 *
 * @param   index  Index before which the element will be inserted. If this is specified as
 *                   @c array_size , the element is appended.
 * @param   value  Value to insert.
 *
 * @return         The index where the element was inserted, or @c ARRAY_ERROR if there was an error.
 */
#define array_insert(id, this, index, value) array_insert_repeatingValue_##id(this, index, 1, value)


/**
 * Inserts @c n copies of @c value before @c index .
 *
 * @param   index   Index before which the elements should be inserted. If this is specified as
 *                   @c array_size , the elements are appended.
 * @param   n       Number of copies of @c value to insert.
 * @param   value   Value to insert.
 *
 * @return          The index where the first element was inserted, or @c ARRAY_ERROR if there was an error.
 */
#define array_insert_repeatingValue(id, this, index, n, value) array_insert_repeatingValue_##id(this, index, n, value)


/**
 * Inserts @c n new elements from @c arr before @c index . Any elements after this index will be shifted 
 * to the right. After insertion, the first of the new elements will be located at @c index .
 *
 * @param   index  Index before which the elements will be inserted. If this is specified as
 *                   @c array_size , the elements are appended.
 * @param   arr    Pointer to the first element to insert in a built-in array. The @c Array type in 
 *                   this file can be used by passing @c array_at(id,a,index) .
 * @param   n      Number of elements to insert from @c arr .
 *
 * @return         The index where the first element was inserted, or @c ARRAY_ERROR if there was an error.
 */
#define array_insert_fromArray(id, this, index, arr, n) array_insert_fromArray_##id(this, index, arr, n)


/**
 * Removes the last element, if the array is not empty.
 */
#define array_pop_back(id, this) array_erase_##id(this, (this)->size - 1, 1)


/**
 * Removes all elements, leaving the array with a size of 0.
 */
#define array_clear(id, this) array_erase_##id(this, 0, (int)(this)->size)


/**
 * Removes @c nelem elements from the array, starting at index @c first .
 *
 * @param  first  The first index to delete.
 * @param  nelem  The number of elements to delete. If this is -1, it means to erase all elements
 *                  from @c first to the end of the array.
 *
 * @return        The index after the last element to be deleted. If the last element to be deleted
 *                was the end of the array, returns @c array_size . If an error occurred, returns
 *                @c ARRAY_ERROR .
 */
#define array_erase(id, this, first, nelem) array_erase_##id(this, first, nelem)


/**
 * If the array's capacity is greater than its size, reallocates only enough memory to hold @c array_size 
 * number of elements. This should only be used if the array's capacity has grown to be much larger 
 * than its size.
 */
#define array_shrink_to_fit(id, this) array_shrink_to_fit_##id(this)


/**
 * Creates a subarray from this array with @c n elements, starting at index @c start and moving to the 
 * next element to include with a step size of @c step_size .
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
 * Generates @c Array function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the array (must be unique).
 * @param  t   Type to be stored in the array.
 */
#define gen_array_headers(id, t)                                                                             \
                                                                                                             \
typedef struct {                                                                                             \
    unsigned size;                                                                                           \
    unsigned capacity;                                                                                       \
    t *arr;                                                                                                  \
} Array_##id;                                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* array_at_##id(Array_##id *this, unsigned i) {                                        \
    if (i >= this->size) return NULL;                                                                        \
    return &(this->arr[i]);                                                                                  \
}                                                                                                            \
                                                                                                             \
void array_reserve_##id(Array_##id *this, unsigned n);                                                       \
int array_erase_##id(Array_##id *this, unsigned first, int nelem);                                           \
int array_insert_repeatingValue_##id(Array_##id *this, unsigned index, unsigned n, t value);                 \
void array_resize_usingValue_##id(Array_##id *this, unsigned n, t value);                                    \
int array_insert_fromArray_##id(Array_##id *this, unsigned index, t *arr, unsigned n);                       \
Array_##id *array_new_fromArray_##id(t *arr, unsigned size);                                                 \
Array_##id *array_new_repeatingValue_##id(unsigned n, t value);                                              \
void array_shrink_to_fit_##id(Array_##id *this);                                                             \
Array_##id *array_subarr_##id(Array_##id *this, unsigned start, int n, int step_size);                       \


/**
 * Generates @c Array function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_array_headers .
 * @param  t            Type used in @c gen_array_headers .
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the array.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_array_source(id, t, copyValue, deleteValue)                                                      \
                                                                                                             \
void array_reserve_##id(Array_##id *this, unsigned n) {                                                      \
    unsigned ncap;                                                                                           \
    t *tmp;                                                                                                  \
    if (n <= this->capacity) return;                                                                         \
                                                                                                             \
    ncap = this->capacity;                                                                                   \
    while (ncap < n) {                                                                                       \
        ncap <<= 1; /* use multiple of 2 */                                                                  \
    }                                                                                                        \
                                                                                                             \
    tmp = realloc(this->arr, ncap * sizeof(t));                                                              \
    if (!tmp) exit(1);                                                                                       \
    this->capacity = ncap;                                                                                   \
    this->arr = tmp;                                                                                         \
}                                                                                                            \
                                                                                                             \
int array_erase_##id(Array_##id *this, unsigned first, int nelem) {                                          \
    unsigned endIdx, i, n = this->size - first, res;                                                         \
    if (first >= this->size || !nelem) return ARRAY_ERROR;                                                   \
                                                                                                             \
    if (nelem >= 0) n = min(n, (unsigned) nelem);                                                            \
                                                                                                             \
    endIdx = first + n;                                                                                      \
    for (i = first; i < endIdx; ++i) {                                                                       \
        deleteValue(this->arr[i]);                                                                           \
    }                                                                                                        \
                                                                                                             \
    if (endIdx < this->size) { /* move elements from endIdx onward back to first */                          \
        memmove(&this->arr[first], &this->arr[endIdx], (this->size - endIdx) * sizeof(t));                   \
        res = first;                                                                                         \
    } else {                                                                                                 \
        res = this->size - n;                                                                                \
    }                                                                                                        \
    this->size -= n;                                                                                         \
    return (int) res;                                                                                        \
}                                                                                                            \
                                                                                                             \
int array_insert_repeatingValue_##id(Array_##id *this, unsigned index, unsigned n, t value) {                \
    char append;                                                                                             \
    t* i; t* end;                                                                                            \
    unsigned res = this->size;                                                                               \
    if (!n) return ARRAY_ERROR;                                                                              \
    else if (!(append = (index == this->size))) {                                                            \
        if (index >= this->size) return ARRAY_ERROR;                                                         \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(this, this->size + n);                                                                \
                                                                                                             \
    if (!append) { /* insert in the middle of a */                                                           \
        memmove(&this->arr[index + n], &this->arr[index], (this->size - index) * sizeof(t));                 \
        res = index;                                                                                         \
    }                                                                                                        \
    end = &this->arr[res + n];                                                                               \
    for (i = &this->arr[res]; i != end; ++i) {                                                               \
        copyValue((*i), value);                                                                              \
    }                                                                                                        \
    this->size += n;                                                                                         \
    return (int) res;                                                                                        \
}                                                                                                            \
                                                                                                             \
void array_resize_usingValue_##id(Array_##id *this, unsigned n, t value) {                                   \
    if (n == this->size) return;                                                                             \
    else if (n < this->size) {                                                                               \
        array_erase_##id(this, n, (int)(this->size - n));                                                    \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    array_insert_repeatingValue_##id(this, this->size, n - this->size, value);                               \
}                                                                                                            \
                                                                                                             \
int array_insert_fromArray_##id(Array_##id *this, unsigned index, t *arr, unsigned n) {                      \
    char append;                                                                                             \
    t* i; t* end;                                                                                            \
    unsigned res = this->size;                                                                               \
    if (!(arr && n)) return ARRAY_ERROR;                                                                     \
    else if (!(append = (index == this->size))) {                                                            \
        if (index >= this->size) return ARRAY_ERROR;                                                         \
    }                                                                                                        \
                                                                                                             \
    array_reserve_##id(this, this->size + n);                                                                \
                                                                                                             \
    if (!append) { /* insert in the middle of a */                                                           \
        memmove(&this->arr[index + n], &this->arr[index], (this->size - index) * sizeof(t));                 \
        res = index;                                                                                         \
    }                                                                                                        \
    end = &this->arr[res + n];                                                                               \
    for (i = &this->arr[res]; i != end; ++i, ++arr) {                                                        \
        copyValue((*i), (*arr));                                                                             \
    }                                                                                                        \
    this->size += n;                                                                                         \
    return (int) res;                                                                                        \
}                                                                                                            \
                                                                                                             \
Array_##id *array_new_fromArray_##id(t *arr, unsigned size) {                                                \
    Array_##id *a;                                                                                           \
    a = malloc(sizeof(Array_##id));                                                                          \
    if (!a) return NULL;                                                                                     \
    a->arr = malloc(8 * sizeof(t));                                                                          \
    if (!a->arr) {                                                                                           \
        free(a);                                                                                             \
        return NULL;                                                                                         \
    }                                                                                                        \
    a->size = 0;                                                                                             \
    a->capacity = 8;                                                                                         \
    array_insert_fromArray_##id(a, 0, arr, size);                                                            \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
Array_##id *array_new_repeatingValue_##id(unsigned n, t value) {                                             \
    Array_##id *a = array_new(id);                                                                           \
    if (!a) return NULL;                                                                                     \
    array_insert_repeatingValue_##id(a, 0, n, value);                                                        \
    return a;                                                                                                \
}                                                                                                            \
                                                                                                             \
void array_shrink_to_fit_##id(Array_##id *this) {                                                            \
    t *tmp;                                                                                                  \
    if (this->capacity == 8 || this->size == this->capacity || this->size == 0) return;                      \
                                                                                                             \
    tmp = realloc(this->arr, this->size * sizeof(t));                                                        \
    if (!tmp) return;                                                                                        \
    this->capacity = this->size;                                                                             \
    this->arr = tmp;                                                                                         \
}                                                                                                            \
                                                                                                             \
Array_##id *array_subarr_##id(Array_##id *this, unsigned start, int n, int step_size) {                      \
    Array_##id *sub;                                                                                         \
    int end, i = (int) start;                                                                                \
    if (start >= this->size || !n) return NULL;                                                              \
                                                                                                             \
    if (step_size == 0) step_size = 1;                                                                       \
    sub = array_new(id);                                                                                     \
    if (!sub) return NULL;                                                                                   \
                                                                                                             \
    if (step_size < 0) {                                                                                     \
        end = (n < 0) ? -1 : max(-1, i + (n * step_size));                                                   \
        for (; i > end; i += step_size) {                                                                    \
            array_push_back(id, sub, this->arr[i]);                                                          \
        }                                                                                                    \
    } else {                                                                                                 \
        int size = (int) this->size;                                                                         \
        end = (n < 0) ? size : min(size, i + (n * step_size));                                               \
        for (; i < end; i += step_size) {                                                                    \
            array_push_back(id, sub, this->arr[i]);                                                          \
        }                                                                                                    \
    }                                                                                                        \
    return sub;                                                                                              \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * MATRIX SECTION
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at row @c row and column @c col , similar to builtin_array[row][col]. 
 * Performs bounds checking, and if the row or column is out of bounds, returns NULL.
 *
 * @param  row  Matrix row.
 * @param  col  Matrix column.
 */
#define matrix_at(matid, this, row, col) matrix_at_##matid(this, row, col)


/**
 * Direct access to the element at row @c row and column @c col . Does not perform bounds checking.
 *
 * @param  row  Matrix row.
 * @param  col  Matrix column.
 */
#define matrix_index(this, row, col) ((this)->arr[(row)]->arr[(col)])


/**
 * Creates a new matrix of size (@c rows X @c cols ).
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
#define matrix_free(matid, this) do {                                                                        \
    unsigned _mfidx;                                                                                         \
    for (_mfidx = 0; _mfidx < (this)->size; ++_mfidx) {                                                      \
        array_free(matid, ((this)->arr[_mfidx]));                                                            \
    }                                                                                                        \
    free((this)->arr);                                                                                       \
    free(this);                                                                                              \
} while(0)


/**
 * Generates matrix function declarations for the specified type and ID. @c gen_array_headers(id,t) 
 * must have been called prior, because the matrix is an array of @c Array_id .
 *
 * @param  id  ID that was used in the prior call to @c gen_array_headers .
 * @param  t   Type that was used in the prior call to @c gen_array_headers (and thus is the type stored 
 *               in the matrix).
 */
#define gen_matrix_headers(id, t)                                                                            \
                                                                                                             \
gen_array_headers(2d_##id, Array_##id *)                                                                     \
                                                                                                             \
__DS_FUNC_PREFIX_INL t* matrix_at_##id(Array_2d_##id *this, unsigned row, unsigned col) {                    \
    unsigned nCols;                                                                                          \
    if (row >= this->size) return NULL;                                                                      \
    nCols = this->arr[row]->size;                                                                            \
    if (col >= nCols) return NULL;                                                                           \
    return &(this->arr[row]->arr[col]);                                                                      \
}                                                                                                            \
                                                                                                             \
Array_2d_##id *matrix_new_##id(unsigned rows, unsigned cols);                                                \


/**
 * Generates matrix function definitions for the specified type and ID. @c gen_array_source(id,t,copyValue,deleteValue) 
 * must have been called prior.
 *
 * @param  id  ID that was used in @c gen_matrix_headers .
 * @param  t   Type that was used in @c gen_matrix_headers .
 */
#define gen_matrix_source(id, t)                                                                             \
                                                                                                             \
gen_array_source(2d_##id, Array_##id *, DSDefault_shallowCopy, DSDefault_shallowDelete)                      \
                                                                                                             \
Array_2d_##id *matrix_new_##id(unsigned rows, unsigned cols) {                                               \
    Array_2d_##id *m;                                                                                        \
    unsigned i;                                                                                              \
    if (!(rows && cols)) return NULL;                                                                        \
    __ds_malloc(m, sizeof(Array_2d_##id))                                                                    \
    __ds_malloc(m->arr, sizeof(Array_##id *) * rows)                                                         \
    m->size = 0;                                                                                             \
    m->capacity = rows;                                                                                      \
    for (i = 0; i < rows; ++i) {                                                                             \
        Array_##id *row = array_new_repeatingValue_##id(cols, 0);                                            \
        array_push_back(2d_##id, m, row);                                                                    \
    }                                                                                                        \
    return m;                                                                                                \
}                                                                                                            \

/* --------------------------------------------------------------------------
 * ARRAY ALGORITHM SECTION
 * -------------------------------------------------------------------------- */

/**
 * Sorts the array in increasing order.
 */
#define array_sort(id, this) ds_sort_##id(this->arr, this->size)


/**
 * Given a sorted array, finds @c key .
 *
 * @param  key  Value to compare to an array element's data.
 *
 * @return      Pointer to the array element if it was found, or NULL if it was not found.
 */
#define array_find(id, this, key) ds_binary_search_##id((this)->arr, 0, (int) (this)->size - 1, key)


/**
 * Merges the arrays in range [@c first1 , @c last1 ) and [@c first2 , @c last2 ) into a new @c Array .
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
 * Creates a new @c Array representing the union of the arrays in range [@c first1 , @c last1 ) and [@c first2 , @c last2 ) 
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
#define array_union(id, first1, last1, first2, last2) array_union_##id(first1, last1, first2, last2)


/**
 * Creates a new @c Array representing the intersection of the arrays in range [@c first1 , @c last1 ) and 
 * [@c first2 , @c last2 ) (i.e. all elements that both arrays have in common). Both arrays must have been 
 * sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define array_intersection(id, first1, last1, first2, last2) array_intersection_##id(first1, last1, first2, last2)


/**
 * Creates a new @c Array representing the difference of the arrays in range [@c first1 , @c last1 ) and 
 * [@c first2 , @c last2 ) (i.e. all elements that are unique to the first array). Both arrays must have 
 * been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define array_difference(id, first1, last1, first2, last2) array_difference_##id(first1, last1, first2, last2)


/**
 * Creates a new @c Array representing the symmetric difference of the arrays in range [@c first1 , @c last1 ) 
 * and [@c first2 , @c last2 ) (i.e. all elements that neither array has in common). Both arrays must have 
 * been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          Pointer to newly allocated array.
 */
#define array_symmetric_difference(id, first1, last1, first2, last2) array_symmetric_difference_##id(first1, last1, first2, last2)


/**
 * Determines whether the array in range [@c first1 , @c last1 ) contains each element in the range [@c first2 , @c last2 ). 
 * Both arrays must have been sorted before this.
 *
 * @param   first1  Pointer to start of first array.
 * @param   last1   Pointer to end of first array (non-inclusive).
 * @param   first2  Pointer to start of second array.
 * @param   last2   Pointer to end of second array (non-inclusive).
 *
 * @return          True if the first array contains each element in the second array, false if not.
 */
#define array_includes(id, first1, last1, first2, last2) array_includes_##id(first1, last1, first2, last2)


/**
 * Generates @c Array function declarations for the specified type and ID, including sort, find, and set functions.
 *
 * @param  id  ID to be used for the array (must be unique).
 * @param  t   Type to be stored in the array.
 */
#define gen_array_headers_withAlg(id, t)                                                                     \
                                                                                                             \
gen_array_headers(id, t)                                                                                     \
gen_alg_headers(id, t)                                                                                       \
                                                                                                             \
Array_##id *array_union_##id(t* first1, t* last1, t* first2, t* last2);                                      \
Array_##id *array_intersection_##id(t* first1, t* last1, t* first2, t* last2);                               \
Array_##id *array_difference_##id(t* first1, t* last1, t* first2, t* last2);                                 \
Array_##id *array_symmetric_difference_##id(t* first1, t* last1, t* first2, t* last2);                       \
unsigned char array_includes_##id(t* first1, t* last1, t* first2, t* last2);                                 \
Array_##id *merge_array_##id(t *first1, t *last1, t *first2, t *last2);                                      \


/**
 * Generates @c Array function definitions for the specified type and ID, including sort, find, and set functions.
 *
 * @param  id           ID used in @c gen_array_headers_withAlg .
 * @param  t            Type used in @c gen_array_headers_withAlg .
 * @param  cmp_lt       Macro of the form (x, y) that returns whether x is strictly less than y.
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the array.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_array_source_withAlg(id, t, cmp_lt, copyValue, deleteValue)                                      \
                                                                                                             \
gen_array_source(id, t, copyValue, deleteValue)                                                              \
gen_alg_source(id, t, cmp_lt)                                                                                \
                                                                                                             \
Array_##id *array_union_##id(t* first1, t* last1, t* first2, t* last2) {                                     \
    Array_##id *d_new = array_new(id);                                                                       \
    if (!d_new) return NULL;                                                                                 \
    if (!(first1 && first2)) {                                                                               \
        if (first1) {                                                                                        \
            array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));            \
        } else if (first2) {                                                                                 \
            array_insert_fromArray_##id(d_new, d_new->size, first2, (unsigned) (last2 - first2));            \
        }                                                                                                    \
        return d_new;                                                                                        \
    }                                                                                                        \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first1, *first2)) {                                                                      \
            array_push_back(id, d_new, *first1);                                                             \
            ++first1;                                                                                        \
        } else if (cmp_lt(*first2, *first1)) {                                                               \
            array_push_back(id, d_new, *first2);                                                             \
            ++first2;                                                                                        \
        } else {                                                                                             \
            array_push_back(id, d_new, *first1);                                                             \
            ++first1;                                                                                        \
            ++first2;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));                \
    } else if (first2 != last2) {                                                                            \
        array_insert_fromArray_##id(d_new, d_new->size, first2, (unsigned) (last2 - first2));                \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
Array_##id *array_intersection_##id(t* first1, t* last1, t* first2, t* last2) {                              \
    Array_##id *d_new = array_new(id);                                                                       \
    if (!d_new) return NULL;                                                                                 \
    if (!(first1 && first2)) return d_new;                                                                   \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first1, *first2)) {                                                                      \
            ++first1;                                                                                        \
        } else if (cmp_lt(*first2, *first1)) {                                                               \
            ++first2;                                                                                        \
        } else {                                                                                             \
            array_push_back(id, d_new, *first1);                                                             \
            ++first1;                                                                                        \
            ++first2;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
Array_##id *array_difference_##id(t* first1, t* last1, t* first2, t* last2) {                                \
    Array_##id *d_new = array_new(id);                                                                       \
    if (!d_new) return NULL;                                                                                 \
    if (!(first1 && first2)) {                                                                               \
        if (first1) {                                                                                        \
            array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));            \
        }                                                                                                    \
        return d_new;                                                                                        \
    }                                                                                                        \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first1, *first2)) {                                                                      \
            array_push_back(id, d_new, *first1);                                                             \
            ++first1;                                                                                        \
        } else if (cmp_lt(*first2, *first1)) {                                                               \
            ++first2;                                                                                        \
        } else {                                                                                             \
            ++first1;                                                                                        \
            ++first2;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));                \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
Array_##id *array_symmetric_difference_##id(t* first1, t* last1, t* first2, t* last2) {                      \
    Array_##id *d_new = array_new(id);                                                                       \
    if (!d_new) return NULL;                                                                                 \
    if (!(first1 && first2)) {                                                                               \
        if (first1) {                                                                                        \
            array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));            \
        } else if (first2) {                                                                                 \
            array_insert_fromArray_##id(d_new, d_new->size, first2, (unsigned) (last2 - first2));            \
        }                                                                                                    \
        return d_new;                                                                                        \
    }                                                                                                        \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first1, *first2)) {                                                                      \
            array_push_back(id, d_new, *first1);                                                             \
            ++first1;                                                                                        \
        } else if (cmp_lt(*first2, *first1)) {                                                               \
            array_push_back(id, d_new, *first2);                                                             \
            ++first2;                                                                                        \
        } else {                                                                                             \
            ++first1;                                                                                        \
            ++first2;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    if (first1 != last1) {                                                                                   \
        array_insert_fromArray_##id(d_new, d_new->size, first1, (unsigned) (last1 - first1));                \
    } else if (first2 != last2) {                                                                            \
        array_insert_fromArray_##id(d_new, d_new->size, first2, (unsigned) (last2 - first2));                \
    }                                                                                                        \
    return d_new;                                                                                            \
}                                                                                                            \
                                                                                                             \
unsigned char array_includes_##id(t* first1, t* last1, t* first2, t* last2) {                                \
    if (!(first1 && first2)) {                                                                               \
        if (first1) return 1;                                                                                \
        else if (first2) return 0;                                                                           \
        else return 1;                                                                                       \
    }                                                                                                        \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first1, *first2)) {                                                                      \
            ++first1;                                                                                        \
        } else if (cmp_lt(*first2, *first1)) {                                                               \
            return 0;                                                                                        \
        } else {                                                                                             \
            ++first1;                                                                                        \
            ++first2;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    return first2 == last2;                                                                                  \
}                                                                                                            \
                                                                                                             \
Array_##id *merge_array_##id(t *first1, t *last1, t *first2, t *last2) {                                     \
    Array_##id *a = array_new(id);                                                                           \
    if (!a) return NULL;                                                                                     \
    if (!(first1 && first2)) {                                                                               \
        if (first1) {                                                                                        \
            array_insert_fromArray_##id(a, a->size, first1, (unsigned)(last1 - first1));                     \
        } else if (first2) {                                                                                 \
            array_insert_fromArray_##id(a, a->size, first2, (unsigned)(last2 - first2));                     \
        }                                                                                                    \
        return a;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    while (first1 != last1 && first2 != last2) {                                                             \
        if (cmp_lt(*first2, *first1)) {                                                                      \
            array_push_back(id, a, *first2);                                                                 \
            ++first2;                                                                                        \
        } else {                                                                                             \
            array_push_back(id, a, *first1);                                                                 \
            ++first1;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    if (first1 != last1) {                                                                                   \
        array_insert_fromArray_##id(a, a->size, first1, (unsigned)(last1 - first1));                         \
    }                                                                                                        \
    if (first2 != last2) {                                                                                   \
        array_insert_fromArray_##id(a, a->size, first2, (unsigned)(last2 - first2));                         \
    }                                                                                                        \
    return a;                                                                                                \
}                                                                                                            \

#endif
