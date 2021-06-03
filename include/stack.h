#ifndef STACK_H
#define STACK_H

#include "ds.h"
#include "deque.h"

/**
 * The number of elements in the stack.
 */
#define stack_size(this) deque_size(this)


/**
 * Tests whether there are no elements in the stack.
 */
#define stack_empty(this) deque_empty(this)


/**
 * Pointer to the top element in the stack, if it is not empty.
 */
#define stack_top(this) deque_back(this)


/**
 * Creates a new, empty stack.
 *
 * @return  Pointer to the newly allocated stack.
 */
#define stack_new(id) deque_new(id)


/**
 * Frees the stack and deletes any remaining elements.
 */
#define stack_free(id, this) deque_free(id, this)


/**
 * Removes the top element from the stack, if it is not empty.
 */
#define stack_pop(id, this) deque_pop_back(id, this)

/**
 * Pushes `value` onto the top of the stack.
 *
 * @param  value  Value to be emplaced.
 */
#define stack_push(id, this, value) deque_push_back(id, this, value)


/**
 * Generates `Stack` code for a specified type and ID.
 *
 * @param   id           ID to be used for the stack (must be unique).
 * @param   t            Type to be stored in the stack.
 * @param   copyValue    Macro of the form (x, y) which copies y into x to store the element in the stack.
 *                         - If no special copying is required, pass DSDefault_shallowCopy.
 *                         - If the value is a string which should be deep-copied, pass DSDefault_deepCopyStr.
 * @param   deleteValue  Macro of the form (x), which is a complement to `copyValue`; if memory was dynamically allocated in `copyValue`, it should be freed here.
 *                         - If DSDefault_shallowCopy was used in `copyValue`, pass DSDefault_shallowDelete here.
 *                         - If DSDefault_deepCopyStr was used in `copyValue`, pass DSDefault_deepDelete here.
 */
#define gen_stack(id, t, copyValue, deleteValue) __setup_deque(id, t, Stack_##id, copyValue, deleteValue)

#endif /* STACK_H */
