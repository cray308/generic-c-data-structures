#include "queue.h"
#include <assert.h>

#define LEN 50
#define FIRST 0
#define LAST (LEN - 1)

gen_queue_headers(int, int)
gen_queue_headers(str, char *)
gen_queue_source(int, int, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_queue_source(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

char *strs[LEN] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049"};

int main(void) {
    int i;
    Queue_int *qi = queue_new(int);
    Queue_str *qs = queue_new(str);
    assert(queue_empty(qi));
    assert(queue_empty(qs));
    assert(queue_size(qi) == 0);
    assert(queue_size(qs) == 0);
    assert(queue_front(qi) == NULL);
    assert(queue_front(qs) == NULL);
    assert(queue_back(qi) == NULL);
    assert(queue_back(qs) == NULL);

    for (i = 0; i < LEN; ++i) {
        queue_push(int, qi, i);
        queue_push(str, qs, strs[i]);
        assert(queue_size(qi) == (unsigned)(i + 1));
        assert(queue_size(qs) == (unsigned)(i + 1));
        assert(*queue_front(qi) == FIRST);
        assert(streq(*queue_front(qs), strs[FIRST]));
        assert(*queue_back(qi) == i);
        assert(streq(*queue_back(qs), strs[i]));
    }
    assert(!queue_empty(qi));
    assert(!queue_empty(qs));
    assert(*queue_front(qi) == FIRST);
    assert(streq(*queue_front(qs), strs[FIRST]));
    assert(*queue_back(qi) == LAST);
    assert(streq(*queue_back(qs), strs[LAST]));

    i = 0;
    while (!queue_empty(qi)) {
        assert(*queue_front(qi) == i++);
        assert(queue_size(qi) == (unsigned)(LEN - i) + 1);
        assert(*queue_back(qi) == LAST);
        queue_pop(int, qi);
    }
    assert(i == LEN);

    i = 0;
    while (!queue_empty(qs)) {
        assert(streq(*queue_front(qs), strs[i++]));
        assert(queue_size(qs) == (unsigned)(LEN - i) + 1);
        assert(streq(*queue_back(qs), strs[LAST]));
        queue_pop(str, qs);
    }
    assert(i == LEN);
    assert(queue_size(qi) == 0);
    assert(queue_size(qs) == 0);
    assert(queue_front(qi) == NULL);
    assert(queue_front(qs) == NULL);
    assert(queue_back(qi) == NULL);
    assert(queue_back(qs) == NULL);

    /* for valgrind: test non-empty free */
    for (i = 0; i < 20; ++i) {
        queue_push(int, qi, i);
        queue_push(str, qs, strs[i]);
    }
    for (i = 0; i < 10; ++i) {
        queue_pop(int, qi);
        queue_pop(str, qs);
    }
    queue_free(int, qi);
    queue_free(str, qs);
    return 0;
}
