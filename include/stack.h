#ifndef DS_STACK_H
#define DS_STACK_H

#include "deque.h"

/* --------------------------------------------------------------------------
 * HELPERS
 * -------------------------------------------------------------------------- */

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

/* --------------------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------------------- */

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
 * Pushes @c value onto the top of the stack.
 *
 * @param   value  Value to be emplaced.
 *
 * @return         Whether the operation succeeded.
 */
#define stack_push(id, this, value) deque_push_back(id, this, value)


/**
 * Generates @c Stack function declarations for the specified type and ID.
 *
 * @param  id  ID to be used for the stack (must be unique).
 * @param  t   Type to be stored in the stack.
 */
#define gen_stack_headers(id, t) __setup_deque_headers(id, t, Stack_##id)


/**
 * Generates @c Stack function definitions for the specified type and ID.
 *
 * @param  id           ID used in @c gen_stack_headers .
 * @param  t            Type used in @c gen_stack_headers .
 * @param  copyValue    Macro of the form (x, y) which copies y into x to store the element in the stack.
 *                        - If no special copying is required, pass @c DSDefault_shallowCopy .
 *                        - If the value is a string which should be deep-copied, pass
 *                         @c DSDefault_deepCopyStr .
 * @param  deleteValue  Macro of the form (x), which is a complement to @c copyValue ; if memory was
 *                       dynamically allocated in @c copyValue , it should be freed here.
 *                        - If @c DSDefault_shallowCopy was used in @c copyValue , pass
 *                         @c DSDefault_shallowDelete here.
 *                        - If @c DSDefault_deepCopyStr was used in @c copyValue , pass
 *                         @c DSDefault_deepDelete here.
 */
#define gen_stack_source(id, t, copyValue, deleteValue)                                                      \
        __setup_deque_source(id, t, Stack_##id, copyValue, deleteValue)

#endif /* DS_STACK_H */
