#ifndef QUEUE_H
#define QUEUE_H

#include "ds.h"
#include "deque.h"

/**
 * The number of elements in the queue.
 */
#define queue_size(q) deque_size(q)


/**
 * Pointer to the first element in the queue, if it is not empty.
 */
#define queue_front(q) deque_front(q)


/**
 * Pointer to the last element in the queue, if it is not empty.
 */
#define queue_back(q) deque_back(q)


/**
 * Tests whether there are no elements in the queue.
 */
#define queue_empty(q) deque_empty(q)


/**
 * Creates a new, empty queue.
 *
 * @return      Pointer to the newly allocated queue.
 */
#define queue_new(id) deque_new(id)


/**
 * Frees the queue and deletes any remaining elements.
 */
#define queue_free(id, q) deque_free(id, q)


/**
 * Removes the first element in the queue, if it is not empty.
 */
#define queue_pop(id, q) deque_pop_front(id, q)


/**
 * Appends `value` to the back of the queue.

 * @param  id     ID used with gen_queue.
 * @param  q      Pointer to queue.
 * @param  value  Value to be emplaced.
 */
#define queue_push(id, q, value) deque_push_back(id, q, value)


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
