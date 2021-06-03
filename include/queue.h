#ifndef QUEUE_H
#define QUEUE_H

#include "ds.h"
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
 * Appends `value` to the back of the queue.

 * @param  value  Value to be emplaced.
 */
#define queue_push(id, this, value) deque_push_back(id, this, value)


/**
 * Generates `Queue` code for a specified type and ID.
 *
 * @param   id           ID to be used for the queue (must be unique).
 * @param   t            Type to be stored in the queue.
 * @param   copyValue    Macro of the form (x, y) which copies y into x to store the element in the queue.
 *                         - If no special copying is required, pass DSDefault_shallowCopy.
 *                         - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param   deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                         - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                         - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_queue(id, t, copyValue, deleteValue) __setup_deque(id, t, Queue_##id, copyValue, deleteValue)

#endif /* QUEUE_H */
