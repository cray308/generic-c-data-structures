#include "stack.h"
#include <assert.h>

typedef struct {
    double val;
    char *str;
} DoubleStr;

gen_stack(dbs, DoubleStr)

char *strs[] = {"One", "Two", "Three", "Four", "Five", "Six",
                      "Seven", "Eight", "Nine", "Ten"};

void test_push(Stack_dbs *s) {
    assert(stack_empty(s));
    assert(stack_size(s) == 0);
    DoubleStr dbs;
    DoubleStr *ptr = NULL;

    for (int i = 0; i < 10; ++i) {
        dbs.val = (double) i + 0.14159;
        dbs.str = strs[i];
        stack_push(dbs, s, dbs);
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i]));
        assert(ptr->val >= i && ptr->val <= (i + 1));
        assert(stack_size(s) == i + 1);
    }
    assert(!stack_empty(s));
}

void test_pop(Stack_dbs *s) {
    DoubleStr dbs = {0, NULL};
    DoubleStr *ptr = NULL;

    int i = 9;
    for (; i > 4; --i) {
        stack_pop(dbs, s, &dbs);
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i-1]));
        assert(stack_size(s) == i);
    }
    assert(stack_size(s) == 5);

    while (stack_pop(dbs, s, &dbs)) {
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        i--;
    }

    assert(stack_empty(s));
    assert(stack_size(s) == 0);
}

int main(void) {
    Stack_dbs *s = stack_new(dbs);
    test_push(s);
    test_pop(s);
    stack_free(dbs, s);
    return 0;
}
