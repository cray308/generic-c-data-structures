#ifndef QUEUE_H
#define QUEUE_H

#include "ds.h"

/**
 * The number of elements in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_size(q) ((int) (q)->size)


/**
 * A pointer to the front element in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_front(q) (((q)->front) ? &((q)->front->data) : (NULL))


/**
 * A pointer to the back element in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_back(q) (((q)->back) ? &((q)->back->data) : (NULL))


/**
 * Tests whether there are no elements in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_empty(q) (!((q)->front))


/**
 * Creates a new empty queue.
 *
 * @param   id  ID used with gen_queue.
 *
 * @return      A pointer to the newly allocated queue.
 */
#define queue_new(id) queue_new_##id()


/**
 * Frees the queue and deletes any remaining items.
 *
 * @param  id  ID used with gen_queue.
 * @param  q   Pointer to queue.
 */
#define queue_free(id, q) queue_free_##id(q)


/**
 * If the queue is not empty, copies the front element into "result" and removes the front item in
 *   the queue.
 *
 * @param   id      ID used with gen_queue.
 * @param   q       Pointer to queue.
 * @param   result  Pointer to where the dequeued value will be copied.
 *
 * @return          True if an item was popped, false if the queue is empty.
 */
#define queue_pop(id, q, result) queue_pop_##id(q, result)


/**
 * Pushes a new element to the back of the queue.

 * @param  id     ID used with gen_queue.
 * @param  q      Pointer to queue.
 * @param  item   Pointer to the item to be emplaced.
 */
#define queue_push(id, q, item) queue_push_##id(q, item)


/**
 * Generates queue code for a specified type and ID.
 *
 * @param   id  ID to be used for the type stored in the queue (must be unique).
 * @param   t   Type to be stored in a queue node.
 */
#define gen_queue(id, t)                                                                                     \
__gen_node(id, t)                                                                                            \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    Node_##id *front;                                                                                        \
    Node_##id *back;                                                                                         \
} Queue_##id;                                                                                                \
                                                                                                             \
__DS_FUNC_PREFIX Queue_##id *queue_new_##id(void) {                                                          \
    Queue_##id *q = malloc(sizeof(Queue_##id));                                                              \
    if (!q) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
                                                                                                             \
    memset(q, 0, sizeof(Queue_##id));                                                                        \
    return q;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void queue_free_##id(Queue_##id *q) {                                                   \
    Node_##id *curr = q->front, *temp = NULL;                                                                \
    while (curr != NULL) { /* iterate and remove any elements */                                             \
        temp = curr->next;                                                                                   \
        free(curr);                                                                                          \
        curr = temp;                                                                                         \
    }                                                                                                        \
    free(q);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL bool queue_pop_##id(Queue_##id *q, t *result) {                                         \
    if (queue_empty(q)) { /* nothing to pop */                                                               \
        return false;                                                                                        \
    }                                                                                                        \
                                                                                                             \
    Node_##id *front = q->front;                                                                             \
    q->front = front->next;                                                                                  \
    /* only copy if the result pointer is provided */                                                        \
    if (result) {                                                                                            \
        *result = front->data;                                                                               \
    }                                                                                                        \
    /* delete top element */                                                                                 \
    free(front);                                                                                             \
    q->size--;                                                                                               \
    return true;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void queue_push_##id(Queue_##id *q, t item) {                                           \
    Node_##id *new = node_new_##id();                                                                        \
    new->data = item;                                                                                        \
    /* set this element to be at the back (and also the front, if the queue is empty) */                     \
    if (queue_empty(q)) {                                                                                    \
        q->front = new;                                                                                      \
        q->back = new;                                                                                       \
    } else {                                                                                                 \
        q->back->next = new;                                                                                 \
        q->back = new;                                                                                       \
    }                                                                                                        \
    q->size++;                                                                                               \
}                                                                                                            \

#endif /* QUEUE_H */
