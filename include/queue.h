#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "deque.h"

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

/**
 * @c unsigned : The number of elements in the queue.
 */
#define queue_size(this) deque_size(this)


/**
 * @c t* : Pointer to the first element in the queue, if it is not empty.
 */
#define queue_front(this) deque_front(this)


/**
 * @c t* : Pointer to the last element in the queue, if it is not empty.
 */
#define queue_back(this) deque_back(this)


/**
 * @c bool : Tests whether there are no elements in the queue.
 */
#define queue_empty(this) deque_empty(this)

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * Creates a new, empty queue.
 *
 * @return  @c Queue* : Newly allocated queue.
 */
#define queue_new(id) deque_new(id)


/**
 * Frees the queue and deletes any remaining elements.
 */
#define queue_free(id, this) deque_free(id, this)


/**
 * Removes the first element in the queue, if it is not empty.
 */
#define queue_pop(id, this) deque_pop_front(id, this)


/**
 * Appends @c value to the back of the queue.
 *
 * @param   value  @c t : Value to be emplaced.
 *
 * @return         @c bool : Whether the operation succeeded.
 */
#define queue_push(id, this, value) deque_push_back(id, this, value)


/**
 * Generates @c Queue function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the queue (must be unique).
 * @param  t   Type to be stored in the queue.
 */
#define gen_queue_headers(id, t) __setup_deque_headers(id, t, Queue_##id)


/**
 * Generates @c Queue function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_queue_headers .
 * @param  t            Type used in @c gen_queue_headers .
 * @param  copyValue    Macro of the form @c (x,y) which copies @c y into @c x
 *                       to store the element in the queue.
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
#define gen_queue_source(id, t, copyValue, deleteValue)                                                      \
        __setup_deque_source(id, t, Queue_##id, copyValue, deleteValue)

#endif /* DS_QUEUE_H */
