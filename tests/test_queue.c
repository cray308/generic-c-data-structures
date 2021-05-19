#include "queue.h"
#include <assert.h>

#define LEN 150
#define FIRST 0
#define LAST (LEN - 1)

gen_queue(int, int)
gen_queue_customCopyDelete(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

char strs[LEN][16] = {};

void test_queue(void) {
    Queue_int *qi = queue_new(int);
    Queue_str *qs = queue_new(str);
    assert(queue_empty(qi) && queue_empty(qs));
    assert(queue_size(qi) == 0 && queue_size(qs) == 0);
    assert(queue_front(qi) == NULL && queue_front(qs) == NULL);
    assert(queue_back(qi) == NULL && queue_back(qs) == NULL);

    for (int i = 0; i < LEN; ++i) {
        queue_push(int, qi, i);
        queue_push(str, qs, strs[i]);
        assert(queue_size(qi) == i + 1 && queue_size(qs) == i + 1);
        assert(*queue_front(qi) == FIRST && streq(*queue_front(qs), strs[FIRST]));
        assert(*queue_back(qi) == i && streq(*queue_back(qs), strs[i]));
    }

    assert(!queue_empty(qi) && !queue_empty(qs));
    assert(*queue_front(qi) == FIRST && streq(*queue_front(qs), strs[FIRST]));
    assert(*queue_back(qi) == LAST && streq(*queue_back(qs), strs[LAST]));

    {
        int i = 0;
        while (!queue_empty(qi)) {
            assert(*queue_front(qi) == i++);
            assert(queue_size(qi) == (LEN - i) + 1);
            assert(*queue_back(qi) == LAST);
            queue_pop(int, qi);
        }
        assert(i == LEN);
    }
    {
        int i = 0;
        while (!queue_empty(qs)) {
            assert(streq(*queue_front(qs), strs[i++]));
            assert(queue_size(qs) == (LEN - i) + 1);
            assert(streq(*queue_back(qs), strs[LAST]));
            queue_pop(str, qs);
        }
        assert(i == LEN);
    }

    assert(queue_size(qi) == 0 && queue_size(qs) == 0);
    assert(queue_front(qi) == NULL && queue_front(qs) == NULL);
    assert(queue_back(qi) == NULL && queue_back(qs) == NULL);
    queue_free(int, qi);
    queue_free(str, qs);
}

void test_nonempty_free(void) {
    Queue_str *q = queue_new(str);
    for (int i = 0; i < 50; ++i) {
        queue_push(str, q, strs[i]);
    }
    for (int i = 0; i < 10; ++i) {
        queue_pop(str, q);
    }
    queue_free(str, q);
}

int main(void) {
    for (int i = 0; i < LEN; ++i) {
        snprintf(strs[i], 16, "%d", i);
    }
    test_queue();
    test_nonempty_free();
    return 0;
}
