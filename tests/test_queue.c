#include "queue.h"
#include <assert.h>

char *strs[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

gen_queue(str, char *)

void test_push(Queue_str *q) {
    assert(queue_size(q) == 0);
    assert(queue_empty(q));

    char **ptr = NULL;
    for (int i = 0; i < 10; ++i) {
        queue_push(str, q, strs[i]);

        ptr = queue_front(q);
        assert(streq(*ptr, strs[0]));

        ptr = queue_back(q);
        assert(streq(*ptr, strs[i]));
        assert(queue_size(q) == i + 1);
    }
    assert(!queue_empty(q));
}

void test_pop(Queue_str *q) {
    char *removed = NULL;
    char **ptr;

    int i = 0;
    for (; i < 5; ++i) {
        queue_pop(str, q, &removed);

        assert(streq(strs[i], removed));
        assert(queue_size(q) == 9 - i);

        ptr = queue_front(q);
        assert(streq(*ptr, strs[i + 1]));
        
        ptr = queue_back(q);
        assert(streq(*ptr, strs[9]));
    }
    assert(queue_size(q) == 5);

    while (queue_pop(str, q, &removed)) {
        assert(streq(strs[i++], removed));
    }
    assert(queue_empty(q));
}

int main(void) {
    Queue_str *q = queue_new(str);
    test_push(q);
    test_pop(q);
    queue_free(str, q);
    return 0;
}
