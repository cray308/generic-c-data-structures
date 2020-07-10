#ifndef QUEUE_H
#define QUEUE_H

#include "ds.h"
#include <stdbool.h>

typedef struct {
    size_t size;
    DSHelper helper;
    Node *front;
    Node *back;
} Queue;

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
#define queue_front(q) (((q)->front) ? (void*)((q)->front->data) : (NULL))

/**
 * A pointer to the back element in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_back(q) (((q)->back) ? (void*)((q)->back->data) : (NULL))

/**
 * Tests whether there are no elements in the queue.
 *
 * @param   q  Pointer to queue.
 */
#define queue_empty(q) (!((q)->front))

/**
 * Creates a new empty queue.
 *
 * @param   helper  Pointer to DSHelper struct.
 *
 * @return          A pointer to the newly allocated queue.
 */
Queue *queue_new(const DSHelper *helper);

/**
 * Frees the queue and deletes any remaining items.
 *
 * @param  queue  Pointer to queue.
 */
void queue_free(Queue *queue);

/**
 * If the queue is not empty, copies the front element into "result" and removes the front item in
 *   the queue.
 *
 * @param   queue   Pointer to queue.
 * @param   result  Pointer to where the dequeued value will be copied.
 *
 * @return          True if an item was popped, false if the queue is empty.
 */
bool queue_pop(Queue *queue, void *result);

/**
 * Pushes a new element to the back of the queue.
 *
 * @param  queue  Pointer to queue.
 * @param  item   Pointer to the item to be emplaced.
 */
void queue_push(Queue *queue, const void *item);

#endif /* QUEUE_H */
