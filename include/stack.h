#ifndef STACK_H
#define STACK_H

#include "ds.h"
#include "deque.h"

/**
 * The number of elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_size(s) deque_size(s)


/**
 * Tests whether there are no elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_empty(s) deque_empty(s)


/**
 * A pointer to the top element in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_top(s) deque_back(s)


/**
 * Creates a new, empty stack.
 *
 * @param   id  ID used with gen_stack.
 *
 * @return      Pointer to the newly allocated stack.
 */
#define stack_new(id) deque_new(id)


/**
 * Frees the stack and deletes any remaining items.
 *
 * @param  id  ID used with gen_stack.
 * @param  s   Pointer to stack.
 */
#define stack_free(id, s) deque_free(id, s)


/**
 * Removes the top element from the stack, if it is not empty.
 *
 * @param   id      ID used with gen_stack.
 * @param   s       Pointer to stack.
 */
#define stack_pop(id, s) deque_pop_back(id, s)

/**
 * Pushes a new element onto the top of the stack.
 *
 * @param  id     ID used with gen_stack.
 * @param  s      Pointer to stack.
 * @param  item   Item to be emplaced.
 */
#define stack_push(id, s, item) deque_push_back(id, s, item)


/**
 * Generates stack code for a specified type and ID. This macro sets up the stack to use default
 *   (shallow) copying and deleting for each element.
 *
 * @param   id  ID to be used for the stack (must be unique).
 * @param   t   Type to be stored in the stack.
 */
#define gen_stack(id, t) gen_stack_customCopyDelete(id, t, DSDefault_shallowCopy, DSDefault_shallowDelete)


/**
 * Generates stack code for a specified type and ID.
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
#define gen_stack_customCopyDelete(id, t, copyValue, deleteValue) __setup_deque(id, t, Stack_##id, copyValue, deleteValue)

#endif /* STACK_H */
