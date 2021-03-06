#ifndef DS_ARRAY_H
#define DS_ARRAY_H

#include "alg_helper.h"

#if UINT_MAX == 0xffffffff
#define DS_ARRAY_MAX_SIZE 0xfffffffe
#define DS_ARRAY_SHIFT_THRESHOLD 0x7fffffff
#define ARRAY_ERROR 0xffffffff
#elif UINT_MAX == 0xffff
#define DS_ARRAY_MAX_SIZE 0xfffe
#define DS_ARRAY_SHIFT_THRESHOLD 0x7fff
#define ARRAY_ERROR 0xffff
#endif

/* --------------------------------------------------------------------------
 * ITERATORS
 * -------------------------------------------------------------------------- */

/**
 * @brief @c t* : The starting point for iterating over elements in forward 
 * order.
 */
#define array_iterator_begin(this) array_front(this)


/**
 * @brief @c t* : The ending point for iterating over elements in forward 
 * order.
 */
#define array_iterator_end(this) &(this)->arr[(this)->size]


/**
 * @brief @c t* : The starting point for iterating over elements in reverse 
 * order.
 */
#define array_iterator_rbegin(this) array_back(this)


/**
 * @brief @c t* : The ending point for iterating over elements in reverse 
 * order.
 */
#define array_iterator_rend(this) &(this)->arr[-1]


/**
 * Macro for iterating over the array from front to back.
 *
 * @param  it  @c t* : Assigned to the current element. May be dereferenced
 *              with @c (*it) .
 */
#define array_iter(this, it)                                                             \
        for (it = array_front(this); it != array_iterator_end(this); ++it)


/**
 * Macro for iterating over the array in reverse (from back to front).
 *
 * @param  it  @c t* : Assigned to the current element. May be dereferenced
 *              with @c (*it) .
 */
#define array_riter(this, it)                                                            \
        for (it = array_back(this); it != array_iterator_rend(this); --it)

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * Pointer to the element at index @c i .
 *
 * @param   i  @c unsigned : The index in the array.
 *
 * @return     @c t* : Pointer to the element at this index, or NULL if it is
 *             out of bounds.
 */
#define array_at(this, i) ((i) < (this)->size ? &(this)->arr[i] : NULL)


/**
 * Direct access to the element at index @c i . Does NOT perform bounds 
 * checking.
 *
 * @param   i  @c unsigned : The index in the array.
 *
 * @return     @c t : Element at this index.
 */
#define array_index(this, i) (this)->arr[i]


/**
 * @brief @c t* : Pointer to the first element. The array should be non-empty 
 * when using this macro.
 */
#define array_front(this) &(this)->arr[0]


/**
 * @brief @c t* : Pointer to the last element. The array should be non-empty 
 * when using this macro.
 */
#define array_back(this)                                                                 \
        ((this)->size ? &(this)->arr[(this)->size - 1] : &(this)->arr[-1])


/**
 * @brief @c unsigned : The current number of elements.
 */
#define array_size(this) (this)->size


/**
 * @brief @c unsigned : The maximum number of elements prior to resizing.
 */
#define array_capacity(this) (this)->capacity


/**
 * @brief @c bool : Whether there are no elements in the array.
 */
#define array_empty(this) !(this)->size

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty array.
 *
 * @return  @c Array* : Newly created array.
 */
#define array_new(id) array_new_fromArray_##id(NULL, 0)


/**
 * Creates a new array with size @c n , where each index is set to @c value .
 *
 * @param   n      @c unsigned : Number of elements to initialize.
 * @param   value  @c t : Value to set for each of the indices.
 *
 * @return         @c Array* : Newly created array.
 */
#define array_new_repeatingValue(id, n, value)                                           \
        array_new_repeatingValue_##id(n, value)


/**
 * Creates a new array as a copy of a built-in array @c arr .
 *
 * @param   arr   @c t* : Pointer to the first element to insert. The @c Array
 *                 type in this file can be used by passing
 *                 @c array_at(id,a,index) .
 * @param   n     @c unsigned : Number of elements to include from @c arr .
 *
 * @return        @c Array* : Newly created array.
 */
#define array_new_fromArray(id, arr, n) array_new_fromArray_##id(arr, n)


/**
 * Creates a new array as a copy of @c other .
 *
 * @param   other  @c Array* : Array to copy.
 * 
 * @return         @c Array* : Newly created array.
 */
#define array_createCopy(id, other)                                                      \
        array_new_fromArray_##id((other)->arr, (other)->size)


/**
 * Deletes all elements and frees the array.
 */
#define array_free(id, this) do {                                                        \
    array_clear(id, this); free((this)->arr); free(this);                                \
} while(0)


/**
 * Resizes the array to a size of @c n . If this is less than the current 
 * size, all but the first @c n elements are removed. If this is greater than 
 * the current size, elements are appended to the array with a value of 0.
 *
 * @param   n  @c unsigned : The new array size.
 *
 * @return     @c bool : Whether the operation succeeded.
 */
#define array_resize(id, this, n) array_resize_usingValue_##id(this, n, 0)


/**
 * Resizes the array to a size of @c n . If this is less than the current 
 * size, all but the first @c n elements are removed. If this is greater than 
 * the current size, elements are appended to the array with a value of 
 * @c value .
 *
 * @param   n      @c unsigned : The new array size.
 * @param   value  @c t : Value to hold in the new indices if @c n is greater
 *                  than the current size.
 *
 * @return         @c bool : Whether the operation succeeded.
 */
#define array_resize_usingValue(id, this, n, value)                                      \
        array_resize_usingValue_##id(this, n, value)


/**
 * Requests a change in capacity to @c n . If this is smaller than the current 
 * capacity, nothing is done. If you wish to decrease the capacity, see 
 * @c array_shrink_to_fit .
 *
 * @param   n  @c unsigned : The new capacity.
 *
 * @return     @c bool : Whether the operation succeeded.
 */
#define array_reserve(id, this, n) array_reserve_##id(this, n)


/**
 * Appends @c value to the array.
 *
 * @param  value  @c t : Value to insert.
 */
#define array_push_back(id, this, value)                                                 \
        array_insert_repeatingValue_##id(this, (this)->size, 1, value)


/**
 * Inserts @c value before @c index . Any elements after this index will be 
 * shifted one position to the right. After insertion, the new element will be 
 * located at @c index .
 *
 * @param   index  @c unsigned : Index before which the element will be
 *                  inserted. If this is specified as @c array_size , the
 *                  element is appended.
 * @param   value  @c t : Value to insert.
 *
 * @return         @c unsigned : The index where the element was inserted, or
 *                 @c ARRAY_ERROR if there was an error.
 */
#define array_insert(id, this, index, value)                                             \
        array_insert_repeatingValue_##id(this, index, 1, value)


/**
 * Inserts @c n copies of @c value before @c index .
 *
 * @param   index   @c unsigned : Index before which the elements should be
 *                   inserted. If this is specified as @c array_size , the
 *                   elements are appended.
 * @param   n       @c unsigned : Number of copies of @c value to insert.
 * @param   value   @c t : Value to insert.
 *
 * @return          @c unsigned : The index where the first element was
 *                  inserted, or @c ARRAY_ERROR if there was an error.
 */
#define array_insert_repeatingValue(id, this, index, n, value)                           \
        array_insert_repeatingValue_##id(this, index, n, value)


/**
 * Inserts @c n new elements from built-in array @c arr before @c index . Any 
 * elements after this index will be shifted to the right. After insertion, 
 * the first of the new elements will be located at @c index .
 *
 * @param   index  @c unsigned : Index before which the elements will be
 *                  inserted. If this is specified as @c array_size , the
 *                  elements are appended.
 * @param   arr    @c t* : Pointer to the first element to insert. The @c Array
 *                  type in this file can be used by passing
 *                  @c array_at(id,a,index) .
 * @param   n      @c unsigned : Number of elements to insert from @c arr .
 *
 * @return         @c unsigned : The index where the first element was
 *                 inserted, or @c ARRAY_ERROR if there was an error.
 */
#define array_insert_fromArray(id, this, index, arr, n)                                  \
        array_insert_fromArray_##id(this, index, arr, n)


/**
 * Removes the last element, if the array is not empty.
 */
#define array_pop_back(id, this) array_erase_##id(this, (this)->size - 1, 1)


/**
 * Removes all elements, leaving the array with a size of 0.
 */
#define array_clear(id, this) array_erase_##id(this, 0, (this)->size)


/**
 * Removes @c nelem elements from the array, starting at index @c first .
 *
 * @param  first  @c unsigned : The first index to delete.
 * @param  nelem  @c unsigned : The number of elements to delete. If this is
 *                 @c DS_ARG_NOT_APPLICABLE , it means to erase all elements
 *                 from @c first to the end of the array.
 *
 * @return        @c unsigned : The index after the last element to be deleted.
 *                If the last element to be deleted was the end of the array,
 *                returns @c array_size . If an error occurred, returns
 *                @c ARRAY_ERROR .
 */
#define array_erase(id, this, first, nelem)                                              \
        array_erase_##id(this, first, nelem)


/**
 * @brief If the array's capacity is greater than its size, reallocates only 
 * enough memory to hold @c array_size number of elements. This should only be 
 * used if the array's capacity has grown to be much larger than its size.
 */
#define array_shrink_to_fit(id, this) array_shrink_to_fit_##id(this)


/**
 * Creates a subarray from this array with @c n elements, starting at index 
 * @c start and moving to the next element to include with a step size of 
 * @c step_size .
 *
 * @param   start      @c unsigned : Index to start the subarray.
 * @param   n          @c unsigned : Maximum number of elements to include in
 *                      the new subarray. @c DS_ARG_NOT_APPLICABLE implies to
 *                      include as many elements as the start and step size
 *                      allow.
 * @param   step_size  @c int : How to adjust the index when copying elements.
 *                      1 means move forward 1 index at a time, -1 means move
 *                      backwards one index at a time, 2 would mean every other
 *                      index, etc.
 *
 * @return             @c Array* : Newly allocated array, or NULL if an error
 *                     occurred.
 */
#define array_subarr(id, this, start, n, step_size)                                      \
        array_subarr_##id(this, start, n, step_size)


/**
 * Generates @c Array function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the array (must be unique).
 * @param  t   Type to be stored in the array.
 */
#define gen_array_headers(id, t)                                                         \
                                                                                         \
typedef struct {                                                                         \
    unsigned size;                                                                       \
    unsigned capacity;                                                                   \
    t* arr;                                                                              \
} Array_##id;                                                                            \
                                                                                         \
unsigned char array_reserve_##id(Array_##id *this, unsigned n)                           \
  __attribute__((nonnull));                                                              \
unsigned array_erase_##id(Array_##id *this, unsigned first, unsigned nelem)              \
  __attribute__((nonnull));                                                              \
unsigned array_insert_repeatingValue_##id(Array_##id *this, unsigned index,              \
                                          unsigned n, t const value)                     \
  __attribute__((nonnull (1)));                                                          \
unsigned char array_resize_usingValue_##id(Array_##id *this,                             \
                                           unsigned n, t const value)                    \
  __attribute__((nonnull (1)));                                                          \
unsigned array_insert_fromArray_##id(Array_##id *this, unsigned index,                   \
                                     t const *arr, unsigned n)                           \
  __attribute__((nonnull));                                                              \
Array_##id *array_new_fromArray_##id(t const *arr, unsigned size);                       \
Array_##id *array_new_repeatingValue_##id(unsigned n, t const value)                     \
  __attribute__((nonnull));                                                              \
void array_shrink_to_fit_##id(Array_##id *this) __attribute__((nonnull));                \
Array_##id *array_subarr_##id(Array_##id *this, unsigned start,                          \
                              unsigned n, int step_size)                                 \
  __attribute__((nonnull));                                                              \


/**
 * Generates @c Array function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_array_headers .
 * @param  t            Type used in @c gen_array_headers .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the array.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form @c (x) which is a complement to
 *                       @c copyValue ; if memory was dynamically allocated in
 *                       @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_array_source(id, t, copyValue, deleteValue)                                  \
                                                                                         \
unsigned char array_reserve_##id(Array_##id *this, unsigned n) {                         \
    unsigned ncap = this->capacity;                                                      \
    t* tmp;                                                                              \
    if (n <= ncap) return 1;                                                             \
                                                                                         \
    if (n < DS_ARRAY_SHIFT_THRESHOLD) {                                                  \
        while (ncap < n) ncap <<= 1;                                                     \
    } else {                                                                             \
        if (n == UINT_MAX) return 0;                                                     \
        ncap = DS_ARRAY_MAX_SIZE;                                                        \
    }                                                                                    \
                                                                                         \
    if (!(tmp = realloc(this->arr, ncap * sizeof(t)))) return 0;                         \
    this->capacity = ncap;                                                               \
    this->arr = tmp;                                                                     \
    return 1;                                                                            \
}                                                                                        \
                                                                                         \
unsigned array_erase_##id(Array_##id *this, unsigned first, unsigned nelem) {            \
    unsigned endIdx, i, n = this->size - first, res;                                     \
    if (first >= this->size || !nelem) return ARRAY_ERROR;                               \
                                                                                         \
    if (nelem != DS_ARG_NOT_APPLICABLE) n = min(n, nelem);                               \
    endIdx = first + n;                                                                  \
    res = this->size - n;                                                                \
                                                                                         \
    for (i = first; i < endIdx; ++i) {                                                   \
        deleteValue(this->arr[i]);                                                       \
    }                                                                                    \
                                                                                         \
    if (endIdx < this->size) { /* move elements from endIdx onward back to first */      \
        memmove(&this->arr[first], &this->arr[endIdx],                                   \
                (this->size - endIdx) * sizeof(t));                                      \
        res = first;                                                                     \
    }                                                                                    \
    this->size -= n;                                                                     \
    return res;                                                                          \
}                                                                                        \
                                                                                         \
static unsigned array_check_insert_##id(Array_##id *this, t** end,                       \
                                        unsigned index, unsigned n) {                    \
    unsigned res = this->size;                                                           \
    unsigned newSize = res + n;                                                          \
    if (newSize <= res || index > res || !array_reserve_##id(this, newSize))             \
        return ARRAY_ERROR;                                                              \
                                                                                         \
    if (index != res) { /* insert in the middle of a */                                  \
        unsigned after = index + n;                                                      \
        memmove(&this->arr[after], &this->arr[index],                                    \
                (res - index) * sizeof(t));                                              \
        res = index;                                                                     \
        *end = &this->arr[after];                                                        \
    } else {                                                                             \
        *end = &this->arr[newSize];                                                      \
    }                                                                                    \
    this->size = newSize;                                                                \
    return res;                                                                          \
}                                                                                        \
                                                                                         \
unsigned array_insert_repeatingValue_##id(Array_##id *this, unsigned index,              \
                                          unsigned n, t const value) {                   \
    t* i; t* end;                                                                        \
    unsigned res = array_check_insert_##id(this, &end, index, n);                        \
    if (res != ARRAY_ERROR) {                                                            \
        for (i = &this->arr[res]; i != end; ++i) {                                       \
            copyValue(*i, value);                                                        \
        }                                                                                \
    }                                                                                    \
    return res;                                                                          \
}                                                                                        \
                                                                                         \
unsigned char array_resize_usingValue_##id(Array_##id *this,                             \
                                           unsigned n, t const value) {                  \
    if (n == this->size) return 1;                                                       \
    else if (n < this->size) {                                                           \
        array_erase_##id(this, n, this->size - n);                                       \
        return 1;                                                                        \
    }                                                                                    \
    return array_insert_repeatingValue_##id(this, this->size, n - this->size,            \
                                            value) != ARRAY_ERROR;                       \
}                                                                                        \
                                                                                         \
unsigned array_insert_fromArray_##id(Array_##id *this, unsigned index,                   \
                                     t const *arr, unsigned n) {                         \
    t* i; t* end;                                                                        \
    unsigned res = array_check_insert_##id(this, &end, index, n);                        \
    if (res != ARRAY_ERROR) {                                                            \
        for (i = &this->arr[res]; i != end; ++i, ++arr) {                                \
            copyValue(*i, *arr);                                                         \
        }                                                                                \
    }                                                                                    \
    return res;                                                                          \
}                                                                                        \
                                                                                         \
Array_##id *array_new_fromArray_##id(t const *arr, unsigned size) {                      \
    Array_##id *a = malloc(sizeof(Array_##id));                                          \
    customAssert(a)                                                                      \
    if (!a) return NULL;                                                                 \
    a->arr = malloc(8 * sizeof(t));                                                      \
    customAssert(a->arr)                                                                 \
    if (!a->arr) {                                                                       \
        free(a);                                                                         \
        return NULL;                                                                     \
    }                                                                                    \
    a->size = 0;                                                                         \
    a->capacity = 8;                                                                     \
    if (arr && size) array_insert_fromArray_##id(a, 0, arr, size);                       \
    return a;                                                                            \
}                                                                                        \
                                                                                         \
Array_##id *array_new_repeatingValue_##id(unsigned n, t const value) {                   \
    Array_##id *a = array_new(id);                                                       \
    if (a && n) array_insert_repeatingValue_##id(a, 0, n, value);                        \
    return a;                                                                            \
}                                                                                        \
                                                                                         \
void array_shrink_to_fit_##id(Array_##id *this) {                                        \
    t* tmp;                                                                              \
    unsigned newCap = this->size;                                                        \
    if (this->capacity != 8 && newCap != this->capacity) {                               \
        if (!newCap) newCap = 8;                                                         \
        if ((tmp = realloc(this->arr, newCap * sizeof(t)))) {                            \
            this->capacity = newCap;                                                     \
            this->arr = tmp;                                                             \
        }                                                                                \
    }                                                                                    \
}                                                                                        \
                                                                                         \
Array_##id *array_subarr_##id(Array_##id *this, unsigned start,                          \
                              unsigned n, int step_size) {                               \
    Array_##id *sub;                                                                     \
    if (start >= this->size || !n || !(sub = array_new(id))) return NULL;                \
                                                                                         \
    if (!step_size) step_size = 1;                                                       \
                                                                                         \
    if (step_size < 0) {                                                                 \
        long i = start, end = -1;                                                        \
        if (n != DS_ARG_NOT_APPLICABLE) {                                                \
            const long limit = i + ((long) n * step_size);                               \
            end = max(end, limit);                                                       \
        }                                                                                \
        for (; i > end; i += step_size) {                                                \
            array_push_back(id, sub, this->arr[i]);                                      \
        }                                                                                \
    } else {                                                                             \
        const unsigned ss = (unsigned) step_size;                                        \
        unsigned long i = start, end = this->size;                                       \
        if (n != DS_ARG_NOT_APPLICABLE) {                                                \
            const unsigned long limit = i + (n * ss);                                    \
            end = min(end, limit);                                                       \
        }                                                                                \
        for (; i < end; i += ss) {                                                       \
            array_push_back(id, sub, this->arr[i]);                                      \
        }                                                                                \
    }                                                                                    \
    return sub;                                                                          \
}                                                                                        \


/* --------------------------------------------------------------------------
 * ARRAY ALGORITHM SECTION
 * -------------------------------------------------------------------------- */

/**
 * Sorts the array in increasing order.
 */
#define array_sort(id, this) ds_sort_##id(this->arr, this->size)


/**
 * Given that the array is sorted, finds @c key .
 *
 * @param  key  @c t : Value to find.
 *
 * @return      @c t* : Pointer to the array element if it was found, or NULL
 *              if it was not found.
 */
#define array_find(id, this, key)                                                        \
        ds_binary_search_##id((this)->arr, 0, (int) (this)->size - 1, key)


/**
 * Merges the arrays in range [ @c first1 , @c last1 ) and 
 * [ @c first2 , @c last2 ) into a new array. Both arrays must have been 
 * sorted before this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c Array* : Newly allocated array.
 */
#define merge_array(id, first1, last1, first2, last2)                                    \
        merge_array_##id(first1, last1, first2, last2)


/**
 * Creates a new array representing the union of the arrays in range 
 * [ @c first1 , @c last1 ) and [ @c first2 , @c last2 ) (i.e. elements that 
 * are in the first array, the second array, or both - all elements). Both 
 * arrays must have been sorted before this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c Array* : Newly allocated array.
 */
#define array_union(id, first1, last1, first2, last2)                                    \
        array_union_##id(first1, last1, first2, last2)


/**
 * Creates a new array representing the intersection of the arrays in range 
 * [ @c first1 , @c last1 ) and [ @c first2 , @c last2 ) (i.e. all elements 
 * that both arrays have in common). Both arrays must have been sorted before 
 * this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c Array* : Newly allocated array.
 */
#define array_intersection(id, first1, last1, first2, last2)                             \
        array_intersection_##id(first1, last1, first2, last2)


/**
 * Creates a new array representing the difference of the arrays in range 
 * [ @c first1 , @c last1 ) and [ @c first2 , @c last2 ) (i.e. all elements 
 * that are unique to the first array). Both arrays must have been sorted 
 * before this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c Array* : Newly allocated array.
 */
#define array_difference(id, first1, last1, first2, last2)                               \
        array_difference_##id(first1, last1, first2, last2)


/**
 * Creates a new array representing the symmetric difference of the arrays 
 * in range [ @c first1 , @c last1 ) and [ @c first2 , @c last2 ) (i.e. all 
 * elements that neither array has in common). Both arrays must have been 
 * sorted before this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c Array* : Newly allocated array.
 */
#define array_symmetric_difference(id, first1, last1, first2, last2)                     \
        array_symmetric_difference_##id(first1, last1, first2, last2)


/**
 * Determines whether the array in range [ @c first1 , @c last1 ) contains 
 * each element in the range [ @c first2 , @c last2 ). Both arrays must have 
 * been sorted before this.
 *
 * @param   first1  @c t* : Pointer to start of first array.
 * @param   last1   @c t* : Pointer to end of first array (non-inclusive).
 * @param   first2  @c t* : Pointer to start of second array.
 * @param   last2   @c t* : Pointer to end of second array (non-inclusive).
 *
 * @return          @c bool : Whether the first array contains each element in
 *                  the second array.
 */
#define array_includes(id, first1, last1, first2, last2)                                 \
        array_includes_##id(first1, last1, first2, last2)


/**
 * Generates @c Array function declarations for the specified type and ID, 
 * including sort, find, and set functions.
 *
 * @param  id  ID to be used for the array (must be unique).
 * @param  t   Type to be stored in the array.
 */
#define gen_array_headers_withAlg(id, t)                                                 \
                                                                                         \
gen_array_headers(id, t)                                                                 \
gen_alg_headers(id, t)                                                                   \
                                                                                         \
Array_##id *array_union_##id(t const *first1, t const *last1,                            \
                             t const *first2, t const *last2)                            \
  __attribute__((nonnull (2,4)));                                                        \
Array_##id *array_intersection_##id(t const *first1, t const *last1,                     \
                                    t const *first2, t const *last2)                     \
  __attribute__((nonnull (2,4)));                                                        \
Array_##id *array_difference_##id(t const *first1, t const *last1,                       \
                                  t const *first2, t const *last2)                       \
__attribute__((nonnull (2,4)));                                                          \
Array_##id *array_symmetric_difference_##id(t const *first1, t const *last1,             \
                                            t const *first2, t const *last2)             \
  __attribute__((nonnull (2,4)));                                                        \
unsigned char array_includes_##id(t const *first1, t const *last1,                       \
                                  t const *first2, t const *last2)                       \
  __attribute__((nonnull (2,4)));                                                        \
Array_##id *merge_array_##id(t const *first1, t const *last1,                            \
                             t const *first2, t const *last2)                            \
  __attribute__((nonnull (2,4)));                                                        \


/**
 * Generates @c Array function definitions for the specified type and ID, 
 * including sort, find, and set functions.
 *
 * @param  id           ID used in @c gen_array_headers_withAlg .
 * @param  t            Type used in @c gen_array_headers_withAlg .
 * @param  cmp_lt       Macro of the form @c (x,y) that returns whether @c x is
 *                       strictly less than @c y .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the array.
 *                        - If no special copying is required, pass
 *                         @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be
 *                         deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form @c (x) which is a complement to
 *                       @c copyValue ; if memory was dynamically allocated in
 *                       @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in
 *                         @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in
 *                         @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_array_source_withAlg(id, t, cmp_lt, copyValue, deleteValue)                  \
                                                                                         \
gen_array_source(id, t, copyValue, deleteValue)                                          \
gen_alg_source(id, t, cmp_lt)                                                            \
                                                                                         \
Array_##id *array_union_##id(t const *first1, t const *last1,                            \
                             t const *first2, t const *last2) {                          \
    Array_##id *d_new = array_new(id);                                                   \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            array_insert_fromArray_##id(d_new, d_new->size,                              \
                                        first1, (unsigned) (last1 - first1));            \
        } else if (first2) {                                                             \
            array_insert_fromArray_##id(d_new, d_new->size,                              \
                                        first2, (unsigned) (last2 - first2));            \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first1, *first2)) {                                                  \
            array_push_back(id, d_new, *first1);                                         \
            ++first1;                                                                    \
        } else if (cmp_lt(*first2, *first1)) {                                           \
            array_push_back(id, d_new, *first2);                                         \
            ++first2;                                                                    \
        } else {                                                                         \
            array_push_back(id, d_new, *first1);                                         \
            ++first1;                                                                    \
            ++first2;                                                                    \
        }                                                                                \
    }                                                                                    \
    if (first1 != last1) {                                                               \
        array_insert_fromArray_##id(d_new, d_new->size,                                  \
                                    first1, (unsigned) (last1 - first1));                \
    } else if (first2 != last2) {                                                        \
        array_insert_fromArray_##id(d_new, d_new->size,                                  \
                                    first2, (unsigned) (last2 - first2));                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Array_##id *array_intersection_##id(t const *first1, t const *last1,                     \
                                    t const *first2, t const *last2) {                   \
    Array_##id *d_new = array_new(id);                                                   \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) return d_new;                                          \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first1, *first2)) {                                                  \
            ++first1;                                                                    \
        } else if (cmp_lt(*first2, *first1)) {                                           \
            ++first2;                                                                    \
        } else {                                                                         \
            array_push_back(id, d_new, *first1);                                         \
            ++first1;                                                                    \
            ++first2;                                                                    \
        }                                                                                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Array_##id *array_difference_##id(t const *first1, t const *last1,                       \
                                  t const *first2, t const *last2) {                     \
    Array_##id *d_new = array_new(id);                                                   \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            array_insert_fromArray_##id(d_new, d_new->size,                              \
                                        first1, (unsigned) (last1 - first1));            \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first1, *first2)) {                                                  \
            array_push_back(id, d_new, *first1);                                         \
            ++first1;                                                                    \
        } else if (cmp_lt(*first2, *first1)) {                                           \
            ++first2;                                                                    \
        } else {                                                                         \
            ++first1;                                                                    \
            ++first2;                                                                    \
        }                                                                                \
    }                                                                                    \
    if (first1 != last1) {                                                               \
        array_insert_fromArray_##id(d_new, d_new->size,                                  \
                                    first1, (unsigned) (last1 - first1));                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
Array_##id *array_symmetric_difference_##id(t const *first1, t const *last1,             \
                                            t const *first2, t const *last2) {           \
    Array_##id *d_new = array_new(id);                                                   \
    if (!d_new) return NULL;                                                             \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            array_insert_fromArray_##id(d_new, d_new->size,                              \
                                        first1, (unsigned) (last1 - first1));            \
        } else if (first2) {                                                             \
            array_insert_fromArray_##id(d_new, d_new->size,                              \
                                        first2, (unsigned) (last2 - first2));            \
        }                                                                                \
        return d_new;                                                                    \
    }                                                                                    \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first1, *first2)) {                                                  \
            array_push_back(id, d_new, *first1);                                         \
            ++first1;                                                                    \
        } else if (cmp_lt(*first2, *first1)) {                                           \
            array_push_back(id, d_new, *first2);                                         \
            ++first2;                                                                    \
        } else {                                                                         \
            ++first1;                                                                    \
            ++first2;                                                                    \
        }                                                                                \
    }                                                                                    \
    if (first1 != last1) {                                                               \
        array_insert_fromArray_##id(d_new, d_new->size,                                  \
                                    first1, (unsigned) (last1 - first1));                \
    } else if (first2 != last2) {                                                        \
        array_insert_fromArray_##id(d_new, d_new->size,                                  \
                                    first2, (unsigned) (last2 - first2));                \
    }                                                                                    \
    return d_new;                                                                        \
}                                                                                        \
                                                                                         \
unsigned char array_includes_##id(t const *first1, t const *last1,                       \
                                  t const *first2, t const *last2) {                     \
    if (!(first1 && first2)) return first2 ? 0 : 1;                                      \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first1, *first2)) {                                                  \
            ++first1;                                                                    \
        } else if (cmp_lt(*first2, *first1)) {                                           \
            return 0;                                                                    \
        } else {                                                                         \
            ++first1;                                                                    \
            ++first2;                                                                    \
        }                                                                                \
    }                                                                                    \
    return first2 == last2;                                                              \
}                                                                                        \
                                                                                         \
Array_##id *merge_array_##id(t const *first1, t const *last1,                            \
                             t const *first2, t const *last2) {                          \
    Array_##id *a = array_new(id);                                                       \
    if (!a) return NULL;                                                                 \
    else if (!(first1 && first2)) {                                                      \
        if (first1) {                                                                    \
            array_insert_fromArray_##id(a, a->size,                                      \
                                        first1, (unsigned) (last1 - first1));            \
        } else if (first2) {                                                             \
            array_insert_fromArray_##id(a, a->size,                                      \
                                        first2, (unsigned) (last2 - first2));            \
        }                                                                                \
        return a;                                                                        \
    }                                                                                    \
                                                                                         \
    while (first1 != last1 && first2 != last2) {                                         \
        if (cmp_lt(*first2, *first1)) {                                                  \
            array_push_back(id, a, *first2);                                             \
            ++first2;                                                                    \
        } else {                                                                         \
            array_push_back(id, a, *first1);                                             \
            ++first1;                                                                    \
        }                                                                                \
    }                                                                                    \
    if (first1 != last1) {                                                               \
        array_insert_fromArray_##id(a, a->size,                                          \
                                    first1, (unsigned) (last1 - first1));                \
    } else if (first2 != last2) {                                                        \
        array_insert_fromArray_##id(a, a->size,                                          \
                                    first2, (unsigned) (last2 - first2));                \
    }                                                                                    \
    return a;                                                                            \
}                                                                                        \

#endif /* DS_ARRAY_H */
