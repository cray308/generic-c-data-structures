#ifndef QUEUE_H
#define QUEUE_H

#include "deque.h"

/**
 * The number of elements in the queue.
 */
#define queue_size(this) deque_size(this)


/**
 * Pointer to the first element in the queue, if it is not empty.
 */
#define queue_front(this) deque_front(this)


/**
 * Pointer to the last element in the queue, if it is not empty.
 */
#define queue_back(this) deque_back(this)


/**
 * Tests whether there are no elements in the queue.
 */
#define queue_empty(this) deque_empty(this)


/**
 * Creates a new, empty queue.
 *
 * @return  Pointer to the newly allocated queue.
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
 * @param  value  Value to be emplaced.
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
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the queue.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass @c DSDefault_deepDelete here.
 */
#define gen_queue_source(id, t, copyValue, deleteValue) __setup_deque_source(id, t, Queue_##id, copyValue, deleteValue)

#endif /* QUEUE_H */
