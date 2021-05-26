#include "stack.h"
#include <assert.h>

#define LEN 150

gen_stack(int, int, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_stack(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

char strs[LEN][16];

void test_stack(void) {
    Stack_int *si = stack_new(int);
    Stack_str *ss = stack_new(str);
    assert(stack_empty(si) && stack_empty(ss));
    assert(stack_size(si) == 0 && stack_size(ss) == 0);
    assert(stack_top(si) == NULL && stack_top(ss) == NULL);

    for (int i = 0; i < LEN; ++i) {
        stack_push(int, si, i);
        stack_push(str, ss, strs[i]);
        assert(*stack_top(si) == i && streq(*stack_top(ss), strs[i]));
        assert(stack_size(si) == i + 1 && stack_size(ss) == i + 1);
    }

    assert(!stack_empty(si) && !stack_empty(ss));

    {
        int i = LEN - 1;
        while (!stack_empty(si)) {
            assert(*stack_top(si) == i);
            assert(stack_size(si) == i-- + 1);
            stack_pop(int, si);
        }
        assert(i == -1);
    }
    {
        int i = LEN - 1;
        while (!stack_empty(ss)) {
            assert(streq(*stack_top(ss), strs[i]));
            assert(stack_size(ss) == i-- + 1);
            stack_pop(str, ss);
        }
        assert(i == -1);
    }

    assert(stack_size(si) == 0 && stack_size(ss) == 0);
    assert(stack_top(si) == NULL && stack_top(ss) == NULL);
    stack_free(int, si);
    stack_free(str, ss);
}

void test_nonempty_free(void) {
    Stack_str *s = stack_new(str);
    for (int i = 0; i < 50; ++i) {
        stack_push(str, s, strs[i]);
    }
    stack_free(str, s);
}

int main(void) {
    for (int i = 0; i < LEN; ++i) {
        snprintf(strs[i], 16, "%d", i);
    }
    test_stack();
    test_nonempty_free();
    return 0;
}
