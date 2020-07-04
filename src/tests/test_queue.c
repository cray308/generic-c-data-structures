#include "defaults.h"
#include "queue.h"

char *strs[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

void test_push(Queue *q) {
    assert(queue_size(q) == 0);
    assert(queue_empty(q));

    char **ptr = NULL;
    for (int i = 0; i < 10; ++i) {
        queue_push(q, &strs[i]);

        ptr = queue_front(q);
        assert(streq(*ptr, strs[0]));

        ptr = queue_back(q);
        assert(streq(*ptr, strs[i]));
        assert(queue_size(q) == i + 1);
    }
    assert(!queue_empty(q));
}

void test_pop(Queue *q) {
    char *removed;
    char **ptr;

    int i = 0;
    for (; i < 5; ++i) {
        queue_pop(q, &removed);

        assert(streq(strs[i], removed));
        assert(queue_size(q) == 9 - i);

        ptr = queue_front(q);
        assert(streq(*ptr, strs[i + 1]));
        
        ptr = queue_back(q);
        assert(streq(*ptr, strs[9]));
    }
    assert(queue_size(q) == 5);

    while (queue_pop(q, &removed)) {
        assert(streq(strs[i++], removed));
    }
    assert(queue_empty(q));
}

int main(int argc, char *argv[]) {
    Queue *q = queue_new(&str_ptr_helper);
    test_push(q);
    test_pop(q);
    queue_free(q);
    return 0;
}
