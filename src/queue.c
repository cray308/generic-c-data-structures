#include "queue.h"

Queue *queue_new(const DSHelper *helper) {
    if (!helper || helper->size == 0) {
        return NULL;
    }

    Queue *queue = malloc(sizeof(Queue));
    if (!queue) {
        DS_OOM();
    }

    memset(queue, 0, sizeof(Queue));
    queue->helper = *helper;
    return queue;
}

void queue_free(Queue *queue) {
    Node *curr = queue->front, *temp = NULL;
    while (curr != NULL) { /* iterate and remove any elements */
        temp = curr->next;

        if (queue->helper.del) {
            queue->helper.del(curr->data);
        }
        free(curr);

        curr = temp;
    }
    free(queue);
}

void queue_push(Queue *queue, const void *item) {
    Node *new = node_new(queue->helper.size);
    if (queue->helper.copy) {
        queue->helper.copy(new->data, item);
    } else {
        memcpy(new->data, item, queue->helper.size);
    }

    /* set this element to be at the back (and also the front, if the queue is empty) */
    if (queue_empty(queue)) {
        queue->front = new;
        queue->back = new;
    } else {
        queue->back->next = new;
        queue->back = new;
    }
    queue->size++;
}

bool queue_pop(Queue *queue, void *result) {
    if (queue_empty(queue)) { /* nothing to pop */
        return false;
    }

    Node *front = queue->front;
    queue->front = front->next;

    /* only copy if result is provided */
    if (result) {
        if (queue->helper.copy) {
            queue->helper.copy(result, front->data);
        } else {
            memcpy(result, front->data, queue->helper.size);
        }
    }
    
    /* delete front element */
    if (queue->helper.del) {
        queue->helper.del(front->data);
    }
    free(front);
    queue->size--;
    return true;
}
