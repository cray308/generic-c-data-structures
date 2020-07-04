#ifndef STACK_H
#define STACK_H

#include "ds.h"
#include <stdbool.h>

typedef struct {
    size_t size;
    DSHelper helper;
    Node *top;
} Stack;

/**
 * The number of elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_size(s) ((s)->size)

/**
 * A pointer to the top element in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_top(s) (((s)->top) ? (void*)((s)->top->data) : (NULL))

/**
 * Tests whether there are no elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_empty(s) (!((s)->top))

/**
 * Creates a new empty stack.
 *
 * @param   helper  Pointer to DSHelper struct.
 *
 * @return          A pointer to the newly allocated stack.
 */
Stack *stack_new(const DSHelper *helper);

/**
 * Frees the stack and deletes any remaining items.
 *
 * @param  stack  Pointer to stack.
 */
void stack_free(Stack *stack);

/**
 * If the stack is not empty, copies the top element into "result" and removes the top item in
 *   the stack.
 *
 * @param   stack   Pointer to stack.
 * @param   result  Pointer to where the popped value will be copied.
 *
 * @return          True if an item was popped, false if the stack is empty.
 */
bool stack_pop(Stack *stack, void *result);

/**
 * Pushes a new element to the top of the stack.
 *
 * @param  stack  Pointer to stack.
 * @param  item   Pointer to the item to be pushed.
 */
void stack_push(Stack *stack, const void *item);

#endif // STACK_H
