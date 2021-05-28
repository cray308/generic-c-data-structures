#include "stack.h"
#include <assert.h>

#define LEN 50

gen_stack(int, int, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_stack(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

char *strs[LEN] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049"};

int main(void) {
    Stack_int *si = stack_new(int);
    Stack_str *ss = stack_new(str);
    int i;
    assert(stack_empty(si));
    assert(stack_empty(ss));
    assert(stack_size(si) == 0);
    assert(stack_size(ss) == 0);
    assert(stack_top(si) == NULL);
    assert(stack_top(ss) == NULL);

    for (i = 0; i < LEN; ++i) {
        stack_push(int, si, i);
        stack_push(str, ss, strs[i]);
        assert(*stack_top(si) == i);
        assert(streq(*stack_top(ss), strs[i]));
        assert(stack_size(si) == i + 1);
        assert(stack_size(ss) == i + 1);
    }
    assert(!stack_empty(si));
    assert(!stack_empty(ss));

    i = LEN - 1;
    while (!stack_empty(si)) {
        assert(*stack_top(si) == i);
        assert(stack_size(si) == i-- + 1);
        stack_pop(int, si);
    }
    assert(i == -1);

    i = LEN - 1;
    while (!stack_empty(ss)) {
        assert(streq(*stack_top(ss), strs[i]));
        assert(stack_size(ss) == i-- + 1);
        stack_pop(str, ss);
    }
    assert(i == -1);
    assert(stack_size(si) == 0);
    assert(stack_size(ss) == 0);
    assert(stack_top(si) == NULL);
    assert(stack_top(ss) == NULL);

    /* for valgrind: test non-empty free */
    for (i = 0; i < 10; ++i) {
        stack_push(int, si, i);
        stack_push(str, ss, strs[i]);
    }
    stack_free(int, si);
    stack_free(str, ss);
    return 0;
}
