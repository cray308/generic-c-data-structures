#ifndef STACK_H
#define STACK_H

#include "ds.h"

/**
 * The number of elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_size(s) ((int) (s)->size)


/**
 * A pointer to the top element in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_top(s) (((s)->top) ? &((s)->top->data) : (NULL))


/**
 * Tests whether there are no elements in the stack.
 *
 * @param   s  Pointer to stack.
 */
#define stack_empty(s) (!((s)->top))


/**
 * Creates a new empty stack.
 *
 * @param   id  ID used with gen_stack.
 *
 * @return      A pointer to the newly allocated stack.
 */
#define stack_new(id) __ds_calloc(1, sizeof(Stack_##id))


/**
 * Frees the stack and deletes any remaining items.
 *
 * @param  id  ID used with gen_stack.
 * @param  s   Pointer to stack.
 */
#define stack_free(id, s) stack_free_##id(s)


/**
 * If the stack is not empty, copies the top element into "result" and removes the top item in
 *   the stack.
 *
 * @param   id      ID used with gen_stack.
 * @param   s       Pointer to stack.
 * @param   result  Pointer to where the popped value will be copied.
 *
 * @return          True if an item was popped, false if the stack is empty.
 */
#define stack_pop(id, s, result) stack_pop_##id(s, result)

/**
 * Pushes a new element to the top of the stack.
 *
 * @param  id     ID used with gen_stack.
 * @param  s      Pointer to stack.
 * @param  item   Pointer to the item to be pushed.
 */
#define stack_push(id, s, item) stack_push_##id(s, item)


/**
 * Generates stack code for a specified type and ID.
 *
 * @param   id  ID to be used for the type stored in the stack (must be unique).
 * @param   t   Type to be stored in a stack node.
 */
#define gen_stack(id, t)                                                                                     \
gen_node(StackEntry_##id, t)                                                                                 \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    StackEntry_##id *top;                                                                                    \
} Stack_##id;                                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX void stack_free_##id(Stack_##id *s) {                                                       \
    StackEntry_##id *curr = s->top, *temp = NULL;                                                            \
    while (curr) { /* iterate and remove any elements */                                                     \
        temp = curr->next;                                                                                   \
        free(curr);                                                                                          \
        curr = temp;                                                                                         \
    }                                                                                                        \
    free(s);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX bool stack_pop_##id(Stack_##id *s, t *result) {                                             \
    if (stack_empty(s)) return false;                                                                        \
    StackEntry_##id *top = s->top;                                                                           \
    s->top = top->next;                                                                                      \
                                                                                                             \
    /* only copy if the result pointer is provided */                                                        \
    if (result) {                                                                                            \
        *result = top->data;                                                                                 \
    }                                                                                                        \
    /* delete top element */                                                                                 \
    free(top);                                                                                               \
    s->size--;                                                                                               \
    return true;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void stack_push_##id(Stack_##id *s, t item) {                                               \
    StackEntry_##id *new = __ds_calloc(1, sizeof(StackEntry_##id));                                          \
    new->data = item;                                                                                        \
    /* set this element to be the top */                                                                     \
    new->next = s->top;                                                                                      \
    s->top = new;                                                                                            \
    s->size++;                                                                                               \
}                                                                                                            \

#endif /* STACK_H */
