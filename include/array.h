#ifndef ARRAY_H
#define ARRAY_H

#include "ds.h"

#define ARRAY_ERROR (-1)
#define ARRAY_END(a) ((int) (a)->size)

typedef enum {
    ARR_INIT_EMPTY,
    ARR_INIT_SIZE, /* set array to start with a certain size */
    ARR_INIT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    ARR_INIT_ARRAY /* like Array *nums */
} ArrayInitializer;

typedef enum {
    ARR_INSERT_SINGLE, /* void *  */
    ARR_INSERT_BUILTIN, /* like int nums[] = {1, 2, 3} */
    ARR_INSERT_ARRAY /* like Array *nums   */
} ArrayInsertType;

typedef struct {
    size_t size;
    size_t capacity;
    DSHelper helper;
    char *arr;
} Array;


/**
 * Pointer to the element at index i, provided that the index is less than or equal to
 *   the array's size - 1.
 *
 * @param   a  Pointer to array.
 * @param   i  The index in the array.
 */
inline void *array_at(Array *a, int i) {
    int _idx = modulo(i, a->size);
    return (_idx >= 0) ? (a->arr + (a->helper.size * _idx)) : NULL;
}


/**
 * Macro for accessing a 2D-array.
 *
 * @param   row  Array row.
 * @param   col  Array column.
 *
 * @return       Pointer to the element at row "row" and column "col", similar to
 *                 builtin_array[row][col].
 */
inline void *matrix_at(Array *a, int row, int col) {
    Array *x = array_at(a, row);
    return x ? array_at(x, col) : NULL;
}


/**
 * Pointer to the first element in the array, if the array is not empty.
 *
 * @param   a  Pointer to array.
 */
#define array_front(a) ((a)->size ? (void *)((a)->arr) : NULL)


/**
 * Pointer to the last element in the array, if the array is not empty.
 *
 * @param   a  Pointer to array.
 */
#define array_back(a) ((a)->size ? (void *)((a)->arr + ((a)->helper.size * ((a)->size - 1))) : NULL)


/**
 * Macro for iterating over the array from front to back.
 *
 * @param   a     Pointer to array.
 * @param   eptr  Pointer which is assigned to the current element's data.
 */
#define array_iter(a,eptr) \
    for ((eptr) = array_front(a); \
         (eptr) != NULL ; \
         (eptr) = ((((((char*)(eptr) - (a)->arr) / (a)->helper.size) + 1) == (a)->size) ? NULL : \
                   (void *) ((a)->arr + ((a)->helper.size * ((((char*)(eptr) - (a)->arr) / (a)->helper.size) + 1)))))


/**
 * Macro for iterating over the array in reverse (from back to front).
 *
 * @param   a     Pointer to array.
 * @param   eptr  Pointer which is assigned to the current element's data.
 */
#define array_riter(a,eptr) \
    for ((eptr) = array_back(a); \
         (eptr) != NULL ; \
         (eptr) = ((((((char*)(eptr) - (a)->arr) / (a)->helper.size)) == 0) ? NULL : \
                   (void *) ((a)->arr + ((a)->helper.size * ((((char*)(eptr) - (a)->arr) / (a)->helper.size) - 1)))))


/**
 * The number of elements in the array.
 *
 * @param   a  Pointer to array.
 */
#define array_size(a) ((int) (a)->size)


/**
 * The capacity (maximum number of elements prior to resizing) of the array.
 *
 * @param   a  Pointer to array.
 */
#define array_capacity(a) ((int) (a)->capacity)


/**
 * Tests whether there are no elements in the array.
 *
 * @param   a  Pointer to array.
 */
#define array_empty(a) (!((a)->size))


/**
 * Creates a new array.
 * In (1), an empty Array is created.
 * In (2), an Array is initialized to size n, with each element equal to "value". If "value" is
 *   NULL, the entries are zero-set using memset.
 * In (3), an Array is initialized from a built-in array data type, passed as a pointer to the
 *   function, composed of at most n elements.
 * In (4), an Array is initialized from another Array pointer.
 * 
 * (1) init = ARR_INIT_EMPTY:    array_new(const DSHelper *helper, ArrayInitializer init)
 * (2) init = ARR_INIT_SIZE:     array_new(const DSHelper *helper, ArrayInitializer init, int size, void *value)
 * (3) init = ARR_INIT_BUILTIN:  array_new(const DSHelper *helper, ArrayInitializer init, void *arr, int n)
 * (4) init = ARR_INIT_ARRAY:    array_new(const DSHelper *helper, ArrayInitializer init, Array *other)
 *
 * @param   helper  Pointer to DSHelper struct.
 * @param   init    Type of initializer to execute.
 *
 * @return          Pointer to the newly created array.
 */
Array *array_new(const DSHelper *helper, ArrayInitializer init, ...);


/**
 * Deletes all elements and frees the array.
 *
 * @param  a  Pointer to array.
 */
void array_free(Array *a);


/**
 * Resizes the array to a size of n. If n is less than the current size, all but the first n
 *   elements are removed. If n is greater than the current size, elements are appended to be
 *   copies of the provided value.
 *
 * @param  a      Pointer to array.
 * @param  n      The new array size.
 * @param  value  The value to copy if n is greater than the current size. If this is NULL, the
 *                  elements are set to 0.
 */
void array_resize(Array *a, size_t n, void *value);


/**
 * Request a change in capacity. If this is smaller than the current capacity, nothing is done. If
 *   you wish to decrease the capacity, see array_shrink_to_fit.
 *
 * @param  a  Pointer to array.
 * @param  n  The new capacity.
 */
void array_reserve(Array *a, size_t n);


/**
 * Appends a new element to the array.
 *
 * @param  a  Pointer to array.
 * @param  e  Pointer to the element to be inserted.
 */
void array_push_back(Array *a, void *e);


/**
 * Inserts new elements BEFORE the specified index. Any elements after this index
 * will be shifted one position to the right. After insertion, the first of the new elements
 * (if there are multiple) will be located at the specified index. If the index is equal to the
 * array's size, this is equivalent to a push_back operation.
 * In (1), a single element is inserted.
 * In (2), elements from a built-in array data type are inserted, starting at index "start" and
 *   using "n" elements. "start" MUST be a zero-based index and "n" MUST be positive.
 * In (3), elements from another Array struct are inserted, starting at "start" and using "n"
 *   elements. "start" may be positive or negative, and "n" may be -1.
 * 
 * (1) type = ARR_INSERT_SINGLE:   array_insert(Array *a, int index, ArrayInsertType type, void *value)
 * (2) type = ARR_INSERT_BUILTIN:  array_insert(Array *a, int index, ArrayInsertType type, void *arr, int start, int n)
 * (3) type = ARR_INSERT_ARRAY:    array_insert(Array *a, int index, ArrayInsertType type, Array *other, int start, int n)
 *
 * @param  a      Pointer to Array.
 * @param  index  Index before which the element(s) will be inserted. If this is specified as
 *                  ARRAY_END(a), this is equivalent to push_back.
 * @param  type   Type of insertion to execute.
 *
 * @return        The index where the first item was inserted, or ARRAY_ERROR if there was
 *                  an error.
 */
int array_insert(Array *a, int index, ArrayInsertType type, ...);


/**
 * Removes the last element if the array is not empty.
 *
 * @param  a  Pointer to array.
 */
void array_pop_back(Array *a);


/**
 * Removes all elements, leaving the array with a size of 0.
 *
 * @param  a  Pointer to array.
 */
#define array_clear(a) array_erase((a), 0, array_size((a)))


/**
 * Removes "nelem" elements from the array, starting at index "first".
 *
 * @param  a      Pointer to array.
 * @param  first  The first index to delete.
 * @param  nelem  The number of elements to delete. If this is -1, it means to erase all elements
 *                  from "first" to the end of the array.
 *
 * @return        The index after the last element to be deleted. If the last element to be deleted
 *                    was the end of the array, returns ARRAY_END(a). If an error occurred, returns
 *                    ARRAY_ERROR.
 */
int array_erase(Array *a, int first, int nelem);


/**
 * If the array's capacity is greater than its size, it will reallocate only enough memory to hold
 *   the array's size number of elements. This should only be used if the array's capacity has
 *   grown to be much larger than its size.
 *
 * @param  a  Pointer to array.
 */
void array_shrink_to_fit(Array *a);


/**
 * Sorts the array according to the comparison function in the provided DSHelper struct.
 *
 * @param  a  Pointer to array.
 */
void array_sort(Array *a);


/**
 * Given a sorted array, calls bsearch, determining equality using the comparison function in
 *   the provided DSHelper struct.
 *
 * @param   a    Pointer to array, which must have been sorted beforehand.
 * @param   key  Pointer to the value to compare to an array element's data.
 *
 * @return       Pointer to the array element if it was found, or NULL if it was not found.
 */
void *array_find(Array *a, const void *key);


/**
 * Creates a subarray from "a" with "n" elements, starting at index "start" and moving to
 * the next element to include with a step size of "step_size".
 *
 * @param   start      Index to start the subarray
 * @param   n          Maximum number of elements to include in the new subarray. -1 implies to
 *                       include as many elements as the start and step size allow.
 * @param   step_size  How to adjust the index when copying elements. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return             A newly allocated subarray, or NULL if an error occurred.
 */
Array *array_subarr(Array *a, int start, int n, int step_size);

void vec_2d_helper_copy(void *_dst, const void *_src);
void vec_2d_helper_del(void *_elem);

static const DSHelper vec_2d_helper __attribute__((__unused__)) = {sizeof(Array), vec_2d_helper_copy, vec_2d_helper_del, NULL};

#endif
