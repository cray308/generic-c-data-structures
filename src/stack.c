#include "defaults.h"
#include "stack.h"

Stack *stack_new(const DSHelper *helper) {
    if (!helper || helper->size == 0) {
        return NULL;
    }

    Stack *stack = malloc(sizeof(Stack));
    if (!stack) {
        DS_OOM();
    }

    memset(stack, 0, sizeof(Stack));
    stack->helper = *helper;
    return stack;
}

void stack_free(Stack *stack) {
    Node *curr = stack->top, *temp = NULL;
    while (curr != NULL) { /* iterate and remove any elements */
        temp = curr->next;

        if (stack->helper.del) {
            stack->helper.del(curr->data);
        }
        free(curr);

        curr = temp;
    }
    free(stack);
}

int stack_pop(Stack *stack, void *result) {
    if (stack_empty(stack)) { /* nothing to pop */
        return 0;
    }

    Node *top = stack->top;
    stack->top = top->next;

    /* only copy if the result pointer is provided */
    if (result) {
        if (stack->helper.copy) {
            stack->helper.copy(result, top->data);
        } else {
            memcpy(result, top->data, stack->helper.size);
        }
    }

    /* delete top element */
    if (stack->helper.del) {
        stack->helper.del(top->data);
    }
    free(top);
    stack->size--;
    return 1;
}

void stack_push(Stack *stack, const void *item) {
    Node *new = node_new(stack->helper.size);
    if (stack->helper.copy) {
        stack->helper.copy(new->data, item);
    } else {
        memcpy(new->data, item, stack->helper.size);
    }

    /* set this element to be the top */
    new->next = stack->top;
    stack->top = new;
    stack->size++;
}
